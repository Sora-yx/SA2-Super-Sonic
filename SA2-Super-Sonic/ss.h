#pragma once

void SuperSonic_Callback_r(NJS_OBJECT* mdl);
void Play_SuperSonicMusic();
void RestoreMusic();
void Load_SuperPhysics(EntityData1* data);
void LoadSuperAura(char pID);
void DisplaySuperSonic_Upgrade(SonicCharObj2* sonicCO2);
void SuperSonicFly_MainManagement(EntityData1* data1, CharObj2Base* co2, EntityData2* data2);
void SuperSonicFly_ActionsManagement(EntityData1* data1, CharObj2Base* co2, EntityData2* data2);