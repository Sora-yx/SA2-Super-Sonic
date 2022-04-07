#pragma once

void ReadConfig(const char* path);

extern Buttons TransformButton;
extern Buttons FlightButton;
extern bool RemoveLimitations;
extern bool AlwaysSuperSonic;
extern int SuperMusicVersion;
extern int superAuraState;
extern bool isUpgrade;
extern bool isPhysics;
extern bool isFlyAllowed;
extern bool unTransform;
extern bool AlwaysSuperShadow;
extern AnimationInfo SuperSonicAnimationList_r[];
extern AnimationInfo SuperShadowAnimationList_r[];

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

	superSonicIntro = 203,
	superSonicVictory = 204,
	superSonicFlying1,
	superSonicFlying2,
	ssBeginAscent,
	ssAscending,
	ssBeginDescent,
	ssDescend,
	ssBeginDash,
	ssDash,
	ssBeginDash2,
	ssDash2
};

enum SuperSonicMusic {

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

enum SuperShadowcAnim {
	superShadowStanding,

	superShadowIntro = 249,
	superShadowVictory = 250,
	superShadowFlying1,
	superShadowFlying2,
	ssHBeginAscent,
	ssHAscending,
	ssHBeginDescent,
	ssHDescend,
	ssHBeginDash,
	ssHDash,
	ssHBeginDash2,
	ssHDash2
};

extern bool isSuper[2];

void init_SuperSonic();
void init_AuraHack();
void init_MusicHack();
void init_PhysicsHack();
NJS_TEXLIST* getSonicTexlist();
NJS_TEXLIST* getShadowTexlist();
void init_SuperShadow();