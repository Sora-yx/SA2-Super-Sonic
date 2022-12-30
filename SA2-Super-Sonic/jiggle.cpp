#include "pch.h"

NJS_CNK_MODEL* jigleSSMDL = nullptr;

void DeleteSSJiggle(SonicCharObj2* sonicCO2) {
	JiggleInfo* Jiggle = sonicCO2->SpineJiggle;

	if (Jiggle)
	{
		Delete_Jiggle(Jiggle);
		sonicCO2->SpineJiggle = 0;
		jigleSSMDL = nullptr;
	}

	return;
}

void Set_SSJiggle(SonicCharObj2* sonicCO2)
{
	if (!sonicCO2->SpineJiggle) {
		return;
	}

	jigleSSMDL = CharacterModels[349].Model->child->chunkmodel;
	CharacterModels[349].Model->child->chunkmodel = sonicCO2->SpineJiggle->SourceModelCopy->chunkmodel;
}

void ResetSSJiggle(SonicCharObj2* sonicCO2)
{
	if (!sonicCO2->SpineJiggle || !jigleSSMDL) {
		return;
	}

	CharacterModels[349].Model->child->chunkmodel = jigleSSMDL;
}

void __cdecl initJiggleSuperSonic(SonicCharObj2* sonicCO2) {
	if (!isJiggle)
		return;

	int modelNumber = isSuper[sonicCO2->base.PlayerNum] == true ? 21 : 349;

	JiggleInfo* jiggleMDL = LoadJiggle(CharacterModels[modelNumber].Model->child);
	sonicCO2->SpineJiggle = jiggleMDL;
	jiggleMDL->type = 18;
	sonicCO2->SpineJiggle->speed = 0.40000001;
	sonicCO2->SpineJiggle->field_8 = 0;
	sonicCO2->SpineJiggle->field_10 = 1024;
	sonicCO2->SpineJiggle->Model = 0;
	return;
}

NJS_CNK_MODEL* jigleSSHMDL = nullptr;

void Set_SSHJiggle(SonicCharObj2* sonicCO2)
{
	if (!sonicCO2->SpineJiggle) {
		return;
	}

	jigleSSHMDL = CharacterModels[376].Model->child->chunkmodel;
	CharacterModels[376].Model->child->chunkmodel = sonicCO2->SpineJiggle->SourceModelCopy->chunkmodel;
}

void ResetSSHJiggle(SonicCharObj2* sonicCO2)
{
	if (!sonicCO2->SpineJiggle || !jigleSSHMDL) {
		return;
	}

	CharacterModels[376].Model->child->chunkmodel = jigleSSHMDL;
}

void DeleteSSHJiggle(SonicCharObj2* sonicCO2) {
	JiggleInfo* Jiggle = sonicCO2->SpineJiggle;

	if (Jiggle)
	{
		Delete_Jiggle(Jiggle);
		sonicCO2->SpineJiggle = 0;
		jigleSSHMDL = nullptr;
	}

	return;
}

void __cdecl initJiggleSuperShadow(SonicCharObj2* sonicCO2) {
	if (!isJiggle)
		return;

	int modelNumber = isSuper[sonicCO2->base.PlayerNum] == true ? 86 : 376;

	JiggleInfo* jiggleMDL = LoadJiggle(CharacterModels[modelNumber].Model->child);
	sonicCO2->SpineJiggle = jiggleMDL;
	jiggleMDL->type = 19;
	sonicCO2->SpineJiggle->speed = 0.40000001;
	sonicCO2->SpineJiggle->field_8 = 0;
	sonicCO2->SpineJiggle->field_10 = 1024;
	sonicCO2->SpineJiggle->Model = 0;
	return;
}