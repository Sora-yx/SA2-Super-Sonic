#pragma once

void SuperSonic_Callback_r(NJS_OBJECT* mdl);
void Play_SuperSonicMusic();
void RestoreMusic();
void Load_SuperPhysics(EntityData1* data);
void LoadSuperAura(char pID);
void DisplaySuperSonic_Upgrade(EntityData1* data1, SonicCharObj2* sonicCO2);
void SuperSonicFly_MainManagement(EntityData1* data1, CharObj2Base* co2, EntityData2* data2);
void SuperSonicFly_ActionsManagement(EntityData1* data1, SonicCharObj2* sCo2, CharObj2Base* co2, EntityData2* data2);

void DeleteJiggle(SonicCharObj2* sonicCO2);
void __cdecl initJiggleSuperSonic(SonicCharObj2* sonicCO2);
void SetJiggle(SonicCharObj2* sonicCO2);
void ResetJiggle(SonicCharObj2* sonicCO2);
void init_AfterImages();
void PResetAngle_r(EntityData1* data1, CharObj2Base* co2);
void Load_NewSuperSonicAnim();
void Delete_SSAnim();
void SuperSonic_DisableFly(EntityData1* data1, CharObj2Base* co2);