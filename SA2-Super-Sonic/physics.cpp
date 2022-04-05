#include "pch.h"

Trampoline* PResetAngle_t;
PhysicsData sonicPhysicsCopy;

//restore sonic's original physics.
void __cdecl SuperPhysics_Delete(ObjectMaster* obj) {
	memcpy(&PhysicsArray[Characters_Sonic], &sonicPhysicsCopy, sizeof(PhysicsData));
	return;
}

void __cdecl SuperPhysics_Main(ObjectMaster* obj) {

	CharObj2Base* co2 = MainCharObj2[obj->Data1.Entity->Index];

	if (!co2 || !isSuper[obj->Data1.Entity->Index])
		DeleteObject_(obj);
}

void __cdecl SuperPhysics_Load(ObjectMaster* obj)
{
	CharObj2Base* co2 = MainCharObj2[obj->Data1.Entity->Index];

	if (co2)
	{
		//save sonic's current physics for restoration later.
		memcpy(&sonicPhysicsCopy, &PhysicsArray[Characters_Sonic], sizeof(PhysicsData));

		//apply custom super sonic physics
		co2->PhysData.AirResist = -0.001f;
		co2->PhysData.AirDecel = -0.0200000009f;
		co2->PhysData.AirAccel = 0.035f;
		co2->PhysData.CenterHeight = 6.4f;

		obj->MainSub = SuperPhysics_Main;
		obj->DeleteSub = SuperPhysics_Delete;
	}
	else
	{
		DeleteObject_(obj);
	}
}


void Load_SuperPhysics(EntityData1* data) {

	if (!isPhysics)
		return;

	ObjectMaster* physics = LoadObject(2, "SuperSonic_Physics", SuperPhysics_Load, LoadObj_Data1 | LoadObj_UnknownB);

	if (physics && data)
	{
		physics->Data1.Entity->Index = data->Index;
	}

	return;
}

static void PResetAngle_Origin(EntityData1* data1, CharObj2Base* data2)
{
	auto target = PResetAngle_t->Target();

	__asm
	{
		mov ebx, [data2]
		mov eax, [data1]
		call target
	}
}

void PResetAngle_r(EntityData1* data1, CharObj2Base* co2)
{

	if (co2->Upgrades & Upgrades_SuperSonic && CurrentLevel != LevelIDs_FinalHazard)
	{
		co2->Upgrades &= ~Upgrades_SuperSonic;
		PResetAngle_Origin(data1, co2);
		co2->Upgrades |= Upgrades_SuperSonic;
	}
	else {
		PResetAngle_Origin(data1, co2);
	}
}

static void __declspec(naked) PResetAngleASM()
{
	__asm
	{
		push ebx 
		push eax 
		call PResetAngle_r
		pop eax
		pop ebx 
		retn
	}
}

void init_PhysicsHack() {
	PResetAngle_t = new Trampoline((int)0x460260, (int)0x460266, PResetAngleASM);
}