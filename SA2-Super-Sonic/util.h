#pragma once

void njCnkMotion(NJS_OBJECT* obj, NJS_MOTION* mot, float frame);
void DoNextAction_r(int playerNum, char action, int unknown);
bool __cdecl IsIngame();
ModelInfo* LoadMDL(const char* name, ModelFormat format);
void FreeMDL(ModelInfo* pointer);