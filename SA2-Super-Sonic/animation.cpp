#include "pch.h"

AnimationIndex* SuperSonicNewAnimList;

AnimationInfo SuperSonicAnimationList_r[] = {
	{ 203, 328, 3, 0, 0.0625f, 0.15f },
	{ 1, 328, 3, 1, 0.25f, 0.1f },
	{ 2, 328, 3, 2, 0.25f, 0.1f },
	{ 3, 328, 4, 0, 0.25f, 1 },
	{ 4, 328, 3, 4, 0.03125f, 0.1f },
	{ 5, 328, 3, 5, 0.03125f, 0.1f },
	{ 6, 328, 11, 6, 0.25f, 3.4f },
	{ 7, 328, 11, 7, 0.25f, 1.4f },
	{ 8, 328, 11, 8, 0.25f, 0.8f },
	{ 204, 328, 11, 9, 0.25f, 0.6f },
	{ 204, 328, 10, 10, 0.5f, 0.4f },
	{ 11, 328, 11, 11, 0.25f, 0.2f },
	{ 12, 328, 10, 12, 0.25f, 0.7f },
	{ 13, 328, 3, 13, 0.25f, 0.4f },
	{ 14, 328, 4, 15, 0.125f, 0.3f },
	{ 15, 328, 3, 15, 0.125f, 0.4f },
	{ 16, 328, 4, 0, 1, 0.2f },
	{ 17, 328, 4, 0, 1, 0.4f },
	{ 18, 328, 9, 0, 0.25f, 0.3f },
	{ 19, 328, 4, 20, 0.125f, 0.2f },
	{ 20, 328, 3, 20, 0.25f, 0.6f },
	{ 21, 328, 6, 21, 1, 0.1f },
	{ 22, 328, 4, 0, 1, 1 },
	{ 23, 328, 3, 23, 0.0625f, 1 },
	{ 24, 328, 4, 25, 0.5f, 0.4f },
	{ 25, 328, 4, 0, 0.0625f, 0.3f },
	{ 26, 328, 4, 1, 0.0625f, 1 },
	{ 27, 328, 4, 0, 1, 0.5f },
	{ 28, 328, 3, 28, 0.0625f, 0.2f },
	{ 29, 328, 3, 29, 0.0625f, 0.2f },
	{ 30, 6, 3, 30, 0.5f, 1 },
	{ 31, 328, 9, 35, 0.125f, 0.2f },
	{ 32, 328, 11, 32, 0.5f, 0.4f },
	{ 33, 328, 6, 33, 0.25f, 0.2f },
	{ 34, 328, 4, 35, 0.5f, 1 },
	{ 35, 328, 3, 35, 0.125f, 0.1f },
	{ 36, 328, 3, 36, 0.03125f, 0.1f },
	{ 37, 328, 3, 37, 0.03125f, 0.1f },
	{ 38, 328, 11, 38, 0.5f, 0.5f },
	{ 39, 328, 4, 6, 0.5f, 0.5f },
	{ 40, 328, 3, 40, 0.5f, 0.6f },
	{ 41, 328, 3, 41, 0.5f, 0.4f },
	{ 42, 328, 3, 42, 0.125f, 0.3f },
	{ 43, 328, 4, 44, 0.5f, 0.2f },
	{ 44, 328, 3, 44, 0.5f, 0.1f },
	{ 45, 328, 3, 45, 0.5f, 0.1f },
	{ 46, 328, 4, 47, 0.5f, 0.1f },
	{ 47, 328, 3, 47, 0.5f, 0.3f },
	{ 48, 328, 4, 35, 1, 0.04f },
	{ 49, 328, 4, 0, 0.5f, 0.3f },
	{ 50, 328, 4, 0, 0.5f, 0.3f },
	{ 51, 328, 4, 15, 0.5f, 0.3f },
	{ 52, 328, 4, 15, 0.5f, 0.3f },
	{ 53, 328, 7, 0, 1, 0.2f },
	{ 54, 328, 6, 54, 1, 0.5f },
	{ 55, 328, 3, 55, 0.125f, 0.3f },
	{ 56, 328, 4, 15, 1, 0.25f },
	{ 57, 328, 4, 58, 0.125f, 0.1f },
	{ 58, 328, 3, 58, 1, 0.1f },
	{ 59, 328, 6, 59, 1, 0.2f },
	{ 60, 328, 6, 60, 0.25f, 1 },
	{ 61, 328, 3, 61, 0.25f, 0.5f },
	{ 62, 328, 6, 62, 0.25f, 0.1f },
	{ 63, 328, 4, 0, 0.25f, 0.3f },
	{ 64, 328, 4, 0, 0.25f, 0.3f },
	{ 65, 328, 4, 66, 0.25f, 1.4f },
	{ 66, 328, 3, 66, 0.25f, 1 },
	{ 67, 328, 4, 68, 0.125f, 0.4f },
	{ 68, 328, 3, 68, 0.125f, 0.4f },
	{ 69, 328, 4, 70, 0.25f, 0.1f },
	{ 70, 328, 3, 70, 1, 0.2f },
	{ 71, 328, 9, 0, 0.125f, 0.2f },
	{ 72, 328, 6, 72, 0.25f, 0.2f },
	{ 73, 328, 4, 0, 0.25f, 0.3f },
	{ 74, 328, 3, 74, 0.25f, 0.4f },
	{ 75, 328, 3, 75, 0.25f, 0.1f },
	{ 76, 328, 6, 76, 0.25f, 0.3f },
	{ 77, 328, 3, 77, 0.25f, 0.8f },
	{ 78, 328, 3, 78, 0.125f, 0.2f },
	{ 79, 328, 9, 35, 0.25f, 0.5f },
	{ 80, 328, 5, 81, 0.25f, 0.5f },
	{ 81, 328, 4, 0, 0.25f, 0.5f },
	{ 82, 328, 9, 0, 0.25f, 0.5f },
	{ 83, 328, 4, 0, 0.0625f, 0.15f },
	{ 84, 328, 4, 0, 0.0625f, 0.15f },
	{ 85, 328, 6, 85, 0.25f, 0.5f },
	{ 86, 328, 3, 86, 1, 0.2f },
	{ 87, 328, 3, 87, 1, 0.5f },
	{ 88, 328, 3, 88, 0.03125f, 0.3f },
	{ 89, 328, 3, 89, 0.03125f, 0.1f },
	{ 90, 328, 9, 0, 0.03125f, 0.25f },
	{ 91, 328, 4, 92, 0.25f, 1 },
	{ 92, 328, 4, 93, 0.25f, 1 },
	{ 93, 328, 4, 16, 0.25f, 0.2f },
	{ 94, 328, 9, 0, 0.25f, 0.3f },
	{ 95, 328, 9, 0, 0.25f, 0.3f },
	{ 96, 328, 4, 97, 0.25f, 1 },
	{ 97, 328, 4, 98, 0.25f, 1 },
	{ 98, 328, 4, 99, 0.25f, 0.2f },
	{ 99, 328, 4, 16, 0.25f, 1 },
	{ 66, 328, 3, 100, 1, 3 },
	{ 101, 328, 6, 67, 0.25f, 0.3f },
	{ 102, 328, 6, 67, 0.25f, 0.3f },
	{ 103, 328, 6, 67, 0.25f, 0.3f },
	{ 104, 328, 13, 104, 0.25f, 0.3f },
	{ 105, 328, 13, 105, 0.25f, 0.3f },
	{ 106, 328, 13, 106, 0.25f, 0.3f },
	{ 107, 328, 13, 107, 0.25f, 0.3f },
	{ 108, 328, 13, 108, 0.25f, 0.3f },
	{ 109, 328, 13, 109, 0.25f, 0.3f },
	{ 110, 328, 13, 110, 0.25f, 0.3f },
	{ 111, 328, 13, 111, 0.25f, 0.3f },
	{ 112, 328, 3, 112, 0.25f, 0.3f },
	{ 113, 328, 3, 113, 0.25f, 0.3f },
	{ 114, 328, 9, 15, 0.0625f, 0.2f },
	{ 115, 328, 4, 15, 0.0625f, 0.35f },
	{ 116, 328, 4, 15, 0.0625f, 0.35f },
	{ 117, 328, 4, 15, 0.125f, 1 },
	{ 118, 328, 6, 118, 1, 1 },
	{ 119, 328, 6, 119, 1, 1 },
	{ 120, 328, 3, 120, 1, 0.1f },
	{ 121, 328, 13, 121, 0.25f, 0.3f },
	{ 122, 328, 13, 122, 0.25f, 0.3f },
	{ 123, 328, 13, 123, 0.25f, 0.3f },
	{ 124, 328, 13, 124, 0.25f, 0.3f },
	{ 125, 328, 3, 125, 0.25f, 0.3f },
	{ 126, 328, 3, 126, 0.25f, 0.3f },
	{ 127, 328, 6, 127, 0.25f, 0.4f },
	{ 128, 328, 4, 129, 0.125f, 0.3f },
	{ 129, 328, 3, 129, 0.125f, 0.4f },
	{ 130, 328, 4, 121, 1, 0.3f },
	{ 132, 328, 4, 121, 0.25f, 0.3f },
	{ 131, 328, 4, 122, 0.25f, 0.3f },
	{ 133, 328, 4, 130, 1, 1 },
	{ 134, 328, 4, 130, 1, 1 },
	{ 135, 328, 4, 130, 0.0625f, 1.25f },
	{ 136, 328, 4, 130, 0.0625f, 1 },
	{ 137, 328, 4, 130, 1, 1 },
	{ 138, 328, 4, 130, 0.0625f, 1.25f },
	{ 139, 328, 4, 130, 1, 1 },
	{ 140, 328, 4, 130, 1, 1 },
	{ 141, 328, 4, 130, 0.0625f, 1.25f },
	{ 142, 328, 4, 130, 0.0625f, 1.25f },
	{ 143, 328, 4, 130, 1, 1 },
	{ 144, 328, 4, 130, 0.0625f, 1 },
	{ 145, 328, 13, 121, 0.25f, 0.3f },
	{ 146, 328, 13, 122, 0.25f, 0.3f },
	{ 147, 328, 13, 123, 0.25f, 0.3f },
	{ 148, 328, 13, 124, 0.25f, 0.3f },
	{ 149, 328, 3, 125, 0.25f, 0.3f },
	{ 150, 328, 3, 126, 0.25f, 0.3f },
	{ 151, 328, 6, 127, 0.25f, 0.4f },
	{ 152, 328, 4, 129, 0.125f, 0.3f },
	{ 153, 328, 3, 129, 0.125f, 0.4f },
	{ 154, 328, 4, 121, 0.125f, 0.4f },
	{ 156, 328, 4, 121, 0.25f, 0.3f },
	{ 155, 328, 4, 122, 0.25f, 0.3f },
	{ 157, 328, 9, 129, 0.0625f, 1 },
	{ 158, 328, 9, 129, 0.0625f, 1 },
	{ 159, 328, 9, 129, 0.0625f, 1 },
	{ 160, 328, 9, 129, 0.0625f, 1 },
	{ 161, 328, 9, 129, 0.0625f, 1 },
	{ 162, 328, 9, 129, 0.0625f, 1 },
	{ 163, 328, 9, 129, 0.0625f, 1 },
	{ 164, 328, 9, 129, 0.0625f, 1 },
	{ 165, 328, 9, 129, 0.0625f, 1 },
	{ 166, 328, 9, 129, 0.0625f, 1 },
	{ 167, 328, 9, 129, 0.0625f, 1 },
	{ 168, 328, 9, 129, 0.0625f, 1 },
	{ 169, 328, 3, 169, 0.0625f, 0.1f },
	{ 170, 328, 3, 170, 0.25f, 0.1f },
	{ 171, 328, 11, 171, 0.25f, 1.4f },
	{ 172, 328, 11, 172, 0.25f, 1.4f },
	{ 173, 328, 11, 173, 0.25f, 1.4f },
	{ 174, 328, 11, 174, 0.25f, 1.4f },
	{ 175, 328, 4, 169, 0.25f, 1.4f },
	{ 176, 328, 3, 176, 0.03125f, 0.1f },
	{ 177, 328, 3, 177, 0.03125f, 0.1f },
	{ 178, 328, 9, 0, 0.03125f, 0.25f },
	{ 179, 328, 4, 169, 0.25f, 1.4f },
	{ 180, 328, 6, 180, 0.25f, 1.4f },
	{ 181, 328, 6, 181, 0.25f, 1.4f },
	{ 182, 328, 6, 182, 1, 1 },
	{ 183, 328, 6, 183, 1, 1 },
	{ 184, 328, 3, 184, 1, 0.5f },
	{ 185, 328, 4, 186, 0.25f, 0.3f },
	{ 186, 328, 3, 186, 0.25f, 0.5f },
	{ 187, 328, 3, 187, 0.25f, 0.8f },
	{ 188, 328, 6, 188, 0.25f, 0.3f },
	{ 189, 328, 4, 189, 0.25f, 0.2f },
	{ 190, 328, 3, 190, 0.25f, 0.3f },
	{ 191, 328, 3, 191, 0.25f, 0.2f },
	{ 192, 328, 11, 192, 0.25f, 0.2f },
	{ 193, 328, 11, 192, 0.25f, 0.2f },
	{ 194, 328, 3, 194, 0.0625f, 0.2f },
	{ 195, 328, 3, 195, 0.0625f, 0.2f },
	{ 196, 328, 3, 196, 0.0625f, 0.2f },
	{ 197, 328, 3, 197, 0.0625f, 0.2f },
	{ 198, 328, 3, 198, 0.125f, 0.2f },
	{ 199, 328, 3, 199, 0.25f, 0.2f },
	{ 200, 328, 3, 200, 0.25f, 0.2f },
	{ 201, 328, 6, 201, 0.125f, 0.8f },
	{ 202, 328, 3, 202, 0.125f, 0.5f },
	{ 205, 328, 6, 0, 1, 0.8f }, //turning to super
	{ 205, 328, 4, 54, 1, 0.5f }, //victory pose
	{ 204, 328, 3, superSonicFlying1, 0.0625f, 0.5f }, //fly 1
	{ 206, 328, 3, superSonicFlying2, 0.0625f, 0.5f }, //fly 2
	{ 207, 328, 4, ssAscending, 0.125f, 0.5f }, //begin ascent
	{ 208, 328, 3, ssAscending, 0.125f, 0.5f }, //ascending
	{ 209, 328, 4, ssDescend, 0.125f, 0.5f }, //begin descent
	{ 210, 328, 3, ssDescend, 0.125f, 0.5f }, //descending
	{ 211, 328, 4, ssDash, 0.0625f, 0.5f }, //begin dash
	{ 212, 328, 3, ssDash, 0.0625f, 0.5f }, //dash
	{ 213, 328, 4, ssDash2, 0.0625f, 0.5f }, //begin dash 2
	{ 214, 328, 3, ssDash2, 0.0625f, 0.5f }, //dash2
};

void Delete_SSAnim() {
	AnimationIndex* copy = SuperSonicNewAnimList;
	UnloadAnimation(copy);

	SuperSonicNewAnimList = 0;
	return;
}

void Load_NewSuperSonicAnim() {

	if (isSA1Char(Characters_SuperSonic))
	{
		SuperSonicAnimationList_r[0].AnimationSpeed = 0.25f;
		SuperSonicAnimationList_r[0].TransitionSpeed = 0.0625f;
		SuperSonicAnimationList_r[204] = { 54, 0, 6, 54, 0.5f, 1.0f };
	}
	else if (SonkTrick)
	{
		SuperSonicAnimationList_r[67] = SuperSonicAnimationList_r[66];
		SuperSonicAnimationList_r[67].NextAnimation = 68;
		SuperSonicAnimationList_r[68] = SuperSonicAnimationList_r[66];
		SuperSonicAnimationList_r[68].NextAnimation = 68;
	}

	SuperSonicNewAnimList = LoadMTNFile((char*)"ssmotion.prs");
}