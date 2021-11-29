#pragma once

#define MATRIX_1A51A00 ((NJS_MATRIX_PTR)0x1A51A00) //matrix used for Sonic CallBack

FunctionPointer(void, DrawObject, (NJS_OBJECT* a1), 0x42E730);
FunctionPointer(void, SuperSonicGetAccel, (EntityData1* data, EntityData2* data2, CharObj2Base* co2), 0x49D870);

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


//void __usercall drawUV(NJS_TEXLIST* a1@<esi>, NJS_OBJECT* obj, float FrameCounter)
static const void* const drawUV_ptr = (void*)0x795790;
static inline void DrawUV(int* a1, NJS_OBJECT* obj, float FrameCounter)
{
	__asm
	{
		push[FrameCounter]
		push[obj]
		mov esi, [a1]
		call drawUV_ptr
		add esp, 4
	}
}


FunctionPointer(void, njReleaseTexture, (NJS_TEXLIST* a1), 0x77F9F0);
DataPointer(RenderInfo, Has_texlist_batadvPlayerChara_in_it, 0x2670544);
FunctionPointer(int, sub_458970, (), 0x458970);
DataPointer(char, byte_1DE4400, 0x1DE4400);

//stuff
DataPointer(SonicCharObj2*, SonicCO2PtrExtern, 0x01A51A9C);

FunctionPointer(void, sub_427040, (NJS_MATRIX a1, float* a2), 0x427040);
DataPointer(char, isLoading, 0x174AFC0);
DataPointer(NJS_VECTOR*, cameraPosMaybe, 0x01DD92B0);

DataPointer(char, TimerStopped, 0x174afda);

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

static const void* const njDrawObjMotionPtr_ = (void*)0x780870;
static inline void DrawMotionAndObject(NJS_MOTION* mtn, NJS_OBJECT* obj, float frame)
{
	__asm
	{
		push[frame]
		push[obj]
		mov ecx, mtn
		call njDrawObjMotionPtr_
		add esp, 8
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
DataPointer(WORD, animate_AuraThing, 0x170ACEE);

FunctionPointer(JiggleInfo*, LoadJiggle, (NJS_OBJECT* jiggle), 0x447580);

//void __usercall Delete_Jiggle(JiggleInfo* Jiggle@<esi>)
static const void* const jiggle_ptr = (void*)0x447910;
static inline void Delete_Jiggle(JiggleInfo* jiggle)
{
	__asm
	{
		mov esi, jiggle
		call jiggle_ptr
	}
}


static const void* const PGetSpeedPtr = (void*)0x460860;
static inline void PGetSpeed(EntityData1* a1, CharObj2Base* co2, EntityData2* data2)
{
	__asm
	{
		push[data2] // a3
		mov ebx, co2 // a2
		mov eax, a1 // a1

		// Call your __cdecl function here:
		call PGetSpeedPtr

		add esp, 4 // a3
	}
}


static const void* const PResetPosPtr = (void*)0x469050;
static inline void PResetPosition(EntityData1* a1, EntityData2* a2, CharObj2Base* a3)
{
	__asm
	{
		push[a3] // a3
		mov ebx, a2 // a2
		mov eax, a1 // a1
		call PResetPosPtr
		add esp, 4 // a2
	}
}

static const void* const PSetPositionptr = (void*)0x4616E0;
static inline int PSetPosition(EntityData1* a1, EntityData2* a2, CharObj2Base* a3)
{
	int result;
	__asm
	{
		push[a3]
		push[a2]
		mov eax, [a1]
		call PSetPositionptr
		add esp, 8
		mov result, eax
	}
	return result;
}

FunctionPointer(void, PGetRotation, (EntityData1* data, EntityData2* data2, CharObj2Base* co2), 0x45FA70);
FunctionPointer(double, FloatCalcResult, (float a1, float a2, float a3), 0x447520);

static const void* const VibeThingPtr = (void*)0x438E70;
static inline void VibeThing(int a1, signed int a2, int a3, signed int a4)
{
	__asm
	{
		push[a4] // int a4
		mov ecx, a3 // a3
		mov edx, a2 // int a2
		mov eax, a1 // a1

		// Call your __cdecl function here:
		call VibeThingPtr
		add esp, 4 // int a4
	}
}

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

//int __usercall Sonic_CheckNextAction@<eax>(SonicCharObj2* a1@<eax>, EntityData1* a2, EntityData2* a3, CharObj2Base* a4)
static const void* const SonicNactPtr = (void*)0x7220E0;
static inline signed int  Sonic_CheckNextAction(SonicCharObj2* a1, EntityData1* a2, EntityData2* a3, CharObj2Base* a4)
{
	signed int result;
	__asm
	{
		push[a4] 
		push[a3]
		push[a2]
		mov eax, a1
		call SonicNactPtr
		mov result, eax
		add esp, 12
	}
}

#pragma pack(push, 1)
struct __declspec(align(2)) HomingAttackTarget
{
	EntityData1* entity;
	float distance;
};
#pragma pack(pop)


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
