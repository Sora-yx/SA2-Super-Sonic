#pragma once

FunctionPointer(void, DrawMotionAndObject, (NJS_OBJECT* a1, float a2), 0x780870);
FunctionPointer(void, DrawObject, (NJS_OBJECT* a1), 0x42E730);

ObjectFunc(SpinDashAura_Display, 0x756040);
ObjectFunc(JumpAura_Display, 0x756960);
ObjectFunc(HomingAura_Display, 0x756520);
DataPointer(float, flt_B18F54, 0xB18F54);

//aura, model stuff
DataPointer(void*, off_A0B1C8, 0xA0B1C8);
DataPointer(void*, off_A0B128, 0xA0B128);
VoidFunc(sub_42D340, 0x42D340);
FunctionPointer(void, sub_420510, (int a2, float a3, float a4, int argC), 0x420510);
DataPointer(char, byte_25EFFCC, 0x25EFFCC);
DataPointer(int, dword_1A55808, 0x1A55808);
DataPointer(int, dword_193411C, 0x193411C);
DataPointer(float, flt_19341C4, 0x19341C4);
DataPointer(float, flt_19341C8, 0x19341C8);
DataPointer(int, dword_19341C0, 0x19341C0);

static const void* const DrawChunkModelPtr = (void*)0x42E6C0;
static inline void DrawChunkModel(NJS_CNK_MODEL* a1)
{
	__asm
	{
		mov eax, [a1]
		call DrawChunkModelPtr
	}
}

static const void* const PlayAnimPtr = (void*)0x4692A0;
static inline void PlayAnimationThing(CharAnimInfo* a1)
{
	__asm
	{
		mov esi, [a1]
		call PlayAnimPtr
	}
}

static const void* const UnloadAnimPtr = (void*)0x459890;
static inline void UnloadAnimation(AnimationIndex* a1)
{
	__asm
	{
		mov esi, [a1]
		call UnloadAnimPtr
	}
}

static const void* const idkPtr = (void*)0x486E50;
static inline void sub_486E50(char a1)
{
	__asm
	{
		mov al, [a1]
		call idkPtr
	}
}

//void __usercall DrawAnimatedObj(NJS_MOTION* a1@<ecx>, NJS_OBJECT* edi0@<edi>, int a3@<esi>, float a2, int a5, int a6)
static const void* const actionPtr = (void*)0x7847D0;
static inline void njAction(NJS_MOTION* a1, NJS_OBJECT* a2, NJS_MOTION* a3, float a4, int(__cdecl* callback)(NJS_CNK_MODEL*), int a6)
{
	__asm
	{
		push[a6]
		push[callback]
		push[a4]
		mov esi, [a3]
		mov edi, [a2]
		mov ecx, [a1]
		call actionPtr
		add esp, 12
	}
}

static const void* const sub_487060Ptr = (void*)0x487060;
static inline void sub_487060(int a1)
{
	__asm
	{
		mov eax, [a1]
		call sub_487060Ptr
	}
}


static const void* const njScaleExPtr = (void*)0x429740;
static inline void njScaleEx(NJS_VECTOR* a1)
{
	__asm
	{
		mov eax, [a1]
		call njScaleExPtr
	}
}

//void __usercall SonicHeldObjectThing(EntityData1* ecx0@<ecx>, CharObj2Base* co2@<eax>)
static const void* const sonicHeldPtr = (void*)0x725C70;
static inline void SonicHeldObjectThing(EntityData1* a1, CharObj2Base* a2)
{
	__asm
	{
		mov eax, [a2]
		mov ecx, [a1]
		call sonicHeldPtr
	}
}

static const void* const sub_46F1E0_ptr = (void*)0x46F1E0;
static inline void sub_46F1E0(int pID)
{
	__asm
	{
		mov eax, [pID]
		call sub_46F1E0_ptr
	}
}


FunctionPointer(void, njReleaseTexture, (NJS_TEXLIST* a1), 0x77F9F0);


DataPointer(RenderInfo, Has_texlist_batadvPlayerChara_in_it, 0x2670544);