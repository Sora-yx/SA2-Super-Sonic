#include "pch.h"

NJS_CNK_MODEL* jigleSSMDL = nullptr;

void DeleteJiggle(SonicCharObj2* sonicCO2) {
	JiggleInfo* Jiggle = sonicCO2->SpineJiggle;

	if (Jiggle)
	{
		Delete_Jiggle(Jiggle);
		sonicCO2->SpineJiggle = 0;
	}

	return;
}

void __cdecl initJiggleSuperSonic(SonicCharObj2* sonicCO2) {

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

void SetJiggle(SonicCharObj2* sonicCO2)
{
	if (!sonicCO2->SpineJiggle) {
		return;
	}

	jigleSSMDL = CharacterModels[349].Model->child->chunkmodel;
	CharacterModels[349].Model->child->chunkmodel = sonicCO2->SpineJiggle->SourceModelCopy->chunkmodel;
}

void ResetJiggle(SonicCharObj2* sonicCO2)
{
	if (!sonicCO2->SpineJiggle || !jigleSSMDL) {
		return;
	}

	CharacterModels[349].Model->child->chunkmodel = jigleSSMDL;
}