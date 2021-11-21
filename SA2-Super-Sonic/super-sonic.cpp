#include "pch.h"

Trampoline* LoadCharacters_t;
Trampoline* Sonic_Main_t;
Trampoline* Sonic_Display_t;

ObjectMaster* superSonicManagerPtr = nullptr;

bool isSuper = false;

static NJS_TEXNAME SSONICTEX[7];
static NJS_TEXLIST SSONIC_TEXLIST = { arrayptrandlength(SSONICTEX, Uint32) };


static NJS_TEXNAME SSEFFTex[25];
static NJS_TEXLIST SSEff_Texlist = { arrayptrandlength(SSEFFTex, Uint32) };

void __cdecl LoadSuperSonicTextures(SonicCharObj2* sco2) {
	njReleaseTexture(sco2->TextureList);
	sco2->TextureList = 0;

	if (!isSuper)
		sco2->TextureList = LoadCharTextures("SSONICTEX");
	else
		sco2->TextureList = LoadCharTextures("SONICTEX");
	return;
}


void __cdecl TransfoSuperSonic(EntityData1* data, int playerID, SonicCharObj2* sco2) {

	ControllerEnabled[playerID] = 0;
	sco2->base.Powerups |= Powerups_Invincibility;
	ReleaseMDLFile(sco2->ModelList);
	AnimationIndex* v4 = sco2->MotionList;
	sco2->ModelList = 0;
	UnloadAnimation(v4);
	sco2->MotionList = 0;
	sco2->ModelList = LoadMDLFile((char*)"SSONICMDL.PRS");
	sco2->base.AnimInfo.Next = 0;
	sco2->base.AnimInfo.Animations = SuperSonicAnimationList_r;
	LoadSuperSonicTextures(sco2);
	sco2->MotionList = LoadMTNFile((char*)"ssmotion.prs");
	PlayAnimationThing(&sco2->base.AnimInfo);
	sco2->base.Upgrades |= Upgrades_SuperSonic;

	PlayMusic("BOSS_07.adx");
	ResetMusic();
	DoNextAction_r(playerID, 9, 0);

	isSuper = true;
}

void SubRings(unsigned char player, EntityData1* data) {

	if (RemoveLimitations || AlwaysSuperSonic || MainCharObj2[player]->CharID != Characters_Sonic || !isSuper || TimeStopped != 0)
		return;

	if (FrameCountIngame % 60 == 0 && RingCount[player] > 0) {
		AddRings(player, -1);
	}

	if (RingCount[player] <= 0) {
		data->Action = superSonicUntransfo;
	}

	return;
}

void unSuper(unsigned char player) {

	if (AlwaysSuperSonic)
		return;

	EntityData1* data = MainCharObj1[player];
	CharObj2Base* co2 = MainCharObj2[player];
	SonicCharObj2* co2S = (SonicCharObj2*)MainCharacter[player]->Data2.Character;

	if (!data)
		return;

	if (co2->CharID == Characters_Sonic) //fix an issue with charsel
		co2->PhysData = PhysicsArray[Characters_Sonic];

	data->Status = 0;
	co2->Upgrades &= ~Upgrades_SuperSonic;
	co2->Powerups &= ~Powerups_Invincibility;
	ReleaseMDLFile(co2S->ModelList);
	AnimationIndex* v4 = co2S->MotionList;
	co2S->ModelList = 0;
	UnloadAnimation(v4);
	co2S->MotionList = 0;
	co2S->ModelList = LoadMDLFile((char*)"SONICMDL.PRS");
	co2S->base.AnimInfo.Next = 0;
	co2S->base.AnimInfo.Animations = SonicAnimList;
	njReleaseTexture(co2S->TextureList);
	co2S->TextureList = 0;
	co2S->TextureList = LoadCharTextures("SONICTEX");
	co2S->MotionList = LoadMTNFile((char*)"sonicmtn.prs");
	PlayAnimationThing(&co2S->base.AnimInfo);
	isSuper = false;
	return;
}

