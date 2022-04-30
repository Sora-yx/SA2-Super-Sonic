#pragma once

void ReadConfig(const char* path);

extern Buttons TransformButton;
extern Buttons FlightButton;
extern Buttons boostBtn;
extern bool RemoveLimitations;
extern bool AlwaysSuperSonic;
extern int SuperMusicVersion;
extern int superAuraState;
extern bool isUpgrade;
extern bool isPhysics;
extern bool isFlyAllowed;
extern bool unTransform;
extern bool AlwaysSuperShadow;
extern bool AllowSuperAttacks;
extern bool isJiggle;
extern bool SADXAura;
extern bool isBoostAllowed;
extern AnimationInfo SuperSonicAnimationList_r[];
extern AnimationInfo SuperShadowAnimationList_r[];

extern HelperFunctions HelperFunctionsGlobal;

extern HMODULE SA2Anim;
extern HMODULE SA1Char;

extern NJS_TEXLIST SSEff_Texlist;
extern NJS_TEXLIST SSHEff_Texlist;

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

extern bool isSuper[2];

void init_SuperSonic();
void init_AuraHack();
void init_MusicHack();
void init_PhysicsHack();
NJS_TEXLIST* getSonicTexlist();
NJS_TEXLIST* getShadowTexlist();
void init_SuperShadow();
void init_WaterHack();
bool isBoosting(char pnum);