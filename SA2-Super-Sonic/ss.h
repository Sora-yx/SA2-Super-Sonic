#pragma once

void SuperSonic_Callback_r(NJS_OBJECT* mdl);
void Play_SuperSonicMusic();
void RestoreMusic();
void Load_SuperPhysics(EntityData1* data);
void LoadSuperAura(char pID);
void DisplaySuperSonic_Upgrade(EntityData1* data1, SonicCharObj2* sonicCO2);
void SuperSonicFly_MainManagement(EntityData1* data1, CharObj2Base* co2, EntityData2* data2);
void SuperSonicFly_ActionsManagement(EntityData1* data1, SonicCharObj2* sCo2, CharObj2Base* co2);

void DeleteSSJiggle(SonicCharObj2* sonicCO2);
void __cdecl initJiggleSuperSonic(SonicCharObj2* sonicCO2);
void Set_SSJiggle(SonicCharObj2* sonicCO2);
void ResetSSJiggle(SonicCharObj2* sonicCO2);
void init_AfterImages();
void PResetAngle_r(EntityData1* data1, CharObj2Base* co2);
void Load_NewSuperSonicAnim();
void Delete_SSAnim();
void SuperSonic_DisableFly(EntityData1* data1, CharObj2Base* co2);
void ChaosControl_Management(CharObj2Base* co2);
void ResetChaosControl(char pnum);
void initChaosControl_Hack();
void ResetWindCutter(char pnum);

void DeleteSSHJiggle(SonicCharObj2* sonicCO2);
void __cdecl initJiggleSuperShadow(SonicCharObj2* sonicCO2);
void Set_SSHJiggle(SonicCharObj2* sonicCO2);
void ResetSSHJiggle(SonicCharObj2* sonicCO2);

void DrawSonicMotion(EntityData1* data1, SonicCharObj2* sonicCO2);
void __cdecl DoSpinDashRotationModel();

void SubRings(unsigned char player, EntityData1* data);
void __cdecl LoadSSEff_Textures();
bool CheckUntransform_Input(unsigned char playerID);
bool CheckTransform_Input(char playerID, EntityData1* player);

void Load_NewSuperShadowAnim();
void Delete_SSHAnim();
void DisplaySuperShadow_Upgrade(EntityData1* data1, SonicCharObj2* sonicCO2);
void SuperShadow_Callback_r(NJS_OBJECT* mdl);

void Load_SuperShadowPhysics(EntityData1* data);
void unSuperShadow(unsigned char player);
void Check_SonicWind(CharObj2Base* co2);
void init_SonicWindHack();
void LoadWaterTextures(char charID);
void LoadWaterMDL();
void FreeWaterMDL();
void Load_SSWaterTask(char pid);
bool isPlayerOnWater(CharObj2Base* co2, EntityData1* player);

void LoadSADXAuraTextures(char charID);