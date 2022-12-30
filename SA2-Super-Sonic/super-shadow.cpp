#include "pch.h"
#include "ss.h"

NJS_TEXLIST* Shadow_Texlist = nullptr;
ModelIndex* SuperShadowMdl;
static FunctionHook<void, int> LoadShadow_t(LoadShadow);

extern std::string currentSuperMusic;

NJS_TEXNAME SSHEffTex[33];
NJS_TEXLIST SSHEff_Texlist = { arrayptrandlength(SSHEffTex) };

NJS_TEXLIST* getShadowTexlist()
{
	return Shadow_Texlist;
}

//add super shadow model to shadow model pointer
void __cdecl SetSuperShadowModels(SonicCharObj2* sco2) {
	sco2->ModelList[355].Index = SuperShadowMdl[355].Index;
	sco2->ModelList[355].Model = SuperShadowMdl[355].Model;
	return;
}

void __cdecl LoadSuperShadowCharTextures(SonicCharObj2* sco2) {
	njReleaseTexture(sco2->TextureList);
	sco2->TextureList = 0;
	sco2->TextureList = LoadCharTextures("SSHADOWTEX");

	//used for upgrade textures.
	if (AltCostume[sco2->base.PlayerNum] != 0)
		Shadow_Texlist = LoadCharTextures("SHADOW1TEX");
	else
		Shadow_Texlist = LoadCharTextures("TERIOSTEX");

	//ugly way to fix Sonic eyes texture when transforming to super, thanks to the awful GBIX Texture system.
	if (TwoPlayerMode || CurrentLevel == LevelIDs_SonicVsShadow1 || CurrentLevel == LevelIDs_SonicVsShadow2)
	{
		char OtherPlayer = sco2->base.PlayerNum == 0 ? 1 : 0;
		if (MainCharObj2[OtherPlayer] && MainCharObj2[OtherPlayer]->CharID2 == Characters_Sonic && !isSuper[OtherPlayer])
		{
			SonicCharObj2* co2SH = (SonicCharObj2*)MainCharacter[OtherPlayer]->Data2.Character;

			if (AltCostume[OtherPlayer] != 0)
				co2SH->TextureList = LoadCharTextures("SONIC1TEX");
			else
				co2SH->TextureList = LoadCharTextures("sonictex");
		}
	}

	return;
}

AnimationInfo ShadowAnimCopy[249] = { 0 };

void __cdecl TransfoSuperShadow(EntityData1* data, int playerID, SonicCharObj2* sco2) {
	if (SuperMusicVersion != None) {
		StopMusic();
		Play_SuperSonicMusic();
		ResetMusic();
	}

	sco2->base.Speed.x = 0;

	memcpy(&ShadowAnimCopy, sco2->base.AnimInfo.Animations, sizeof(ShadowAnimCopy));
	ControllerEnabled[playerID] = 0;

	if (CurrentLevel != LevelIDs_ChaoWorld)
		sco2->base.Powerups |= Powerups_Invincibility;

	SetSuperShadowModels(sco2);
	DeleteSSHJiggle(sco2);
	initJiggleSuperShadow(sco2);
	sco2->base.AnimInfo.Next = 0;
	sco2->base.AnimInfo.Animations = SuperShadowAnimationList_r;
	LoadSuperShadowCharTextures(sco2);

	PlayAnimationThing(&sco2->base.AnimInfo);
	Load_SuperShadowPhysics(data);

	if (CurrentLevel != LevelIDs_ChaoWorld)
		sco2->base.Upgrades |= Upgrades_SuperSonic;

	isSuper[playerID] = true;
}

void unSuperShadow(unsigned char player) {
	if (AlwaysSuperShadow)
		return;

	EntityData1* data = MainCharObj1[player];
	CharObj2Base* co2 = MainCharObj2[player];

	if (!data || !isSuper[player])
		return;

	SonicCharObj2* co2S = (SonicCharObj2*)MainCharacter[player]->Data2.Character;

	if (co2->CharID == Characters_Shadow)
		co2->PhysData = PhysicsArray[Characters_Shadow];

	ResetChaosControl(player);
	DeleteSSHJiggle(co2S);
	initJiggleSuperShadow(co2S);
	data->Status = 0;
	co2->Upgrades &= ~Upgrades_SuperSonic;
	co2->Powerups &= ~Powerups_Invincibility;
	co2S->base.AnimInfo.Next = 15;
	co2S->base.AnimInfo.Animations = ShadowAnimCopy;
	njReleaseTexture(co2S->TextureList);
	co2S->TextureList = 0;

	if (AltCostume[player] != 0)
		co2S->TextureList = LoadCharTextures("SHADOW1TEX");
	else
		co2S->TextureList = LoadCharTextures("TERIOSTEX");

	MainCharacter[player]->DisplaySub_Delayed4 = nullptr;
	isSuper[player] = false;

	if (IsIngame())
	{
		PlayAnimationThing(&co2S->base.AnimInfo);
		RestoreMusic();
	}
	else {
		njReleaseTexture(Shadow_Texlist);
		Shadow_Texlist = nullptr;
	}

	return;
}