bool CheckUntransform_Input(unsigned char playerID) {

	EntityData1* player = MainCharObj1[playerID];

	if (ControllerPointers[playerID]->press & TransformButton)
	{
		unSuper(playerID);
		return true;
	}

	return false;
}

bool CheckPlayer_Input(char playerID, EntityData1* player)
{

	if (Controllers[playerID].press & TransformButton)
	{
		return true;
	}

	return false;
}

void SuperSonic_Manager_Delete(ObjectMaster* obj)
{
	DeleteObject_(superSonicManagerPtr);
	superSonicManagerPtr = nullptr;
}

void SuperSonic_Manager(ObjectMaster* obj)
{

	EntityData1* data = obj->Data1.Entity;
	EntityData1* player = MainCharObj1[data->Index];
	SonicCharObj2* sonicCO2 = (SonicCharObj2*)MainCharacter[data->Index]->Data2.Character;

	if (!player || GameState != GameStates_Ingame)
		return;

	unsigned char playerID = data->Index;

	switch (data->Action)
	{
	case superSonicSetTask:
		obj->DeleteSub = SuperSonic_Manager_Delete;
		data->Action++;
		break;
	case playerInputCheck:

		if (CheckPlayer_Input(playerID, player))
			data->Action++;
		break;
	case superSonicTransfo:
		TransfoSuperSonic(player, playerID, sonicCO2);
		data->Action++;
		break;
	case superSonicTransition:
		sonicCO2->base.AnimInfo.Next = superSonicIntro;
		data->Action++;
		break;
	case superSonicWait:

		if (++data->field_6 == 100)
		{
			ControllerEnabled[playerID] = 1;
			DoNextAction_r(playerID, 15, 0);
			data->Action++;
			data->field_6 = 0;
		}
		break;
	case superSonicOnFrames:
		if (CheckUntransform_Input(playerID)) {

			data->Action = playerInputCheck;
		}

		break;
	case superSonicUntransfo:
		unSuper(playerID);
		data->Action = playerInputCheck;
		break;
	}
}


void Sonic_Main_r(ObjectMaster* obj)
{
	CharObj2Base* co2 = MainCharObj2[obj->Data2.Character->PlayerNum];
	EntityData1* data1 = MainCharObj1[obj->Data2.Character->PlayerNum];
	SonicCharObj2* co2S = (SonicCharObj2*)obj->Data2.Character;

	if (isSuper)
	{
		if (data1)
		{
			if (data1->Status & Status_Ball)
				data1->Status = data1->Status & ~(Status_Ball);
		}
	}

	ObjectFunc(origin, Sonic_Main_t->Target());
	origin(obj);
}

DataPointer(SonicCharObj2*, SonicCO2PtrExtern, 0x01A51A9C);
DataArray(float, flt_1A51A00, 0x1A51A00, 12);
FunctionPointer(void, sub_427040, (float* a1), 0x427040);
DataPointer(char, isLoading, 0x174AFC0);
DataPointer(NJS_VECTOR*, cameraPosMaybe, 0x01DD92B0);
ThiscallFunctionPointer(void, njMotion, (NJS_MOTION* motion, float duration), 0x7819A0);

FunctionPointer(int, sub_42E660, (NJS_CNK_MODEL* a1), 0x42E660);

DataPointer(int, dword_1D19C0C, 0x1D19C0C);


