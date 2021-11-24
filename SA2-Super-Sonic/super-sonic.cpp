#include "pch.h"

Trampoline* Sonic_Main_t;
Trampoline* Sonic_Display_t;
Trampoline* LoadSonic_t;

ObjectMaster* superSonicManagerPtr = nullptr;

bool isSuper = false;

NJS_TEXNAME SSEffTex[16];
NJS_TEXLIST SSEff_Texlist = { arrayptrandlength(SSEffTex) };

ModelIndex* SuperSonicMdl;

//add super sonic model to sonic model pointer
void __cdecl SetSuperSonicModels(SonicCharObj2* sco2) {
	sco2->ModelList[328].Index = SuperSonicMdl[328].Index;
	sco2->ModelList[328].Model = SuperSonicMdl[328].Model;
	return;
}

void __cdecl LoadSuperSonicCharTextures(SonicCharObj2* sco2) {
	njReleaseTexture(sco2->TextureList);
	sco2->TextureList = 0;
	sco2->TextureList = LoadCharTextures("SSONICTEX");
	return;
}

void __cdecl LoadSSEff_Textures() {
	LoadTextureList("ss_efftex", &SSEff_Texlist);
	LoadTextureList("SSONEFFTEX", &SSONEFFTEX_TEXLIST);
	LoadTextureList("sh_efftex", &Texlist_ShadEff);
	return;
}

void __cdecl TransfoSuperSonic(EntityData1* data, int playerID, SonicCharObj2* sco2) {

	StopMusic();
	Play_SuperSonicMusic();
	ResetMusic();
	ControllerEnabled[playerID] = 0;
	sco2->base.Powerups |= Powerups_Invincibility;
	AnimationIndex* v4 = sco2->MotionList;
	UnloadAnimation(v4);
	sco2->MotionList = 0;
	SetSuperSonicModels(sco2);

	sco2->base.AnimInfo.Next = 0;
	sco2->base.AnimInfo.Animations = SuperSonicAnimationList_r;
	LoadSuperSonicCharTextures(sco2);
	sco2->MotionList = LoadMTNFile((char*)"ssmotion.prs");
	PlayAnimationThing(&sco2->base.AnimInfo);
	sco2->base.Upgrades |= Upgrades_SuperSonic;
	Load_SuperPhysics(data);
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

	if (!data || !isSuper)
		return;

	if (co2->CharID == Characters_Sonic)
		co2->PhysData = PhysicsArray[Characters_Sonic];

	data->Status = 0;
	co2->Upgrades &= ~Upgrades_SuperSonic;
	co2->Powerups &= ~Powerups_Invincibility;
	AnimationIndex* v4 = co2S->MotionList;
	UnloadAnimation(v4);
	co2S->MotionList = 0;
	co2S->base.AnimInfo.Next = 0;
	co2S->base.AnimInfo.Animations = SonicAnimList;
	njReleaseTexture(co2S->TextureList);
	co2S->TextureList = 0;
	co2S->TextureList = LoadCharTextures("SONICTEX");
	co2S->MotionList = LoadMTNFile((char*)"sonicmtn.prs");

	isSuper = false;

	if (IsIngame())
	{
		PlayAnimationThing(&co2S->base.AnimInfo);
		RestoreMusic();
	}

	return;
}

bool CheckUntransform_Input(unsigned char playerID) {

	if (AlwaysSuperSonic)
		return false;

	EntityData1* player = MainCharObj1[playerID];

	if (ControllerPointers[playerID]->press & TransformButton)
	{
		if (player->Action == Action_Jump) {
			unSuper(playerID);
			return true;
		}
	}

	return false;
}

bool CheckTransform_Input(char playerID, EntityData1* player)
{
	if (isSuper)
		return false;

	if (RingCount[playerID] >= 50 || RemoveLimitations) {

		if (Controllers[playerID].press & TransformButton)
		{
			if (player->Action == Action_Jump) {
				return true;
			}
		}
	}

	return false;
}

