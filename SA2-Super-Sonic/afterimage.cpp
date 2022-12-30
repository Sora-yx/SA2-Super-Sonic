#include "pch.h"

static UsercallFuncVoid(SonicAfterImage_t, (EntityData1* a1, CharObj2Base* a2, SonicCharObj2* a3), (a1, a2, a3), 0x71E460, rESI, stack4, stack4);

void SonicDisplayAfterImage_r(EntityData1* a1, CharObj2Base* a2, SonicCharObj2* a3)
{
	if (isSuper[a2->PlayerNum] && !a2->CharID2)
	{
		return;
	}

	return SonicAfterImage_t.Original(a1, a2, a3);
}

void init_AfterImages() {
	SonicAfterImage_t.Hook(SonicDisplayAfterImage_r);
}