void __cdecl Sonic_Display_r(ObjectMaster* obj)
{

	SonicCharObj2* co2SS; // ebx
	EntityData1* data1; // [esp+1Ch] [ebp-30h]
	NJS_OBJECT* SonicModel;
	NJS_OBJECT* LowPolyModel;
	NJS_OBJECT* v22;

	co2SS = (SonicCharObj2*)obj->Data2.Character;
	data1 = obj->Data1.Entity;
	int animID = co2SS->base.AnimInfo.Current;
	char pID = co2SS->base.PlayerNum;
	double v20 = 0.0f;
	int curAnim2;
	int curAnim3;

	if (!isSuper)
	{
		ObjectFunc(origin, Sonic_Display_t->Target());
		return origin(obj);
	}


	sub_486E50(pID);
	memcpy(flt_1A51A00, _nj_current_matrix_ptr_, sizeof(flt_1A51A00));
	SonicCO2PtrExtern = co2SS;
	sub_427040(flt_1A51A00);

	njSetTexture(co2SS->TextureList);
	njPushMatrixEx();
	njTranslateEx(&data1->Position);
	if (data1->Rotation.z)
	{
		njRotateZ(CURRENT_MATRIX, data1->Rotation.z);
	}
	if (data1->Rotation.x)
	{
		njRotateX(CURRENT_MATRIX, data1->Rotation.x);
	}
	if (data1->Rotation.y != 0x8000)
	{
		njRotateY(CURRENT_MATRIX, 0x8000 - data1->Rotation.y);
	}
	njScale(CURRENT_MATRIX, data1->Scale.x, data1->Scale.y, data1->Scale.z);

	SonicModel = CharacterModels[co2SS->base.AnimInfo.Animations[animID].ModelNum].Model;
	NJS_MOTION* motion = CharacterAnimations[co2SS->base.AnimInfo.Animations[co2SS->base.AnimInfo.Current].AnimNum].Animation;

	njCnkMotion(SonicModel, motion, co2SS->base.AnimInfo.field_10);

	njPopMatrixEx();
}

DataArray(char, byte_1DE4664, 0x1DE4664, 2);
DataArray(float, flt_19EE0C0, 0x19EE0C0, 12);
DataPointer(SuperSonicCharObj2*, SuperSonicCO2Ptr, 0x19EE150);
DataPointer(ObjectFuncPtr, MotionDrawCallback2, 0x1A55834);
DataPointer(char, byte_1DE4400, 0x1DE4400);