void SuperSonic_Manager_Delete(ObjectMaster* obj)
{
	superSonicManagerPtr = nullptr;
}

void SuperSonic_Manager(ObjectMaster* obj)
{

	EntityData1* data = obj->Data1.Entity;
	EntityData1* player = MainCharObj1[data->Index];
	SonicCharObj2* sonicCO2 = (SonicCharObj2*)MainCharacter[data->Index]->Data2.Character;

	//if player dies, remove transformation.
	if (GameState == GameStates_LoadFinished && !AlwaysSuperSonic) {
		unSuper(sonicCO2->base.PlayerNum);
		return;
	}

	if (!player || !IsIngame() || GameMode == GameMode_Event) {
		return;
	}

	unsigned char playerID = data->Index;

	switch (data->Action)
	{
	case superSonicSetTask:
		obj->DeleteSub = SuperSonic_Manager_Delete;
		data->Action++;
		break;
	case playerInputCheck:

		if (CheckTransform_Input(playerID, player) || AlwaysSuperSonic)
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

		if (++data->field_6 == 100 || AlwaysSuperSonic)
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

		SubRings(playerID, data);

		break;
	case superSonicUntransfo:
		unSuper(playerID);
		data->Action = playerInputCheck;
		break;
	}
}

void LoadSuperSonicManager(char playNum) {

	if (superSonicManagerPtr)
		return;

	int id = MainCharObj2[playNum]->CharID;
	int id2 = MainCharObj2[playNum]->CharID2;

	if (id == Characters_Sonic && id2 == Characters_Sonic) {
		superSonicManagerPtr = LoadObject(1, "SuperSonic_Manager", SuperSonic_Manager, LoadObj_Data1);

		if (superSonicManagerPtr)
		{
			SuperSonicMdl = LoadMDLFile((char*)"SSONICMDL.PRS");
			superSonicManagerPtr->Data1.Entity->Index = playNum;
		}
	}
}

void SuperSonic_PlayVictoryAnimation(EntityData1* data1, CharObj2Base* co2) {

	if (data1->Action == Action_ObjectControl)
	{
		if (TimerStopped != 0 && (co2->AnimInfo.Next == 54 || co2->AnimInfo.Current == 54)) { //Check if the level is finished
			if (isSuper) {

				co2->AnimInfo.Next = superSonicVictory;
				data1->Action = 120;
			}
		}
	}
}

void Sonic_Main_r(ObjectMaster* obj)
{
	CharObj2Base* co2 = MainCharObj2[obj->Data2.Character->PlayerNum];
	EntityData1* data1 = MainCharObj1[obj->Data2.Character->PlayerNum];
	SonicCharObj2* sco2 = (SonicCharObj2*)obj->Data2.Character;

	SuperSonic_PlayVictoryAnimation(data1, co2);

	ObjectFunc(origin, Sonic_Main_t->Target());
	origin(obj);
}

void __cdecl Sonic_HealdObjectStuff(EntityData1* data1, CharObj2Base* co2) {

	*(int*)0x25F02D8 &= 0xFFFFDBFF;
	*(int*)0x25F02D4 = *(int*)0x1DEB6A0;
	*(int*)0x1A55834 = 0;
	*(int*)0x25F0268 = *(int*)0x1DEB6A8;
	sub_487060(byte_1DE4400);
	SonicHeldObjectThing(data1, co2);
	return;
}

