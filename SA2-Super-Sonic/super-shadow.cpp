#include "pch.h"
#include "ss.h"

NJS_TEXLIST* Shadow_Texlist = nullptr;
ModelIndex* SuperShadowMdl;
Trampoline* LoadShadow_t = nullptr;
extern NJS_TEXLIST SSEff_Texlist;

extern std::string currentSuperMusic;

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

	return;
}

AnimationInfo ShadowAnimCopy[249];

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
	sco2->base.AnimInfo.Animations = SuperSonicAnimationList_r;
	LoadSuperShadowCharTextures(sco2);

	PlayAnimationThing(&sco2->base.AnimInfo);
	Load_SuperPhysics(data);

	if (CurrentLevel != LevelIDs_ChaoWorld)
		sco2->base.Upgrades |= Upgrades_SuperSonic;

	isSuper[playerID] = true;
}


void unSuperShadow(unsigned char player) {

	if (AlwaysSuperShadow)
		return;

	EntityData1* data = MainCharObj1[player];
	CharObj2Base* co2 = MainCharObj2[player];
	SonicCharObj2* co2S = (SonicCharObj2*)MainCharacter[player]->Data2.Character;

	if (!data || !isSuper[player])
		return;

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
	Delete_SSAnim();
}

void SuperShadow_Manager(ObjectMaster* obj)
{
	EntityData1* data = obj->Data1.Entity;
	EntityData1* player = MainCharObj1[data->Index];
	SonicCharObj2* shadowCO2 = (SonicCharObj2*)MainCharacter[data->Index]->Data2.Character;


	if (shadowCO2->base.CharID2 != Characters_Shadow)
	{
		DeleteObject_(obj);
		return;
	}

	//if player dies, remove transformation and reset manager action.
	if (GameState == GameStates_LoadFinished && !AlwaysSuperSonic && data->Action > playerInputCheck) {

		unSuperShadow(data->Index);
		data->Action = playerInputCheck;
		return;
	}

	if (!player || !IsIngame() || GameMode == GameMode_Event) {
		return;
	}

	if ((CurrentLevel == LevelIDs_SonicVsShadow1 || CurrentLevel == LevelIDs_SonicVsShadow2) && data->Index == 1 && AlwaysSuperSonic)
	{
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

		if (CheckTransform_Input(playerID, player) || AlwaysSuperSonic)
			data->Action++;

		break;
	case superSonicTransfo:
		TransfoSuperShadow(player, playerID, shadowCO2);
		data->Action++;
		break;
	case superSonicTransition:
		shadowCO2->base.AnimInfo.Next = superSonicIntro;
		data->Action++;
		break;
	case superSonicWait:

		if (++data->Timer == 100 || AlwaysSuperSonic)
		{
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
			SuperShadowMdl = LoadMDLFile((char*)"SSONICMDL.PRS");
			Load_NewSuperShadowAnim();
			superShadowManagerPtr->Data1.Entity->Index = playNum;
		}
	}
}

void LoadShadow_r(int playerNum) {

	auto original = reinterpret_cast<decltype(LoadShadow_r)*>(LoadShadow_t->Target());
	original(playerNum);

	if (CurrentLevel != LevelIDs_FinalHazard) {
		LoadSuperShadowManager(playerNum);

		if (playerNum > 0)
			LoadSSEff_Textures();
	}
}

void init_SuperShadow() {

	LoadShadow_t = new Trampoline((int)LoadShadow, (int)LoadShadow + 0x6, LoadShadow_r);
	return;
}