void __cdecl Super_Display_r(ObjectMaster* obj)
{
	int v1; // edi
	EntityData1* data; // ebx
	SuperSonicCharObj2* co2; // ebp
	char pNum; // al
	char v5; // cl
	BOOL isSuperSonic; // zf
	NJS_MATRIX_PTR* matrice; // esi
	NJS_MATRIX_PTR* matriceResult; // eax
	int v9; // eax
	char charID; // cl
	NJS_OBJECT* childSS; // ecx
	EntityData1* v12; // esi
	NJS_OBJECT* v13; // ecx
	char v14; // al
	int animID; // [esp+1Ch] [ebp-4h]

	data = obj->Data1.Entity;
	co2 = (SuperSonicCharObj2*)obj->Data2.Character;
	pNum = co2->base.PlayerNum;

	(*(int*)0x1DEB6A0 = *(int*)0x25F02D4);
	*(int*)0x25F02D4 &= 0xFFFFFDFF;
	v5 = byte_1DE4664[pNum];
	(*(int*)0x1DEB6A8 = (*(int*)0x25F0268));
	if (v5 != 1 && v5 != 5)
	{
		(*(int*)0x25F02D8 |= 0x2400u);
	}
	sub_486E50(pNum);
	memcpy(flt_19EE0C0, _nj_current_matrix_ptr_, sizeof(flt_19EE0C0));
	SuperSonicCO2Ptr = co2;
	sub_427040(flt_19EE0C0);
	isSuperSonic = co2->base.CharID == Characters_SuperSonic;
	MotionDrawCallback2 = (ObjectFuncPtr)0x49C130;
	if (!isSuperSonic)
	{
		MotionDrawCallback2 = (ObjectFuncPtr)0x49C5B0;
	}
	matrice = &_nj_current_matrix_ptr_;
	animID = co2->base.AnimInfo.Current;

	njSetTexture(co2->TextureList);
	njPushMatrix(CURRENT_MATRIX);
	njTranslate(*matrice, data->Position.x, data->Position.y, data->Position.z);
	if (data->Rotation.z)
	{
		njRotateZ((float*)matrice, data->Rotation.z);
	}
	if (data->Rotation.x)
	{
		njRotateX((float*)matrice, data->Rotation.x);
	}
	if (data->Rotation.y != 0x8000)
	{
		njRotateY(*matrice, 0x8000 - data->Rotation.y);
	}
	njScale(CURRENT_MATRIX, data->Scale.x, data->Scale.y, data->Scale.z);
	v9 = *(DWORD*)&co2->field_1BC[428];
	if (!v9)
	{
		goto LABEL_21;
	}
	charID = co2->base.CharID;
	if (charID == Characters_SuperSonic)
	{
		childSS = CharacterModels[349].Model->child;
		v12 = (EntityData1*)childSS->model;
		childSS->model = *(NJS_MODEL**)(*(DWORD*)(v9 + 24) + 4);
		goto LABEL_22;
	}
	if (charID == Characters_SuperShadow)
	{
		v13 = CharacterModels[376].Model->child;
		v12 = (EntityData1*)v13->model;
		v13->model = *(NJS_MODEL**)(*(DWORD*)(v9 + 24) + 4);
	}
	else
	{
	LABEL_21:
		v12 = data;
	}
LABEL_22:

	{
		njCnkMotion(CharacterModels[co2->base.AnimInfo.Animations[animID].ModelNum].Model, co2->base.AnimInfo.Motion, FrameCountIngame % co2->base.AnimInfo.Motion->nbFrame);
	}
	if (*(DWORD*)&co2->field_1BC[428])
	{
		v14 = co2->base.CharID;
		if (v14 == Characters_SuperSonic)
		{
			CharacterModels[349].Model->child->model = (NJS_MODEL*)v12;
		}
		else if (v14 == Characters_SuperShadow)
		{
			CharacterModels[376].Model->child->model = (NJS_MODEL*)v12;
		}
	}
	njPopMatrix(1u);
	*(int*)0x25F02D8 &= 0xFFFFDBFF;
	*(int*)0x25F02D4 = *(int*)0x1DEB6A0;
	*(int*)0x1A55834 = 0;
	*(int*)0x25F0268 = *(int*)0x1DEB6A8;
	//sub_487060(byte_1DE4400);
}

void LoadSuperSonicManager() {

	for (int i = 0; i < 2; i++) {

		if (MainCharObj1[i]) {

			int id = MainCharObj2[i]->CharID;
			int id2 = MainCharObj2[i]->CharID2;

			if (id == Characters_Sonic && id2 == Characters_Sonic) {
				superSonicManagerPtr = LoadObject(1, "SuperSonic_Manager", SuperSonic_Manager, LoadObj_Data1);

				if (superSonicManagerPtr)
				{
					LoadTextureList("SSONEFFTEX", &SSONEFFTEX_TEXLIST);
					LoadTextureList("ss_efftex", &SSEff_Texlist);
					superSonicManagerPtr->Data1.Entity->Index = i;
				}
			}
		}
	}


}

void LoadCharacters_r() {

	auto original = reinterpret_cast<decltype(LoadCharacters_r)*>(LoadCharacters_t->Target());
	original();


	LoadSuperSonicManager();
	return;
}


DataPointer(int, playID, 0x1DD92A0);
DataPointer(char, byte_174AFFD, 0x174AFFD);
DataPointer(char, byte_1DE4660, 0x1DE4660);
FunctionPointer(bool, sub_7983F0, (NJS_VECTOR* a1, float a2), 0x7983F0);
DataPointer(float, flt_1DEB070, 0x1DEB070);
FunctionPointer(void, SetCharaModelFlag, (int a1), 0x46F0D0);
DataArray(float, flt_25F02A0, 0x25F02A0, 5);

