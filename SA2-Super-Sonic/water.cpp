#include "pch.h"

//port of the sa1 water effect when hovering on water!

static ModelInfo* WaterMdl[2] = { nullptr, nullptr }; //water effect model
static ModelInfo* gridCol = nullptr; //used to make Sonic able to float on water

static NJS_TEXNAME watertexid0 = { (char*)"ss_waterl01", 0, 0 };
static NJS_TEXNAME watertexid1 = { (char*)"ss_waterl03", 0, 0 };
static NJS_TEXNAME watertexid2 = { (char*)"ss_waterl05", 0, 0 };
static NJS_TEXNAME watertexid3 = { (char*)"ss_waterl07", 0, 0 };
static NJS_TEXNAME watertexid4 = { (char*)"ss_waterl09", 0, 0 };
static NJS_TEXNAME watertexid5 = { (char*)"ss_waterl11", 0, 0 };
static NJS_TEXNAME watertexid6 = { (char*)"ss_waterl13", 0, 0 };
static NJS_TEXNAME watertexid7 = { (char*)"ss_waterl15", 0, 0 };

extern NJS_TEXLIST SSEff_Texlist;
extern NJS_TEXLIST SSHEff_Texlist;

static NJS_TEXLIST waterTexList[8] = {
	&watertexid0, 0,
	&watertexid1, 0,
	&watertexid2, 0,
	&watertexid3, 0,
	&watertexid4, 0,
	&watertexid5, 0,
	&watertexid6, 0,
	&watertexid7, 0
};

void __cdecl SS_Water_Display(ObjectMaster* obj)
{
	float XScale;
	float XScalea;
	float YScale;
	float ZScale;

	EntityData1* data = obj->Data1.Entity;

	char pnum = data->Index;
	EntityData1* playerData = MainCharObj1[pnum];
	CharObj2Base* co2 = MainCharObj2[pnum];

	if ((data->Action >= 2))
	{
		njSetTexture(&waterTexList[((unsigned __int8)data->Timer >> 1) & 7]);
		njPushMatrix(CURRENT_MATRIX);
		njTranslateV(0, &playerData->Position);

		njRotateZ_(CURRENT_MATRIX, (unsigned __int16)playerData->Rotation.z);
		njRotateX_(CURRENT_MATRIX, (unsigned __int16)playerData->Rotation.x);
		njRotateY_(CURRENT_MATRIX, (unsigned __int16)-(playerData->Rotation.y));

		if (data->Scale.x != 0.0)   // draw water effect
		{
			njPushMatrix(0);
			XScale = data->Scale.x * -1.0;
			njScale(0, XScale, data->Scale.x, data->Scale.x);
			DrawObject(WaterMdl[0]->getmodel());
			DrawObject(WaterMdl[1]->getmodel());
			njPopMatrix(1u);
		}
		if (data->Scale.y != 0.0)
		{
			njPushMatrix(CURRENT_MATRIX);
			ZScale = data->Scale.y * 1.3;
			YScale = data->Scale.y * 0.80000001;
			XScalea = data->Scale.y * -0.80000001;
			njScale(0, XScalea, YScale, ZScale);
			DrawObject(WaterMdl[0]->getmodel());
			DrawObject(WaterMdl[1]->getmodel());
			njPopMatrix(1u);
		}
		njPopMatrix(1u);
	}
}

DataPointer(float, MH_WaterPosY, 0xEF68B8);
bool isFakeWaterLevel()
{
	return CurrentLevel == LevelIDs_GreenForest || CurrentLevel == LevelIDs_MetalHarbor;
}

float getWaterposY(char pnum)
{
	if (!MainCharObj1[pnum])
		return 0.0f;

	if (CurrentLevel == LevelIDs_MetalHarbor)
	{
		return MH_WaterPosY + 5.0f;
	}

	return MainCharObj1[pnum]->Position.y;
}


