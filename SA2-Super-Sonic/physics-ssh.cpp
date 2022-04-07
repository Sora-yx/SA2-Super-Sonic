#include "pch.h"

PhysicsData shadowPhysicsCopy;
PhysicsData superShadowPhysicsCopy;

//restore shadow's original physics.
void __cdecl SuperShadowPhysics_Delete(ObjectMaster* obj) {
	memcpy(&PhysicsArray[Characters_Shadow], &shadowPhysicsCopy, sizeof(PhysicsData));
	memcpy(&PhysicsArray[Characters_SuperShadow], &superShadowPhysicsCopy, sizeof(PhysicsData));
	return;
}

void __cdecl SuperShadowPhysics_Main(ObjectMaster* obj) {

	CharObj2Base* co2 = MainCharObj2[obj->Data1.Entity->Index];

	if (!co2 || !isSuper[obj->Data1.Entity->Index])
		DeleteObject_(obj);
}

void __cdecl SuperShadowPhysics_Load(ObjectMaster* obj)
{
	CharObj2Base* co2 = MainCharObj2[obj->Data1.Entity->Index];

	if (co2)
	{
		//save Shadow's current physics for restoration later.
		memcpy(&shadowPhysicsCopy, &PhysicsArray[Characters_Shadow], sizeof(PhysicsData));
		memcpy(&superShadowPhysicsCopy, &PhysicsArray[Characters_SuperShadow], sizeof(PhysicsData));

		//swap to SS physics. (This is to add support for physics swap mod, in vanilla both physics are the same.)
		memcpy(&PhysicsArray[Characters_Shadow], &PhysicsArray[Characters_SuperShadow], sizeof(PhysicsData));

		//apply custom super sonic physics
		if (isPhysics) {
			co2->PhysData.AirResist = -0.001f;
			co2->PhysData.AirDecel = -0.0200000009f;
			co2->PhysData.AirAccel = 0.035f;
			co2->PhysData.CenterHeight = 6.4f;
		}

		obj->MainSub = SuperShadowPhysics_Main;
		obj->DeleteSub = SuperShadowPhysics_Delete;
	}
	else
	{
		DeleteObject_(obj);
	}
}


void Load_SuperShadowPhysics(EntityData1* data) {

	ObjectMaster* physics = LoadObject(2, "SuperShadow_Physics", SuperShadowPhysics_Load, LoadObj_Data1 | LoadObj_UnknownB);

	if (physics && data)
	{
		physics->Data1.Entity->Index = data->Index;
	}

	return;
}