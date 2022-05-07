#include "pch.h"
#include "ss.h"

Trampoline* Sonic_Main_t;
Trampoline* Sonic_Display_t;
Trampoline* LoadSonic_t;
Trampoline* Sonic_runsActions_t;

bool isSuper[2] = { false, false };

NJS_TEXNAME SSEffTex[33];
NJS_TEXLIST SSEff_Texlist = { arrayptrandlength(SSEffTex) };

NJS_TEXLIST* Sonic_Texlist = nullptr;

ModelIndex* SuperSonicMdl = nullptr;
extern std::string currentSuperMusic;
extern NJS_TEXLIST SSHEff_Texlist;

NJS_TEXLIST* getSonicTexlist()
{
	return Sonic_Texlist;
}

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

	//used for upgrade textures.
	if (AltCostume[sco2->base.PlayerNum] != 0)
		Sonic_Texlist = LoadCharTextures("SONIC1TEX");
	else
		Sonic_Texlist = LoadCharTextures("SONICTEX");

	//ugly way to fix Shadow eyes texture when transforming to super, thanks to the awful GBIX Texture system.
	if (TwoPlayerMode || CurrentLevel == LevelIDs_SonicVsShadow1 || CurrentLevel == LevelIDs_SonicVsShadow2)
	{
		char OtherPlayer = sco2->base.PlayerNum == 0 ? 1 : 0;
		if (MainCharObj2[OtherPlayer] && MainCharObj2[OtherPlayer]->CharID2 == Characters_Shadow)
		{
			SonicCharObj2* co2SH = (SonicCharObj2*)MainCharacter[OtherPlayer]->Data2.Character;

			if (AltCostume[OtherPlayer] != 0)
				co2SH->TextureList = LoadCharTextures("SHADOW1TEX");
			else
				co2SH->TextureList = LoadCharTextures("teriostex");
		}
	}

	return;
}

void __cdecl LoadSSEff_Textures() {
	LoadTextureList("ss_efftex", &SSEff_Texlist);
	LoadTextureList("SSONEFFTEX", &SSONEFFTEX_TEXLIST);
	LoadTextureList("sh_efftex", &Texlist_ShadEff);
	LoadTextureList("ssh_efftex", &SSHEff_Texlist);
	return;
}

AnimationInfo SonicAnimCopy[203];

void __cdecl TransfoSuperSonic(EntityData1* data, int playerID, SonicCharObj2* sco2) {

	if (SuperMusicVersion != None) {
		StopMusic();
		Play_SuperSonicMusic();
		ResetMusic();
	}

	sco2->base.Speed.x = 0;

	memcpy(&SonicAnimCopy, sco2->base.AnimInfo.Animations, sizeof(SonicAnimCopy));
	ControllerEnabled[playerID] = 0;

	if (CurrentLevel != LevelIDs_ChaoWorld)
		sco2->base.Powerups |= Powerups_Invincibility;

	SetSuperSonicModels(sco2);
	DeleteSSJiggle(sco2);
	initJiggleSuperSonic(sco2);
	sco2->base.AnimInfo.Next = 0;
	sco2->base.AnimInfo.Animations = SuperSonicAnimationList_r;
	LoadSuperSonicCharTextures(sco2);

	PlayAnimationThing(&sco2->base.AnimInfo);
	Load_SuperPhysics(data);

	if (CurrentLevel != LevelIDs_ChaoWorld)
		sco2->base.Upgrades |= Upgrades_SuperSonic;

	isSuper[playerID] = true;
}

void SubRings(unsigned char player, EntityData1* data) {

	if (RemoveLimitations || AlwaysSuperSonic || MainCharObj2[player]->CharID != Characters_Sonic || !isSuper[player] || TimerStopped != 0)
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

	if (!data || !isSuper[player])
		return;

	if (co2->CharID == Characters_Sonic)
		co2->PhysData = PhysicsArray[Characters_Sonic];

	ResetChaosControl(player);
	ResetWindCutter(player);
	DeleteSSJiggle(co2S);
	initJiggleSuperSonic(co2S);
	data->Status = 0;
	co2->Upgrades &= ~Upgrades_SuperSonic;
	co2->Powerups &= ~Powerups_Invincibility;
	co2S->base.AnimInfo.Next = 15;
	co2S->base.AnimInfo.Animations = SonicAnimCopy;
	njReleaseTexture(co2S->TextureList);
	co2S->TextureList = 0;

	if (AltCostume[player] != 0)
		co2S->TextureList = LoadCharTextures("SONIC1TEX");
	else
		co2S->TextureList = LoadCharTextures("SONICTEX");


	MainCharacter[player]->DisplaySub_Delayed4 = nullptr;
	isSuper[player] = false;

	if (IsIngame())
	{
		PlayAnimationThing(&co2S->base.AnimInfo);
		RestoreMusic();
	}
	else {
		njReleaseTexture(Sonic_Texlist);
		Sonic_Texlist = nullptr;
		FreeWaterMDL();
	}

	return;
}

