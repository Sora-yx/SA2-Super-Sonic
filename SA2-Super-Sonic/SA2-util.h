#pragma once

#define MATRIX_1A51A00 ((NJS_MATRIX_PTR)0x1A51A00) //matrix used for Sonic CallBack


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
FunctionPointer(int, sub_458970, (), 0x458970);
DataPointer(char, byte_1DE4400, 0x1DE4400);

//stuff
FunctionPointer(void, sub_427040, (NJS_MATRIX_PTR a1, NJS_MATRIX_PTR a2), 0x427040); //njs_matrix_ptr not float, optimization in release = fucked
DataPointer(char, isLoading, 0x174AFC0);
DataPointer(NJS_VECTOR*, cameraPosMaybe, 0x01DD92B0);

ObjectFunc(SuperAura, 0x49CD50);

using ModelFuncPtr = void(__cdecl*)(NJS_OBJECT*);
DataPointer(ModelFuncPtr, UpgradeDrawCallback, 0x1A55834);
FunctionPointer(void, DrawObjWithCallBack, (NJS_OBJECT* obj), 0x42E730);

//void __usercall sub_426420(int a1@<eax>, int a2)
static const void* const sub426420_ptr = (void*)0x426420;
static inline void sub_426420(int a1, int a2)
{
	__asm
	{
		push[a2]
		mov eax, a1
		call sub426420_ptr
		add esp, 4
	}
}

//void __usercall sub_426E40(float* a1@<eax>, float* a2@<edx>, float* a3@<ecx>
static const void* const sub_426E40_ptr = (void*)0x426E40;
static inline void sub_426E40(float* a1, float* a2, float* a3)
{
	__asm
	{
		mov ecx, a3
		mov edx, a2
		mov eax, a1
		call sub_426E40_ptr
	}
}

static const void* const njCalcPointPtr_ = (void*)0x426CC0;
static inline void njCalcPoint_(float* matrix, NJS_VECTOR* v, NJS_VECTOR* transform, char additive)
{
	__asm
	{
		movzx eax, [additive]
		push eax
		mov ecx, [transform]
		mov edx, [v]
		mov eax, [matrix]
		call njCalcPointPtr_
		add esp, 4;
	}
}

static const void* const njCalcVectorPtr_ = (void*)0x4273B0;
static inline void njCalcVector_(NJS_VECTOR* transform, NJS_VECTOR* v, NJS_MATRIX_PTR m)
{
	__asm
	{
		mov ecx, [m]
		mov edx, [v]
		mov eax, [transform]
		call njCalcVectorPtr_
	}
}


DataPointer(NJS_MATRIX, sonic_RightHandMatrix, 0x1A51A3C);
DataPointer(NJS_MATRIX, sonic_LeftHandMatrix, 0x1A51AA0);
DataPointer(NJS_MATRIX, sonic_LeftFootMatrix, 0x1A51A6C);
DataPointer(NJS_MATRIX, sonic_RightFootMatrix, 0x1A519D0);

DataPointer(int, dword_25F02D8, 0x25F02D8);
DataPointer(int, dword_1DEB6A4, 0x1DEB6A4);

DataArray(float, flt_25F02A0, 0x25F02A0, 5);
FunctionPointer(void, Sonic_CallBack, (NJS_OBJECT* mdl), 0x71EAA0);
DataPointer(uint8_t, animate_AuraThing, 0x170ACEE);

FunctionPointer(double, FloatCalcResult, (float a1, float a2, float a3), 0x447520);


static const void* const fbossSoundPtr = (void*)0x435630;
static inline void LoadFinalBossSound(const char* a1, void* address, char a3)
{
	__asm
	{
		push[a3] // int a4
		push[address]
		mov edi, a1
		call fbossSoundPtr
	}
}


//HomingAttackTarget* __usercall sub_721480@<eax>(CharObj2Base* eax0@<eax>, EntityData1* a2, float a3)
static const void* const sub_721480_ptr = (void*)0x721480;
static inline HomingAttackTarget* sub_721480(CharObj2Base* a1, EntityData1* a2, float a3)
{
	HomingAttackTarget* result;
	__asm
	{
		push[a3]
		push[a2]
		mov eax, a1
		call sub_721480_ptr
		mov result, eax
		add esp, 8
	}
	return result;
}

ObjectFunc(sub_437E10, 0x437E10);

static const void* const sub_458BB0Ptr = (void*)0x458BB0;
static inline void sub_458BB0(float* result)
{
	__asm
	{
		mov eax, [result]
		call sub_458BB0Ptr
	}
}

static const void* const sub_458C30Ptr = (void*)0x458BB0;
static inline void sub_458C30(int a1)
{
	__asm
	{
		mov eax, [a1]
		call sub_458C30Ptr
	}
}

DataPointer(float, HorizontalStretch, 0x01A559E4);


static const void* const PoseEffectManPtr = (void*)0x477490;
static inline void PoseEffectMan_Load_(int a1, int a2)
{
	__asm
	{
		push[a2]
		mov eax, [a1]
		call PoseEffectManPtr
		add esp, 4
	}
}

FunctionPointer(void, LoadChaosControlEffect, (char pnum), 0x758C60);
ObjectFunc(ChaosControl_Display, 0x724780);


static const void* const sub_42F770Ptr = (void*)0x42F770;
static inline void sub_42F770(float* a1)
{
	__asm
	{
		mov eax, [a1]
		call sub_42F770Ptr
	}
}

//void __usercall LoadSplashEffect(int a1@<ebx>)
static const void* const Smallsplash_ptr = (void*)0x6ED630;
static inline void LoadSplashEffect(int a1)
{
	__asm
	{
		mov ebx, a1
		call Smallsplash_ptr
	}
}

//void __usercall LoadSplashEffect(int a1@<ebx>)
static const void* const sub_476CC0_ptr = (void*)0x476CC0;
static inline void sub_476CC0(int pnum, NJS_VECTOR* pos)
{
	__asm
	{
		mov edi, pos
		mov eax, pnum
		call sub_476CC0_ptr
	}
}

