#pragma once

void ReadConfig(const char* path);

extern Buttons TransformButton;
extern bool RemoveLimitations;
extern bool AlwaysSuperSonic;
extern int SuperMusicVersion;

extern AnimationInfo SuperSonicAnimationList_r[];

extern HelperFunctions HelperFunctionsGlobal;

#define CURRENT_MATRIX _nj_current_matrix_ptr_

#pragma pack(push, 8)
union ModelPointers
{
	NJS_MODEL* basic;
	NJS_CNK_MODEL* chunk;
	SA2B_Model* sa2b;
};
#pragma pack(pop)


struct auraStruct {
	char idk;
	char charID;
	NJS_VECTOR pos;
	NJS_OBJECT* Model;
	NJS_TEXNAME* Tex;
	int idk3[40];
};

enum SuperSonicM {
	superSonicInit,
	playerInputCheck,
	superSonicTransfo,
	superSonicTransition,
	superSonicWait,
	superSonicOnFrames,
	superSonicUntransfo,
};

enum SuperSonicAnim {
	superSonicStanding,
	superSonicFlying1,
	superSonicFlying2,
	superSonicIntro = 203,
	superSonicVictory = 204
};

enum SuperSonicsMusic {

	None,
	Sonic2,
	Sonic3,
	SonicAndKnuckles,
	SADX,
	SA2Event,
	SA2LiveAndLearn,
	Mania,
	Random
};

extern bool isSuper;

void init_SuperSonic();
void init_AuraHack();
void init_MusicHack();
void init_PhysicsHack();
void init_UpgradesHack();