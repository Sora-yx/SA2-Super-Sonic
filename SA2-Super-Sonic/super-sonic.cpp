#include "pch.h"
#include "ss.h"

Trampoline* Sonic_Main_t;
Trampoline* Sonic_Display_t;
Trampoline* LoadSonic_t;
Trampoline* Sonic_runsActions_t;

bool isSuper = false;

NJS_TEXNAME SSEffTex[16];
NJS_TEXLIST SSEff_Texlist = { arrayptrandlength(SSEffTex) };

NJS_TEXNAME SonicTex[16];
NJS_TEXLIST Sonic_Texlist = { arrayptrandlength(SonicTex) };

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
	LoadTextureList("sonictex", &Sonic_Texlist); //used for upgrade textures.
	return;
}

void __cdecl LoadSSEff_Textures() {
	LoadTextureList("ss_efftex", &SSEff_Texlist);
	LoadTextureList("SSONEFFTEX", &SSONEFFTEX_TEXLIST);
	LoadTextureList("sh_efftex", &Texlist_ShadEff);
	return;
}

void DeleteJiggle(SonicCharObj2* sonicCO2) {
	JiggleInfo* Jiggle = sonicCO2->SpineJiggle;

	if (Jiggle)
	{
		Delete_Jiggle(Jiggle);
		sonicCO2->SpineJiggle = 0;
	}

	return;
}

void __cdecl initJiggleSuperSonic(SonicCharObj2* sonicCO2) {

	int modelNumber = isSuper == true ? 21 : 349;

	JiggleInfo* jiggleMDL = LoadJiggle(CharacterModels[modelNumber].Model->child);
	sonicCO2->SpineJiggle = jiggleMDL;
	jiggleMDL->type = 18;
	sonicCO2->SpineJiggle->speed = 0.40000001;
	sonicCO2->SpineJiggle->field_8 = 0;
	sonicCO2->SpineJiggle->field_10 = 1024;
	sonicCO2->SpineJiggle->Model = 0;
	return;
}


//ugly as fuck but needed until I find a way to fix the aura matrix shit
void __cdecl SuperSonicHack_Display(bool enabled) {

	if (enabled)
	{
		WriteJump((void*)0x720181, (void*)0x720b20);  //force sonic display to return just after setting the matrix callback thing.
	}
	else { //restore original code
		WriteData<1>((int*)0x720181, 0x3C);
		WriteData<1>((int*)0x720182, 0x8);
		WriteData<1>((int*)0x720183, 0x75);
		WriteData<1>((int*)0x720184, 0x30);
		WriteData<1>((int*)0x720185, 0xC7);
	}
}

AnimationInfo SonicAnimCopy[203];


void __cdecl TransfoSuperSonic(EntityData1* data, int playerID, SonicCharObj2* sco2) {

	if (SuperMusicVersion != None) {
		StopMusic();
		Play_SuperSonicMusic();
		ResetMusic();
	}

	memcpy(&SonicAnimCopy, sco2->base.AnimInfo.Animations, sizeof(SonicAnimCopy));

	ControllerEnabled[playerID] = 0;
	sco2->base.Powerups |= Powerups_Invincibility;
	AnimationIndex* v4 = sco2->MotionList;
	UnloadAnimation(v4);
	sco2->MotionList = 0;
	SetSuperSonicModels(sco2);
	DeleteJiggle(sco2);
	initJiggleSuperSonic(sco2);
	sco2->base.AnimInfo.Next = 0;
	sco2->base.AnimInfo.Animations = SuperSonicAnimationList_r;
	LoadSuperSonicCharTextures(sco2);

	sco2->MotionList = LoadMTNFile((char*)"ssmotion.prs");
	PlayAnimationThing(&sco2->base.AnimInfo);
	Load_SuperPhysics(data);
	sco2->base.Upgrades |= Upgrades_SuperSonic;

	DoNextAction_r(playerID, 9, 0);
	SuperSonicHack_Display(true);
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

	SuperSonicHack_Display(false);

	if (AlwaysSuperSonic)
		return;

	EntityData1* data = MainCharObj1[player];
	CharObj2Base* co2 = MainCharObj2[player];
	SonicCharObj2* co2S = (SonicCharObj2*)MainCharacter[player]->Data2.Character;

	if (!data || !isSuper)
		return;

	if (co2->CharID == Characters_Sonic)
		co2->PhysData = PhysicsArray[Characters_Sonic];

	DeleteJiggle(co2S);
	initJiggleSuperSonic(co2S);
	data->Status = 0;
	co2->Upgrades &= ~Upgrades_SuperSonic;
	co2->Powerups &= ~Powerups_Invincibility;
	AnimationIndex* v4 = co2S->MotionList;
	UnloadAnimation(v4);
	co2S->MotionList = 0;
	co2S->base.AnimInfo.Next = 15;
	co2S->base.AnimInfo.Animations = SonicAnimCopy;
	njReleaseTexture(co2S->TextureList);
	co2S->TextureList = 0;
	co2S->TextureList = LoadCharTextures("SONICTEX");
	co2S->MotionList = LoadMTNFile((char*)"sonicmtn.prs");
	MainCharacter[player]->DisplaySub_Delayed4 = nullptr;
	isSuper = false;

	if (IsIngame())
	{
		PlayAnimationThing(&co2S->base.AnimInfo);
		RestoreMusic();
	}
	else {
		njReleaseTexture(&Sonic_Texlist);
	}

	return;
}

