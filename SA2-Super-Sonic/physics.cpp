#include "pch.h"

PhysicsData sonicPhysicsCopy;

void __cdecl SuperPhysics_Delete(ObjectMaster* obj) {
	//restore sonic's original physics.
	memcpy(&PhysicsArray[Characters_Sonic], &sonicPhysicsCopy, sizeof(PhysicsData));
}

void __cdecl SuperPhysics_Main(ObjectMaster* obj) {

	CharObj2Base* co2 = MainCharObj2[obj->Data1.Entity->Index];

	if (!co2 || !isSuper)
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

	ObjectMaster* physics = LoadObject(2, "SuperSonic_Physics", SuperPhysics_Load, LoadObj_Data1 | LoadObj_UnknownB);

	if (physics && data)
	{
		physics->Data1.Entity->Index = data->Index;
	}

	return;
}
