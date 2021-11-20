#include "pch.h"

void njCnkMotion(NJS_OBJECT* obj, NJS_MOTION* mot, float frame)
{
	*(int*)0x25EFE54 = 0x25EFE60;
	njSetMotion(mot, frame);
	MotionDrawCallback = (ObjectFuncPtr)0x42E660;
	DrawObjMotion(obj);
}