bool CheckUntransform_Input(unsigned char playerID) {

	EntityData1* player = MainCharObj1[playerID];

	if (!player || AlwaysSuperSonic)
		return false;

	if (player->NextAction != 0 || player->Status & Status_DoNextAction)
	{
		return false;
	}

	if (ControllerPointers[playerID]->press & TransformButton)
	{
		if (player->Action == Action_Jump || player->Action == Action_BounceUp && (player->Status & Status_Ball) == 0) {
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
			if (player->Action == Action_Jump || player->Action == Action_BounceUp && (player->Status & Status_Ball) == 0) {
				return true;
			}
		}
	}

	return false;
}

void SuperSonic_ManagerDelete(ObjectMaster* obj)
{
	unSuper(obj->Data1.Entity->Index);
	isSuper = false;
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

	case superSonicInit:
		obj->DeleteSub = SuperSonic_ManagerDelete;
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
			LoadSuperAura(playerID);
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


	int id = MainCharObj2[playNum]->CharID;
	int id2 = MainCharObj2[playNum]->CharID2;

	if (id == Characters_Sonic && id2 == Characters_Sonic) {
		ObjectMaster* superSonicManagerPtr = LoadObject(0, "SuperSonic_Manager", SuperSonic_Manager, LoadObj_Data1);

		if (superSonicManagerPtr)
		{
			SuperSonicMdl = LoadMDLFile((char*)"SSONICMDL.PRS");
			superSonicManagerPtr->Data1.Entity->Index = playNum;
		}
	}
}

void __cdecl Sonic_HealdObjectStuff(EntityData1* data1, CharObj2Base* co2) {

	*(int*)0x25F02D8 &= 0xFFFFDBFF;
	*(int*)0x25F02D4 = *(int*)0x1DEB6A0;
	UpgradeDrawCallback = 0;
	*(int*)0x25F0268 = *(int*)0x1DEB6A8;
	sub_487060(byte_1DE4400);
	SonicHeldObjectThing(data1, co2);
	return;
}

void DrawSonicMotion(EntityData1* data1, SonicCharObj2* sonicCO2) {

	NJS_MOTION* Motion;

	NJS_TEXLIST* texlist = sonicCO2->TextureList;

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

	if (curAnim == 30)
	{
		texlist = &SSEff_Texlist;
	}

	njSetTexture(texlist);
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
	char char2 = sonicCO2->base.CharID2;

	if (!sonicCO2->TextureList)
		return;


	ObjectFunc(origin, Sonic_Display_t->Target());
	origin(obj);

	if (!isSuper || char2 != Characters_Sonic)
		return;


	//njSetMatrix(MATRIX_1A51A00, CURRENT_MATRIX);
	SonicCO2PtrExtern = sonicCO2;
	//sub_427040(MATRIX_1A51A00);

	UpgradeDrawCallback = SuperSonic_Callback_r;

	if ((data1->field_6 & 2) != 0 && !Pose2PStart_PlayerNum)
	{
		Sonic_HealdObjectStuff(data1, &sonicCO2->base);
		return;
	}

	int curAnim = sonicCO2->base.AnimInfo.Current;


	njPushMatrixEx();
	njTranslateEx(&data1->Position);
	njScale(CURRENT_MATRIX, data1->Scale.x, data1->Scale.y, data1->Scale.z);

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
		DisplaySuperSonic_Upgrade(sonicCO2);
		Sonic_HealdObjectStuff(data1, &sonicCO2->base);
		njPopMatrixEx();
		return;
	}

	DoSpinDashRotationModel();
	DrawSonicMotion(data1, sonicCO2);
	njPopMatrixEx();
}


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


void SuperSonic_PlayVictoryAnimation(EntityData1* data1, CharObj2Base* co2) {

	if (data1->Action == Action_ObjectControl)
	{
		if (TimerStopped != 0 && (co2->AnimInfo.Next == 54 || co2->AnimInfo.Current == 54)) { //Check if the level is finished
			{
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
	EntityData2* data2 = MainCharData2[obj->Data2.Character->PlayerNum];
	SonicCharObj2* sco2 = (SonicCharObj2*)obj->Data2.Character;

	if (isSuper) {
		SuperSonic_PlayVictoryAnimation(data1, co2);
		SuperSonicFly_MainManagement(data1, co2, data2);
	}

	ObjectFunc(origin, Sonic_Main_t->Target());
	origin(obj);
}

void __cdecl Sonic_runsActions_r(EntityData1* data1, EntityData2* data2, CharObj2Base* co2, SonicCharObj2* SonicCO2)
{

	SuperSonicFly_ActionsManagement(data1, co2, data2);

	auto original = reinterpret_cast<decltype(Sonic_runsActions_r)*>(Sonic_runsActions_t->Target());
	original(data1, data2, co2, SonicCO2);
}

void init_SuperSonic() {

	WriteJump((void*)0x71E460, SonicDisplayAfterImageASM);

	LoadSonic_t = new Trampoline((int)LoadSonic, (int)LoadSonic + 0x6, LoadSonic_r);
	Sonic_Display_t = new Trampoline((int)Sonic_Display, (int)Sonic_Display + 0x6, Sonic_Display_r);
	Sonic_runsActions_t = new Trampoline((int)0x719920, (int)0x719920 + 0x8, Sonic_runsActions_r);
	Sonic_Main_t = new Trampoline((int)Sonic_Main, (int)Sonic_Main + 0x6, Sonic_Main_r);
	return;
}