bool CheckUntransform_Input(unsigned char playerID) {

	EntityData1* player = MainCharObj1[playerID];

	if (!player || AlwaysSuperSonic && MainCharObj2[playerID]->CharID2 == Characters_Sonic
		|| AlwaysSuperShadow && MainCharObj2[playerID]->CharID2 == Characters_Shadow || !unTransform)
		return false;

	if (player->NextAction != 0 || player->Status & Status_DoNextAction)
	{
		return false;
	}

	if (ControllerPointers[playerID]->on & TransformButton)
	{
		if (player->Action == Action_HomingAttack) {
			player->Status &= ~Status_Ball;

			if (MainCharObj2[playerID]->CharID2 == Characters_Sonic)
				unSuper(playerID);
			else
				unSuperShadow(playerID);

			return true;
		}
	}

	return false;
}

bool CheckTransform_Input(char playerID, EntityData1* player)
{
	if (isSuper[playerID])
		return false;

	if (RingCount[playerID] >= 50 || RemoveLimitations) {

		if (player->Action == Action_Jump || player->Action == Action_BounceUp && (player->Status & Status_Ball) == 0) {

			if (Controllers[playerID].press & TransformButton)
			{
				player->Action = 18;
				player->Status &= ~Status_Ball;
				return true;
			}
		}
	}

	return false;
}

void SuperSonic_ManagerDelete(ObjectMaster* obj)
{
	currentSuperMusic = "";
	char pnum = obj->Data1.Entity->Index;
	unSuper(pnum);
	isSuper[pnum] = false;
	ResetChaosControl(pnum);
	ResetWindCutter(pnum);
	ReleaseMDLFile(SuperSonicMdl);
	Delete_SSAnim();
}


void SuperSonic_Manager(ObjectMaster* obj)
{
	EntityData1* data = obj->Data1.Entity;
	EntityData1* player = MainCharObj1[data->Index];

	if (!player)
		return;

	SonicCharObj2* sonicCO2 = (SonicCharObj2*)MainCharacter[data->Index]->Data2.Character;

	if (sonicCO2->base.CharID2 != Characters_Sonic || (player->field_2 == 3 && data->Index == 1))
	{
		DeleteObject_(obj);
		return;
	}

	//if player dies, remove transformation and reset manager action.
	if (GameState == GameStates_LoadFinished && !AlwaysSuperSonic && data->Action > playerInputCheck) {

		unSuper(data->Index);
		data->Action = playerInputCheck;
		return;
	}

	if (!player || !IsIngame() || GameMode == GameMode_Event || miniEventPtr && miniEventPtr->MainSub != nullptr) {
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

		if (isLastStoryBeaten()) {
			if (CheckTransform_Input(playerID, player) || AlwaysSuperSonic)
				data->Action++;
		}
		else
		{
			DeleteObject_(obj);
			return;
		}

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

		if (++data->Timer == 100 || AlwaysSuperSonic)
		{
			Load_SSWaterTask(playerID);
			LoadSuperAura(playerID);
			ControllerEnabled[playerID] = 1;
			DoNextAction_r(playerID, 15, 0);
			data->Action++;
			data->Timer = 0;
		}
		break;
	case superSonicOnFrames:
		if (CheckUntransform_Input(playerID)) {

			data->Action = playerInputCheck;
		}

		Create_SS_WaterTask(sonicCO2->base.CharID2, playerID);
		SubRings(playerID, data);

		break;
	case superSonicUntransfo:
		SuperSonic_DisableFly(player, &sonicCO2->base);
		unSuper(playerID);

		data->Action = playerInputCheck;
		break;
	}
}