void SuperShadow_ManagerDelete(ObjectMaster* obj)
{
	currentSuperMusic = "";
	char pnum = obj->Data1.Entity->Index;
	unSuperShadow(pnum);
	isSuper[pnum] = false;
	ResetChaosControl(pnum);
	ReleaseMDLFile(SuperShadowMdl);
	Delete_SSHAnim();
}

void SuperShadow_Manager(ObjectMaster* obj)
{
	EntityData1* data = obj->Data1.Entity;
	EntityData1* player = MainCharObj1[data->Index];

	if (!player)
		return;

	SonicCharObj2* shadowCO2 = (SonicCharObj2*)MainCharacter[data->Index]->Data2.Character;

	if (shadowCO2->base.CharID2 != Characters_Shadow || (player->field_2 == 3 && data->Index == 1))
	{
		DeleteObject_(obj);
		return;
	}

	//if player dies, remove transformation and reset manager action.
	if (GameState == GameStates_LoadFinished && !AlwaysSuperShadow && data->Action > playerInputCheck) {
		unSuperShadow(data->Index);
		data->Action = playerInputCheck;
		return;
	}

	if (!IsIngame() || GameMode == GameMode_Event || miniEventPtr && miniEventPtr->MainSub != nullptr) {
		return;
	}

	unsigned char playerID = data->Index;

	switch (data->Action)
	{
	case superSonicInit:
		obj->DeleteSub = SuperShadow_ManagerDelete;
		data->Action++;
		break;
	case playerInputCheck:

		if (isLastStoryBeaten()) {
			if (CheckTransform_Input(playerID, player) || AlwaysSuperShadow)
				data->Action++;
		}
		else
		{
			DeleteObject_(obj);
			return;
		}

		break;
	case superSonicTransfo:
		data->Scale.y = ControllerEnabled[playerID];
		TransfoSuperShadow(player, playerID, shadowCO2);
		data->Action++;
		break;
	case superSonicTransition:
		shadowCO2->base.AnimInfo.Next = superSonicIntro;
		data->Action++;
		break;
	case superSonicWait:

		if (++data->Timer == 100 || AlwaysSuperShadow)
		{
			Load_SSWaterTask(playerID);
			LoadSuperAura(playerID);
			ControllerEnabled[playerID] = data->Scale.y;
			DoNextAction_r(playerID, 15, 0);
			data->Action++;
			data->Timer = 0;
		}
		break;
	case superSonicOnFrames:
		if (CheckUntransform_Input(playerID)) {
			data->Action = playerInputCheck;
		}

		SubRings(playerID, data);

		break;
	case superSonicUntransfo:
		SuperSonic_DisableFly(player, &shadowCO2->base);
		unSuperShadow(playerID);

		data->Action = playerInputCheck;
		break;
	}
}

void LoadSuperShadowManager(char playNum) {
	int id2 = MainCharObj2[playNum]->CharID2;

	if (id2 == Characters_Shadow) {
		ObjectMaster* superShadowManagerPtr = LoadObject(0, "SuperShadow_Manager", SuperShadow_Manager, LoadObj_Data1);

		if (superShadowManagerPtr)
		{
			SuperShadowMdl = LoadMDLFile((char*)"sshadowmdl.prs");
			Load_NewSuperShadowAnim();
			superShadowManagerPtr->Data1.Entity->Index = playNum;
		}
	}
}

void LoadShadow_r(int playerNum) {
	LoadShadow_t.Original(playerNum);

	if (CurrentLevel != LevelIDs_FinalHazard) {
		if (MainCharObj1[playerNum] && MainCharObj1[playerNum]->field_2 != 3) {
			LoadSuperShadowManager(playerNum);
			LoadSSEff_Textures();
			LoadWaterTextures(playerNum);
			LoadSADXAuraTextures(playerNum);
		}
	}
}

void init_SuperShadow() {
	LoadShadow_t.Hook(LoadShadow_r);
	return;
}