DataPointer(ObjectFuncPtr, MotionDrawCallbackMaybe, 0x1A55834);

void Sonic_Display_R2(ObjectMaster* obj)
{

	SonicCharObj2* sonicCO2; // ebx
	int pNumCopy; // eax
	char v3; // cl
	BOOL getChar; // zf
	char curChar; // al
	NJS_MATRIX_PTR* v6; // eax
	NJS_MATRIX_PTR* currentMatrice; // esi
	EntityData1* data; // edx
	EntityData1* v9; // edi
	float* v10; // esi
	CollisionInfo* v11; // edx
	Angle v12; // edi
	NJS_MATRIX_PTR* v13; // eax
	float* v14; // ebx
	JiggleInfo* jiggle; // eax
	NJS_OBJECT* v16; // ecx
	NJS_OBJECT* v17; // ecx
	int curAnim3; // edi
	NJS_OBJECT* SonicModel; // esi
	double v20; // st7
	NJS_OBJECT* LowPolyModel; // edx
	NJS_OBJECT* LowPolyModel2; // ecx
	int curAnim; // edx
	double v24; // st6
	double v25; // st7
	double v26; // st6
	int upgrade2; // eax
	NJS_OBJECT* v28; // eax
	int v29; // esi
	signed int v30; // ecx
	NJS_OBJECT* v31; // ecx
	NJS_OBJECT* v32; // eax
	NJS_MATRIX_PTR* v33; // edi
	NJS_OBJECT* v34; // esi
	signed int v35; // ecx
	NJS_OBJECT* v36; // edx
	NJS_OBJECT* a4; // [esp+8h] [ebp-44h]
	NJS_OBJECT* a4a; // [esp+8h] [ebp-44h]
	NJS_OBJECT* a4b; // [esp+8h] [ebp-44h]
	int v40; // [esp+Ch] [ebp-40h]
	EntityData1* data1; // [esp+1Ch] [ebp-30h]
	SonicCharObj2* sonicCO2Copy; // [esp+20h] [ebp-2Ch]
	NJS_OBJECT* v43; // [esp+24h] [ebp-28h]
	NJS_OBJECT* v44; // [esp+24h] [ebp-28h]
	NJS_OBJECT* v45; // [esp+24h] [ebp-28h]
	int curAnim4; // [esp+28h] [ebp-24h]
	float curAnim2; // [esp+28h] [ebp-24h]
	float curAnim2a; // [esp+28h] [ebp-24h]
	float curAnim2b; // [esp+28h] [ebp-24h]
	float curAnim2c; // [esp+28h] [ebp-24h]
	ModelPointers v51; // [esp+2Ch] [ebp-20h]
	float* v52; // [esp+30h] [ebp-1Ch]
	float curFrame; // [esp+30h] [ebp-1Ch]
	float v54; // [esp+30h] [ebp-1Ch]
	NJS_VECTOR v55; // [esp+34h] [ebp-18h] BYREF
	NJS_VECTOR a1; // [esp+40h] [ebp-Ch] BYREF

	sonicCO2 = (SonicCharObj2*)obj->Data2.Character;
	data1 = obj->Data1.Entity;
	data = data1;
	sonicCO2Copy = sonicCO2;
	if ((*(int*)0x174AFE2
		|| *(int*)0x174AFE0 != 1
		|| playID == sonicCO2->base.PlayerNum
		|| !sub_7983F0(&data1->Position, 50.0))
		&& (!Pose2PStart_PlayerNum || Pose2PStart_PlayerNum == sonicCO2->base.PlayerNum + 1))
	{
		*(int*)0x1DEB6A8 = *(int*)0x25F0268;
		*(int*)0x1DEB6A0 = *(int*)0x25F02D4;
		*(int*)0x25F02D4 &= 0xFFFFFDFF;
		pNumCopy = sonicCO2->base.PlayerNum;
		v3 = byte_1DE4664[pNumCopy];
		if (v3 != 1 && v3 != 5)
		{
			*(int*)0x25F02D8 |= 0x2400u;
		}
		switch (byte_174AFFD)
		{
		case 1:
			getChar = playID == 0;
			break;
		case 2:
			getChar = playID == 1;
			break;
		case 3:
			goto LABEL_19;
		default:
			goto LABEL_13;
		}
		if (!getChar)
		{
		LABEL_13:
			sub_486E50(pNumCopy);
			goto LABEL_14;
		}
	LABEL_19:
		byte_1DE4664[pNumCopy & 1] = byte_1DE4660;
	LABEL_14:
		memcpy(flt_1A51A00, _nj_current_matrix_ptr_, sizeof(flt_1A51A00));
		SonicCO2PtrExtern = sonicCO2;
		sub_427040(flt_1A51A00);
		curChar = sonicCO2->base.CharID2;

		{
			getChar = sonicCO2->base.CharID == 0;
			MotionDrawCallbackMaybe = (ObjectFuncPtr)0x71EAA0;         // Calc fire animation and upgrade pos I think
			if (!getChar)
			{
				MotionDrawCallbackMaybe = (ObjectFuncPtr)0x71F5E0;
			}
		}
		curAnim4 = sonicCO2->base.AnimInfo.Current;
		if ((data1->field_6 & 2) != 0 && !Pose2PStart_PlayerNum)
		{
			goto LABEL_130;
		}
		currentMatrice = &_nj_current_matrix_ptr_;
		njSetTexture(sonicCO2->TextureList);
		njPushMatrix(CURRENT_MATRIX);
		if (data1->Action == Action_Pulley)
		{
			v9 = data1;
			a1.z = 0.0;
			v11 = data1->Collision;
			a1.x = 0.0;
			v52 = (float*)&v11->CollisionArray->kind;
			a1.y = -sonicCO2->base.PhysData.Height * 0.5;
			njPushMatrix(flt_25F02A0);
			v10 = (float*)_nj_current_matrix_ptr_;
			if (data1->Rotation.z)
			{
				njRotateZ((float*)_nj_current_matrix_ptr_, data1->Rotation.z);
			}
			if (data1->Rotation.x)
			{
				njRotateX(v10, data1->Rotation.x);
				v9 = data1;
			}
			v12 = -v9->Rotation.y;
			if (v12)
			{
				njRotateY(v10, v12);
			}
			njCalcPoint(&a1, &v55, v10); //njcalc Vector
			v13 = (NJS_MATRIX_PTR*)(v10 - 12);
			njPopMatrix(1u);
			v55.x = v52[2] + v55.x;
			v55.y = v52[3] + v55.y;
			v55.z = v52[4] + v55.z;
			njTranslate(v10, v55.x, v55.y, v55.z);
			data = data1;
		}
		else
		{
			njTranslate(*currentMatrice, data1->Position.x, data1->Position.y, data1->Position.z);
		}
		if (Pose2PStart_PlayerNum == sonicCO2->base.PlayerNum + 1)
		{
			if (data->Rotation.y == 0x8000)
			{
			LABEL_52:
				njScale(CURRENT_MATRIX, data1->Scale.x, data1->Scale.y, data1->Scale.z);
				jiggle = sonicCO2->SpineJiggle;
				if (jiggle)
				{
					if (sonicCO2->base.CharID2 == Characters_Amy)
					{
						v16 = CharacterModels[414].Model->child;
					}
					else
					{
						v17 = CharacterModels[21].Model;
						if (sonicCO2->base.CharID)
						{
							v17 = CharacterModels[86].Model;
						}
						v16 = v17->child;
					}
					v51.basic = (NJS_MODEL*)v16->model;
					v16->model = jiggle->SourceModelCopy->model;
				}
				if (data1->Action == 54 && sonicCO2->base.Animation)
				{
					SetCharaModelFlag(playID);
					//DrawMotionAndObject(CharacterModels[sonicCO2->base.AnimInfo.Animations->ModelNum].Model, flt_1DEB070);
					njCnkMotion(CharacterModels[sonicCO2->base.AnimInfo.Animations->ModelNum].Model, sonicCO2->base.AnimInfo.Motion, sonicCO2->base.AnimInfo.field_10);
					sub_46F1E0(sonicCO2->base.PlayerNum);
					curAnim3 = curAnim4;
				}
				else
				{
					curAnim3 = curAnim4;
					SonicModel = CharacterModels[sonicCO2->base.AnimInfo.Animations[curAnim4].ModelNum].Model;
					if (isLoading)
					{
						v20 = 0.0;
					}
					else
					{
						v20 = CheckDistance(*(&cameraPosMaybe + playID), &data1->Position);
					}
					curAnim2 = v20;
					if (Pose2PStart_PlayerNum != sonicCO2->base.PlayerNum + 1
						&& (SonicModel == CharacterModels[0].Model// sonic model
							&& (LowPolyModel = CharacterModels[48].Model) != 0
							&& (LowPolyModel2 = CharacterModels[31].Model) != 0
							|| SonicModel == CharacterModels[65].Model// shadow model
							&& (LowPolyModel = CharacterModels[123].Model) != 0
							&& (LowPolyModel2 = CharacterModels[104].Model) != 0))
					{
						if (curAnim2 <= 150.0)
						{
							if (curAnim2 > 100.0 || TwoPlayerMode)
							{
								SonicModel = LowPolyModel2;
							}
						}
						else
						{
							SonicModel = LowPolyModel;
						}
					}
					if (sonicCO2->base.AnimInfo.AnimationFrame != 2
						&& (data1->Status & Status_Ball) != 0
						&& sonicCO2->base.CharID2 != Characters_MetalSonic
						&& (sonicCO2->gap35E[2] & 0x11) != 0)
					{
						SonicModel = CharacterModels[sonicCO2->base.AnimInfo.Animations[30].ModelNum].Model;// ball form
						curAnim3 = 30;
					}


					njCnkMotion(SonicModel, CharacterAnimations[sonicCO2->base.AnimInfo.Animations[sonicCO2->base.AnimInfo.Current].AnimNum].Animation, sonicCO2->base.AnimInfo.field_10);
					//DrawMotionAndObject((NJS_ACTION*), sonicCO2->base.AnimInfo.field_10);// Draw Sonic animated
				}
				if (curAnim3 != 30)
				{
					njPushMatrixEx();

					upgrade2 = sonicCO2->base.Upgrades;
					if ((upgrade2 & (Upgrades_ShadowFlameRing | Upgrades_SonicFlameRing)) != 0)
					{
						getChar = (upgrade2 & Upgrades_SonicFlameRing) == 0;
						v28 = CharacterModels[25].Model;
						if (getChar)
						{
							v28 = CharacterModels[100].Model;
						}
						v44 = v28;

					}
					njPopMatrixEx();
				}

				njPopMatrix(1u);
			LABEL_130:
				*(int*)0x25F02D8 &= 0xFFFFDBFF;
				*(int*)0x25F02D4 = *(int*)0x1DEB6A0;
				*(int*)0x1A55834 = 0;
				*(int*)0x25F0268 = *(int*)0x1DEB6A8;
				sub_487060(byte_1DE4400);
				SonicHeldObjectThing(data1, &sonicCO2->base);
				return;
			}
			njRotateY(_nj_current_matrix_ptr_, 0x8000 - data->Rotation.y);
		}
		else
		{
			if (data->Rotation.z)
			{
				njRotateZ(_nj_current_matrix_ptr_, data->Rotation.z);
				data = data1;
			}
			if (data->Rotation.x)
			{
				njRotateX(_nj_current_matrix_ptr_, data->Rotation.x);
				data = data1;
			}
			if (data->Rotation.y != 0x8000)
			{
				njRotateY(_nj_current_matrix_ptr_, 0x8000 - data->Rotation.y);
			}
			if (curAnim4 != 11 || (data1->Status & (Status_OnObjectColli | Status_Ground)) == 0)
			{
				goto LABEL_52;
			}
			v55.x = 0.0;
			v55.y = -1.0;
			v55.z = 0.0;
			njTranslateEx(&v55);
			v14 = (float*)_nj_current_matrix_ptr_;
			njTranslate(_nj_current_matrix_ptr_, 0.0, 5.0, 0.0);
			njRotateZ(v14, 0x2000);
			njTranslate(v14, 0.0, -5.0, 0.0);
			v55.x = 0.69999999;
			v55.y = 1.1;
			v55.z = 0.80000001;

			njScaleEx(&v55);
		}
		sonicCO2 = sonicCO2Copy;
		goto LABEL_52;
	}
}