void DrawSonicMotion(EntityData1* data1, SonicCharObj2* sonicCO2) {

	NJS_MOTION* Motion;

	njScale(CURRENT_MATRIX, data1->Scale.x, data1->Scale.y, data1->Scale.z);

	int curAnim = sonicCO2->base.AnimInfo.Current;
	NJS_OBJECT* SonicModel = CharacterModels[sonicCO2->base.AnimInfo.Animations[curAnim].ModelNum].Model;

	if (sonicCO2->base.AnimInfo.AnimationFrame == 2)
	{
		Motion = sonicCO2->base.AnimInfo.Motion;
	}
	else
	{
		if ((data1->Status & Status_Ball) != 0
			&& (sonicCO2->gap35E[2] & 0x11) != 0)
		{
			SonicModel = CharacterModels[sonicCO2->base.AnimInfo.Animations[30].ModelNum].Model;// ball form
			curAnim = 30;
		}
		Motion = CharacterAnimations[sonicCO2->base.AnimInfo.Animations[curAnim].AnimNum].Animation;
	}
	njCnkMotion(SonicModel, Motion, sonicCO2->base.AnimInfo.field_10);// Draw Sonic animated

}


void __cdecl DoSpinDashRotationModel() {

	NJS_VECTOR spinDashThing = { 0.0f, -1.0f, 0.0f };

	njTranslateEx(&spinDashThing);
	NJS_MATRIX_PTR v14 = _nj_current_matrix_ptr_;
	njTranslate(_nj_current_matrix_ptr_, 0.0, 5.0, 0.0);
	njRotateZ(v14, 0x2000);
	njTranslate(v14, 0.0, -5.0, 0.0);
	spinDashThing.x = 0.69999999;
	spinDashThing.y = 1.1;
	spinDashThing.z = 0.80000001;
	njScaleEx(&spinDashThing);
}

void __cdecl Sonic_Display_r(ObjectMaster* obj)
{

	SonicCharObj2* sonicCO2 = (SonicCharObj2*)obj->Data2.Undefined;
	EntityData1* data1 = obj->Data1.Entity;
	char pID = sonicCO2->base.PlayerNum;

	if (!sonicCO2)
		return;

	if (!sonicCO2->TextureList)
		return;

	if (data1->Status & Status_Ball)
		*(DWORD*)(*(DWORD*)Has_texlist_batadvPlayerChara_in_it.gap0 + 32) = (DWORD)&SSEff_Texlist;
	else
		*(DWORD*)(*(DWORD*)Has_texlist_batadvPlayerChara_in_it.gap0 + 32) = (DWORD)sonicCO2->TextureList;

	if (!isSuper || sonicCO2->base.CharID2 != Characters_Sonic) {
		ObjectFunc(origin, Sonic_Display_t->Target());
		return origin(obj);
	}

	sub_486E50(pID);
	memcpy(flt_1A51A00, _nj_current_matrix_ptr_, sizeof(flt_1A51A00));
	SonicCO2PtrExtern = sonicCO2;
	sub_427040(flt_1A51A00);

	if ((data1->field_6 & 2) != 0 && !Pose2PStart_PlayerNum)
	{
		Sonic_HealdObjectStuff(data1, &sonicCO2->base);
		return;
	}

	int curAnim = sonicCO2->base.AnimInfo.Current;

	if (data1->Status & Status_Ball)
		*(DWORD*)(*(DWORD*)Has_texlist_batadvPlayerChara_in_it.gap0 + 32) = (DWORD)&SSEff_Texlist;
	else
		*(DWORD*)(*(DWORD*)Has_texlist_batadvPlayerChara_in_it.gap0 + 32) = (DWORD)sonicCO2->TextureList;

	njPushMatrixEx();
	njTranslateEx(&data1->Position);
	njScale(CURRENT_MATRIX, data1->Scale.x, data1->Scale.y, data1->Scale.z);
	Sonic_HealdObjectStuff(data1, &sonicCO2->base);

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

	if (curAnim != 11 || (data1->Status & (Status_OnObjectColli | Status_Ground)) == 0)
	{
		DrawSonicMotion(data1, sonicCO2);
		njPopMatrixEx();
		return;
	}

	DoSpinDashRotationModel();
	DrawSonicMotion(data1, sonicCO2);
	njPopMatrixEx();
}



