#pragma once

void ReadConfig(const char* path);

extern Buttons TransformButton;
extern bool RemoveLimitations;
extern bool AlwaysSuperSonic;


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
	superSonicSetTask,
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
	superSonicIntro = 203
};


void init_SuperSonic();

extern bool isSuper;