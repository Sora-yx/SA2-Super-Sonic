#pragma once

void ReadConfig(const char* path);
extern AnimationInfo SuperSonicAnimationList_r[];

#define CURRENT_MATRIX _nj_current_matrix_ptr_

#pragma pack(push, 8)
union ModelPointers
{
	NJS_MODEL* basic;
	NJS_CNK_MODEL* chunk;
	SA2B_Model* sa2b;
};
#pragma pack(pop)


void init_SuperSonic();

extern bool isSuper;