DataArray(char, byte_1DE4664, 0x1DE4664, 2);
DataArray(float, flt_19EE0C0, 0x19EE0C0, 12);
DataPointer(SuperSonicCharObj2*, SuperSonicCO2Ptr, 0x19EE150);
DataPointer(ObjectFuncPtr, MotionDrawCallback2, 0x1A55834);

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
	co2 = (SuperSonicCharObj2*)obj->Data2.Undefined;
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




DataPointer(int, playID, 0x1DD92A0);
DataPointer(char, byte_174AFFD, 0x174AFFD);
DataPointer(char, byte_1DE4660, 0x1DE4660);
FunctionPointer(bool, sub_7983F0, (NJS_VECTOR* a1, float a2), 0x7983F0);
DataPointer(float, flt_1DEB070, 0x1DEB070);
FunctionPointer(void, SetCharaModelFlag, (int a1), 0x46F0D0);
DataArray(float, flt_25F02A0, 0x25F02A0, 5);

DataPointer(ObjectFuncPtr, MotionDrawCallbackMaybe, 0x1A55834);

DataPointer(char, byte_174AFE2, 0x174AFE2);

DataPointer(char, byte_174AFE0, 0x174AFE0);

DataPointer(int, dword_25F02D4, 0x25F02D4);
DataPointer(int, dword_1DEB6A0, 0x1DEB6A0);


DataPointer(float, dword_1DEB6A8, 0x1DEB6A8);

DataPointer(float, dword_25F0268, 0x25F0268);

FunctionPointer(void, PlayerAfterImageMaybe, (NJS_OBJECT* a1, int a2, NJS_TEXLIST* a3, float a4, char a5), 0x476C20);

void SonicDisplayAfterImage_r(EntityData1* a1, CharObj2Base* data, SonicCharObj2* a3)
{
	NJS_OBJECT* Model; // edi

	if (isSuper && !data->CharID2)
	{
		return;
	}

	if ((FrameCountIngame & 1) == 0 && !data->CharID && CharacterModels[30].Model)
	{
		Model = CharacterModels[30].Model;
		njPushMatrix(flt_25F02A0);
		njTranslateEx(&a1->Position);
		if (a1->Rotation.z)
		{
			njRotateZ(CURRENT_MATRIX, a1->Rotation.z);
		}
		if (a1->Rotation.x)
		{
			njRotateX(CURRENT_MATRIX, a1->Rotation.x);
		}
		if (a1->Rotation.y != 0x8000)
		{
			njRotateY(CURRENT_MATRIX, 0x8000 - a1->Rotation.y);
		}
		if (!TwoPlayerMode)
		{
			PlayerAfterImageMaybe(Model, 0, a3->TextureList, 0.0, data->PlayerNum);

		}
		njPopMatrix(1u);
	}
}

static void __declspec(naked) SonicDisplayAfterImageASM()
{
	__asm
	{
		push[esp + 08h] // a3
		push[esp + 08h] // data
		push esi // a1

		// Call your __cdecl function here:
		call SonicDisplayAfterImage_r

		pop esi // a1
		add esp, 4 // data
		add esp, 4 // a3
		retn
	}
}

void LoadSonic_r(int playerNum) {

	auto original = reinterpret_cast<decltype(LoadSonic_r)*>(LoadSonic_t->Target());
	original(playerNum);

	LoadSuperSonicManager(playerNum);
	LoadSSEff_Textures();
}

void init_SuperSonic() {

	WriteJump((void*)0x71E460, SonicDisplayAfterImageASM);
	LoadSonic_t = new Trampoline((int)LoadSonic, (int)LoadSonic + 0x6, LoadSonic_r);
	Sonic_Display_t = new Trampoline((int)Sonic_Display, (int)Sonic_Display + 0x6, Sonic_Display_r);
	Sonic_Main_t = new Trampoline((int)Sonic_Main, (int)Sonic_Main + 0x6, Sonic_Main_r);

	initAura_Hack();
}