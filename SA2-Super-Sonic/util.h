#pragma once

void njCnkMotion(NJS_OBJECT* obj, NJS_MOTION* mot, float frame);
void DoNextAction_r(int playerNum, char action, int unknown);
bool __cdecl IsIngame();
ModelInfo* LoadMDL(const char* name, ModelFormat format);
void FreeMDL(ModelInfo* pointer);
void DisplayTimed_Message_OnFrames();
void SendTimedMessage(std::string msg, int timer);
AnimationIndex* getCharAnim_r();
int getNumber_r();
bool isSA1Char(uint8_t charID);

NJS_OBJECT* DynCol_AddFromObject(ObjectMaster* obj, NJS_OBJECT* object, NJS_VECTOR* position, Angle rotY, int flags);
bool isBossLevel();

void njRotateX_r(Angle x);
void njRotateY_r(Angle y);
void njRotateZ_r(Angle z);
void DisplayDebugStringFormatted(Int loc, const char* Format, ...);