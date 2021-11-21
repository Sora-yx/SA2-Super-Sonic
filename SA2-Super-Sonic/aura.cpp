#include "pch.h"

//serie of hacks to disable ball form model and aura when Super to fix crashes.

Trampoline* SpinDashAura_Display_t;
Trampoline* DoSpinDashAura_t;
Trampoline* DoJumpAura_t;
Trampoline* DoHomingAura_t;
Trampoline* LoadHomingAura_t;

ObjectFunc(SpinDashAura_Display, 0x756040);
DataPointer(float, flt_B18F54, 0xB18F54);

void __cdecl SpinDashAura_Display_r(ObjectMaster* a1)
{
	if (isSuper)
		return;

	ObjectFunc(origin, SpinDashAura_Display_t->Target());
	return origin(a1);
}

void DoSpinDashAura_Origin(ObjectMaster* a1)
{
	auto target = DoSpinDashAura_t->Target();

	__asm
	{
		mov edi, [a1]
		call target
	}
}

void DoSpinDashAura_r(ObjectMaster* obj)
{
	auraStruct* aura; // esi
	CharObj2Base* co2; // eax
	char charID; // ecx
	char charID2; // cl

	aura = (auraStruct*)obj->Data2.Undefined;

	if (!MainCharObj1[aura->charID] || !aura)
		return;

	charID = aura->charID;
	co2 = MainCharObj2[charID];
	charID2 = co2->CharID2;

	if (charID2 == Characters_Amy || charID2 == Characters_MetalSonic || co2->CharID == Characters_Shadow || !isSuper)
	{
		return DoSpinDashAura_Origin(obj);
	}

	//the aura could work for Super Sonic if he had a ball form but he doesn't.
	return;
}

static void __declspec(naked) DoSpinDashAuraASM()
{
	__asm
	{
		push edi // obj

		// Call your __cdecl function here:
		call DoSpinDashAura_r

		pop edi // obj
		retn
	}
}

void DoJumpAura_Origin(ObjectMaster* a1)
{
	auto target2 = DoJumpAura_t->Target();

	__asm
	{
		mov edi, [a1]
		call target2
	}
}

void DoJumpAura_r(ObjectMaster* obj)
{
	auraStruct* aura; // esi
	CharObj2Base* co2; // eax
	char charID; // ecx
	char charID2; // cl

	aura = (auraStruct*)obj->Data2.Undefined;

	if (!MainCharObj1[aura->charID] || !aura)
		return;

	charID = aura->charID;
	co2 = MainCharObj2[charID];
	charID2 = co2->CharID2;

	if (charID2 == Characters_Amy || charID2 == Characters_MetalSonic || co2->CharID == Characters_Shadow || !isSuper)
	{
		return DoJumpAura_Origin(obj);
	}

	//the aura could work for Super Sonic if he had a ball form but he doesn't.
	return;
}

static void __declspec(naked) DoJumpAuraASM()
{
	__asm
	{
		push edi // obj

		// Call your __cdecl function here:
		call DoJumpAura_r

		pop edi // obj
		retn
	}
}

void DoHomingAura_Origin(ObjectMaster* a1)
{
	auto target3 = DoHomingAura_t->Target();

	__asm
	{
		mov edi, [a1]
		call target3
	}
}

void DoHomingAura_r(ObjectMaster* obj)
{
	auraStruct* aura; // esi
	CharObj2Base* co2; // eax
	char charID; // ecx
	char charID2; // cl

	aura = (auraStruct*)obj->Data2.Undefined;

	if (!MainCharObj1[aura->charID] || !aura)
		return;

	charID = aura->charID;
	co2 = MainCharObj2[charID];
	charID2 = co2->CharID2;

	if (charID2 == Characters_Amy || charID2 == Characters_MetalSonic || co2->CharID == Characters_Shadow || !isSuper)
	{
		return DoHomingAura_Origin(obj);
	}

	//the aura could work for Super Sonic if he had a ball form but he doesn't.
	return;
}

static void __declspec(naked) DoHomingAuraASM()
{
	__asm
	{
		push edi // obj

		// Call your __cdecl function here:
		call DoHomingAura_r

		pop edi // obj
		retn
	}
}


void __cdecl LoadHomingAura_r(ObjectMaster* a1)
{
	if (isSuper)
		return;

	ObjectFunc(origin, LoadHomingAura_t->Target());
	return origin(a1);
}


void initAura_Hack() {
	//Remove aura when Super Sonic because the game crashes for model not found.
	SpinDashAura_Display_t = new Trampoline((int)0x756040, (int)0x75604A, SpinDashAura_Display_r);
	DoSpinDashAura_t = new Trampoline((int)0x7562A0, (int)0x7562A7, DoSpinDashAuraASM);
	DoJumpAura_t = new Trampoline((int)0x756AE0, (int)0x756AE5, DoJumpAuraASM);
	DoHomingAura_t = new Trampoline((int)0x7566C0, (int)0x7566C5, DoHomingAuraASM);
	LoadHomingAura_t = new Trampoline((int)0x7577E0, (int)0x7577E6, LoadHomingAura_r);
}