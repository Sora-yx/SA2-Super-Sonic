#include "pch.h"

Trampoline* SonicAfterImage_t;

static const void* const SonicAfterImagePtr = (void*)0x71E460;
static inline void Sonic_AfterImageOrigin(EntityData1* a1, CharObj2Base* a2, SonicCharObj2* a3)
{
	auto target = SonicAfterImage_t->Target();

	__asm
	{
		push[a3]
		push[a2]
		mov esi, a1
		call target
		add esp, 8
	}
}

void SonicDisplayAfterImage_r(EntityData1* a1, CharObj2Base* a2, SonicCharObj2* a3)
{
	if (isSuper[a2->PlayerNum] && !a2->CharID2)
	{
		return;
	}

	return Sonic_AfterImageOrigin(a1, a2, a3);
}

static void __declspec(naked) SonicDisplayAfterImageASM()
{
	__asm
	{
		push[esp + 08h] // a3
		push[esp + 08h] // data
		push esi // a1

		call SonicDisplayAfterImage_r

		pop esi // a1
		add esp, 4 // data
		add esp, 4 // a3
		retn
	}
}

void init_AfterImages() {
	SonicAfterImage_t = new Trampoline((int)0x71E460, (int)0x71E467, SonicDisplayAfterImageASM);
	return;
}