void LoadSuperSonicManager(char playNum) {

	int id2 = MainCharObj2[playNum]->CharID2;

	if (id2 == Characters_Sonic) {
		ObjectMaster* superSonicManagerPtr = LoadObject(0, "SuperSonic_Manager", SuperSonic_Manager, LoadObj_Data1);

		if (superSonicManagerPtr)
		{
			SuperSonicMdl = LoadMDLFile((char*)"SSONICMDL.PRS");
			Load_NewSuperSonicAnim();
			LoadWaterMDL();
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

	if (!texlist)
		return;

	bool isSonic = sonicCO2->base.CharID2 == Characters_Sonic;

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

		AnimationIndex* CharAnim = getCharAnim_r();

		if (!CharAnim)
			Motion = CharacterAnimations[sonicCO2->base.AnimInfo.Animations[curAnim].AnimNum].Animation;
		else
			Motion = CharAnim[sonicCO2->base.AnimInfo.Animations[curAnim].AnimNum].Animation;
	}

	if (curAnim == 30)
	{
		if (isSonic && !isSA1Char(Characters_SuperSonic))
			texlist = &SSEff_Texlist;
		else if (!isSonic && !isSA1Char(Characters_SuperShadow))
			texlist = &SSHEff_Texlist;
	}

	if (AltCostume[sonicCO2->base.PlayerNum] == 0)
		njSetTexture(texlist);

	njCnkMotion(SonicModel, Motion, sonicCO2->base.AnimInfo.field_10); //Draw Sonic animated

	if (isSonic)
		ResetSSJiggle(sonicCO2);
	else
		ResetSSHJiggle(sonicCO2);
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

	if (!sonicCO2->TextureList || !sonicCO2->MotionList)
		return;

	if (!isSuper[pID] || char2 != Characters_Sonic && char2 != Characters_Shadow) {

		ObjectFunc(origin, Sonic_Display_t->Target());
		return origin(obj);
	}

	bool isShadow = char2 == Characters_Shadow;

	//used to calc matrix for upgrades, pick/drop object and aura position.
	memcpy(MATRIX_1A51A00, CURRENT_MATRIX, 0x30u);
	SonicCO2PtrExtern = sonicCO2;
	sub_427040(MATRIX_1A51A00, CURRENT_MATRIX);

	if (isShadow)
		UpgradeDrawCallback = SuperShadow_Callback_r;
	else
		UpgradeDrawCallback = SuperSonic_Callback_r;

	if ((data1->Timer & 2) != 0 && !Pose2PStart_PlayerNum)
	{
		Sonic_HealdObjectStuff(data1, &sonicCO2->base);
		return;
	}

	int curAnim = sonicCO2->base.AnimInfo.Current;

	njSetTexture(sonicCO2->TextureList);
	njPushMatrixEx();
	njTranslateEx(&data1->Position);
	njScale(CURRENT_MATRIX, data1->Scale.x, data1->Scale.y, data1->Scale.z);

	if (isShadow)
		Set_SSHJiggle(sonicCO2);
	else
		Set_SSJiggle(sonicCO2);

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

		if (isShadow)
			DisplaySuperShadow_Upgrade(data1, sonicCO2);
		else
			DisplaySuperSonic_Upgrade(data1, sonicCO2);

		Sonic_HealdObjectStuff(data1, &sonicCO2->base);
		njPopMatrixEx();
		return;
	}

	DoSpinDashRotationModel();
	DrawSonicMotion(data1, sonicCO2);
	njPopMatrixEx();
}


void LoadSonic_r(int playerNum) {

	auto original = reinterpret_cast<decltype(LoadSonic_r)*>(LoadSonic_t->Target());
	original(playerNum);

	if (CurrentLevel != LevelIDs_FinalHazard) {
		if (MainCharObj1[playerNum] && MainCharObj1[playerNum]->field_2 != 3) {

			LoadSuperSonicManager(playerNum);
			LoadSSEff_Textures();
			LoadWaterTextures(playerNum);
			LoadSADXAuraTextures(playerNum);
		}
	}
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

void SuperSonic_RunCustomAction(EntityData1* data1, SonicCharObj2* SonicCO2, CharObj2Base* co2)
{
	SuperSonicFly_ActionsManagement(data1, SonicCO2, co2);

	if (AllowSuperAttacks) {
		ChaosControl_Management(co2);
		Check_SonicWind(co2);
	}
}

void Sonic_Main_r(ObjectMaster* obj)
{
	CharObj2Base* co2 = MainCharObj2[obj->Data2.Character->PlayerNum];
	EntityData1* data1 = MainCharObj1[obj->Data2.Character->PlayerNum];
	EntityData2* data2 = MainCharData2[obj->Data2.Character->PlayerNum];
	SonicCharObj2* sco2 = (SonicCharObj2*)obj->Data2.Character;

	if (isSuper[co2->PlayerNum]) {

		SuperSonic_PlayVictoryAnimation(data1, co2);
		SuperSonicFly_MainManagement(data1, co2, data2);
	}

	ObjectFunc(origin, Sonic_Main_t->Target());
	origin(obj);
}

void __cdecl Sonic_runsActions_r(EntityData1* data1, EntityData2* data2, CharObj2Base* co2, SonicCharObj2* SonicCO2)
{
	if (isSuper[co2->PlayerNum]) {
		SuperSonic_RunCustomAction(data1, SonicCO2, co2);
	}


	auto original = reinterpret_cast<decltype(Sonic_runsActions_r)*>(Sonic_runsActions_t->Target());
	original(data1, data2, co2, SonicCO2);
}


void init_SuperSonic() {

	LoadSonic_t = new Trampoline((int)LoadSonic, (int)LoadSonic + 0x6, LoadSonic_r);
	Sonic_Display_t = new Trampoline((int)Sonic_Display, (int)Sonic_Display + 0x6, Sonic_Display_r);
	Sonic_runsActions_t = new Trampoline((int)0x719920, (int)0x719920 + 0x8, Sonic_runsActions_r);
	Sonic_Main_t = new Trampoline((int)Sonic_Main, (int)Sonic_Main + 0x6, Sonic_Main_r);
	init_AfterImages();
	initChaosControl_Hack();
	init_SonicWindHack();

	return;
}
