#include "pch.h"

static UsercallFuncVoid(PResetAngle_t, (EntityData1* a1, CharObj2Base* a2), (a1, a2), 0x460260, rEAX, rEBX);
PhysicsData sonicPhysicsCopy = { 0 };
PhysicsData superSonicPhysicsCopy = { 0 };

//restore sonic's original physics.
void __cdecl SuperPhysics_Delete(ObjectMaster* obj) {
	memcpy(&PhysicsArray[Characters_Sonic], &sonicPhysicsCopy, sizeof(PhysicsData));
	memcpy(&PhysicsArray[Characters_SuperSonic], &superSonicPhysicsCopy, sizeof(PhysicsData));
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
		memcpy(&superSonicPhysicsCopy, &PhysicsArray[Characters_SuperSonic], sizeof(PhysicsData));

		//swap to SS physics. (This is to add support for physics swap mod, in vanilla both physics are the same.)
		memcpy(&PhysicsArray[Characters_Sonic], &PhysicsArray[Characters_SuperSonic], sizeof(PhysicsData));

		//apply custom super sonic physics
		if (isPhysics) {
			co2->PhysData.AirResist = -0.001f;
			co2->PhysData.AirDecel = -0.0200000009f;
			co2->PhysData.AirAccel = 0.035f;
			co2->PhysData.CenterHeight = 6.4f;
		}

		obj->MainSub = SuperPhysics_Main;
		obj->DeleteSub = SuperPhysics_Delete;
	}
	else
	{
		DeleteObject_(obj);
	}
}

void Load_SuperPhysics(EntityData1* data) {
	ObjectMaster* physics = LoadObject(2, "SuperSonic_Physics", SuperPhysics_Load, LoadObj_Data1 | LoadObj_UnknownB);

	if (physics && data)
	{
		physics->Data1.Entity->Index = data->Index;
	}

	return;
}

void PResetAngle_r(EntityData1* data1, CharObj2Base* co2)
{
	if (co2->Upgrades & Upgrades_SuperSonic && CurrentLevel != LevelIDs_FinalHazard)
	{
		co2->Upgrades &= ~Upgrades_SuperSonic;
		PResetAngle_t.Original(data1, co2);
		co2->Upgrades |= Upgrades_SuperSonic;
	}
	else {
		PResetAngle_t.Original(data1, co2);
	}
}

void init_PhysicsHack() {
	PResetAngle_t.Hook(PResetAngle_r);
}