ObjectFunc(SpinDashAura_Display, 0x756040);
DataPointer(float, flt_B18F54, 0xB18F54);


Trampoline* SpinDashAura_Display_t;

void __cdecl SpinDashAura_Display_r(ObjectMaster* a1)
{
	if (isSuper)
		return;

	ObjectFunc(origin, SpinDashAura_Display_t->Target());
	return origin(a1);
}


Trampoline* DoSpinDashAura_t;


void DoSpinDashAura_Origin(ObjectMaster* a1)
{
	auto target = DoSpinDashAura_t->Target();

	__asm
	{
		mov edi, [a1]
		call target
	}
}

void DoSpinDashAura_r(ObjectMaster* obj)
{
	auraStruct* aura; // esi
	CharObj2Base* co2; // eax
	char charID; // ecx
	char charID2; // cl

	aura = (auraStruct*)obj->Data2.Undefined;

	if (!MainCharObj1[aura->charID] || !aura)
		return;

	charID = aura->charID;
	co2 = MainCharObj2[charID];
	charID2 = co2->CharID2;

	if (charID2 == Characters_Amy || charID2 == Characters_MetalSonic || co2->CharID == Characters_Shadow || !isSuper)
	{
		return DoSpinDashAura_Origin(obj);
	}

	//the aura could work for Super Sonic if he had a ball form but he doesn't.
	return;
}