void UpdateWaterEffectPos(ObjectMaster* obj, EntityData1* player)
{
	NJS_OBJECT* dyncol = (NJS_OBJECT*)obj->EntityData2;
	EntityData1* data = obj->Data1.Entity;

	data->Position = { player->Position.x, getWaterposY(data->Index), player->Position.z };
	*(NJS_VECTOR*)dyncol->pos = data->Position; //update dyncol pos
}

bool isPlayerOnWater(CharObj2Base* co2, EntityData1* player)
{
	if (!co2)
		return false;
	
	if (co2->SurfaceInfo.TopSurface & SurfaceFlag_Water || isFakeWaterLevel() && player->Position.y <= getWaterposY(co2->PlayerNum) +5.0f)
		return true;

	return false;
}


NJS_OBJECT* DynCol_AddFromObject(ObjectMaster* obj, NJS_OBJECT* object, NJS_VECTOR* position, Angle rotY, int flags)
{
	NJS_OBJECT* dynobj = GetFreeDyncolObjectEntry();

	if (dynobj)
	{
		memcpy(dynobj, object, sizeof(NJS_OBJECT));

		dynobj->evalflags &= 0xFFFFFFFC;

		dynobj->ang[1] = rotY;
		dynobj->pos[0] = position->x;
		dynobj->pos[1] = position->y;
		dynobj->pos[2] = position->z;

		DynCol_Add((SurfaceFlags)flags, obj, dynobj);
		obj->EntityData2 = (UnknownData2*)dynobj;
	}

	return dynobj;
}



void __cdecl SS_Water_Main(ObjectMaster* obj)
{
	EntityData1* data = obj->Data1.Entity;

	char pnum = data->Index;

	if (!MainCharObj1[pnum])
		return;

	EntityData1* player = MainCharObj1[pnum];

	switch (data->Action)
	{
	case 0:

		obj->DisplaySub = SS_Water_Display;
		obj->field_4C = gridCol->getmodel();
		obj->DeleteSub = DeleteFunc_DynCol;
		DynCol_AddFromObject(obj, (NJS_OBJECT*)obj->field_4C, &data->Position, data->Rotation.y, SurfaceFlag_Solid | SurfaceFlag_Dynamic);
		data->Action++;
		break;
	case 1:

		if (isPlayerOnWater(MainCharObj2[pnum], MainCharObj1[pnum])) {
			player->Position.y += 1.0f;
			data->Action++;
		}

		break;
	case 2:
	{
		UpdateWaterEffectPos(obj, player);

		if (data->Timer >= SHRT_MAX)
			data->Timer = 0;

		data->Timer++;

		if (!isPlayerOnWater(MainCharObj2[pnum], MainCharObj1[pnum]) || player->Action == Action_Jump) {
			data->Position = { 0, -10000, 0 };
			data->Action = 1;
			data->Timer = 0;
			return;
		}
	}

		break;
	}
}

void Load_SSWaterTask(char pid)
{
	ObjectMaster* water = LoadObject(2, "SS_Water_Eff", SS_Water_Main, LoadObj_Data1 | LoadObj_Data2);
	water->Data1.Entity->Index = pid;
	return;
}


void LoadWaterTextures(char charID)
{

	char count = 0;
	if (charID == Characters_Sonic) {

		for (uint8_t i = 0; i < LengthOfArray(waterTexList); i++) {

			waterTexList[i].textures[0] = SSEff_Texlist.textures[1 + count];
			count += 2;
		}
	}
	else
	{
		for (uint8_t i = 0; i < LengthOfArray(waterTexList); i++) {

			waterTexList[i].textures[0] = SSHEff_Texlist.textures[1 + count];
			count += 2;
		}
	}
}

void LoadWaterMDL()
{
	WaterMdl[0] = LoadMDL("WaterMDL0", ModelFormat_Chunk);
	WaterMdl[1] = LoadMDL("WaterMDL1", ModelFormat_Chunk);
	gridCol = LoadMDL("gridCol", ModelFormat_Basic);

}

void FreeWaterMDL()
{
	for (uint8_t i = 0; i < LengthOfArray(WaterMdl); i++)
	{
		FreeMDL(WaterMdl[i]);
	}
}