static void __declspec(naked) DoSpinDashAuraASM()
{
	__asm
	{
		push edi // obj

		// Call your __cdecl function here:
		call DoSpinDashAura_r

		pop edi // obj
		retn
	}
}


void init_SuperSonic() {

	//WriteData((NJS_TEXNAME**)0x7564b3, SSEff_Texlist.textures);	
	//gWriteData((NJS_TEXNAME**)0x75649e, SSEff_Texlist.textures);

	//WriteData((NJS_OBJECT**)0x756478, CharacterModels[328].Model);

	//WriteJump((void*)0x7562A0, DoAuraSpinDashASM);
	//WriteData<1>((int*)0x755ea0, 0xC3);
	LoadCharacters_t = new Trampoline((int)LoadCharacters, (int)LoadCharacters + 0x6, LoadCharacters_r);
	Sonic_Display_t = new Trampoline((int)Sonic_Display, (int)Sonic_Display + 0x6, Sonic_Display_r);
	Sonic_Main_t = new Trampoline((int)Sonic_Main, (int)Sonic_Main + 0x6, Sonic_Main_r);
	SpinDashAura_Display_t = new Trampoline((int)0x756040, (int)0x75604A, SpinDashAura_Display_r);
	DoSpinDashAura_t = new Trampoline((int)0x7562A0, (int)0x7562A7, DoSpinDashAuraASM);
}