#include "pch.h"

//serie of hacks to display a yellow aura when using Super Sonic, done by using shadow texlist.
//Also add the super aura from Finalhazard fight

Trampoline* DoSpinDashAura_t;
Trampoline* DoJumpAura_t;
Trampoline* DoHomingAura_t;
Trampoline* HomingDashAura_Display_t;

ModelInfo* SADXSuperAuraModel[3];

bool isNotSuperSonic(CharObj2Base* co2)
{
	char charID2 = co2->CharID2;

	if (charID2 == Characters_Amy || charID2 == Characters_MetalSonic || co2->CharID == Characters_Shadow || !isSuper[co2->PlayerNum])
	{
		return true;
	}

	return false;
}

void DoSpinDashAura_Origin(ObjectMaster* a1)
{
	auto target = DoSpinDashAura_t->Target();

	__asm
	{
		mov edi, [a1]
		call target
	}
}

void DoSpinDashAura_r(ObjectMaster* obj)
{
	auraStruct* aura; // esi
	NJS_VECTOR a1;
	double v2; // st7
	double v3; // st7
	float* v4; // ebx
	double v5; // st7
	CharObj2Base* co2; // eax
	int v7; // ecx
	aura = (auraStruct*)obj->Data2.Undefined;

	if (!MainCharObj1[aura->charID] || !aura)
		return;

	if (isNotSuperSonic(MainCharObj2[aura->charID]))
	{
		return DoSpinDashAura_Origin(obj);
	}


	aura = (auraStruct*)obj->Data2.Undefined;
	if (MainCharObj1[aura->charID])
	{
		a1.z = 0.0;
		a1.x = 0.0;
		a1.y = (double)rand() * 0.000030517578125 + 0.2000000029802322;
		njPushUnitMatrix();
		v2 = (double)rand() * 0.000030517578125 * 65536.0;
		if ((int)v2)
		{
			njRotateZ((float*)_nj_current_matrix_ptr_, (int)v2);
		}
		v3 = (double)rand() * 0.000030517578125 * 65536.0;
		if ((int)v3)
		{
			njRotateX((float*)_nj_current_matrix_ptr_, (int)v3);
		}
		v5 = (double)rand() * 0.000030517578125 * 65536.0;
		v4 = (float*)_nj_current_matrix_ptr_;
		if ((int)v5)
		{
			njRotateY(_nj_current_matrix_ptr_, (int)v5);
		}
		njCalcVector_(&a1, &aura->pos, v4);
		njPopMatrix(1u);
	}
	v7 = aura->charID;
	*(float*)&aura->idk3[1] = flt_B18F54;
	aura->idk3[0] = 1;
	co2 = MainCharObj2[v7];
	char charID2 = co2->CharID2;

	if (CharacterModels[19].Model)
		aura->Model = CharacterModels[19].Model;
	else
		return;

	if ((double)rand() * 0.000030517578125 <= 0.5)
	{
		aura->Tex = Texlist_ShadEff.textures + 1;
	}
	else
	{
		aura->Tex = Texlist_ShadEff.textures;
	}

	obj->DisplaySub_Delayed1 = SpinDashAura_Display;

	return;
}

static void __declspec(naked) DoSpinDashAuraASM()
{
	__asm
	{
		push edi // obj

		// Call your __cdecl function here:
		call DoSpinDashAura_r

		pop edi // obj
		retn
	}
}

void DoJumpAura_Origin(ObjectMaster* a1)
{
	auto target2 = DoJumpAura_t->Target();

	__asm
	{
		mov edi, [a1]
		call target2
	}
}


void DoJumpAura_r(ObjectMaster* obj)
{
	auraStruct* aura = (auraStruct*)obj->Data2.Undefined;

	if (!MainCharObj1[aura->charID] || !aura)
		return;

	if (isNotSuperSonic(MainCharObj2[aura->charID]))
	{
		return DoJumpAura_Origin(obj);
	}

	char charID = aura->charID;

	EntityData1* playerData = MainCharObj1[charID];

	if (playerData)
	{
		CollisionInfo* col = playerData->Collision;
		if (col)
		{
			CollisionData* colArray = col->CollisionArray;
			aura->pos.x = colArray->center.x;
			aura->pos.y = colArray->center.y;
			aura->pos.z = colArray->center.z;
		}
	}

	aura->idk3[0] = 1;
	*(float*)&aura->idk3[1] = 1.2;

	if (CharacterModels[19].Model)
		aura->Model = CharacterModels[19].Model;
	else
		return;

	if ((double)rand() * 0.000030517578125 <= 0.5)
	{
		aura->Tex = Texlist_ShadEff.textures + 1;
	}
	else
	{
		aura->Tex = Texlist_ShadEff.textures;
	}
	obj->DisplaySub_Delayed1 = JumpAura_Display;
	return;
}

static void __declspec(naked) DoJumpAuraASM()
{
	__asm
	{
		push edi // obj

		// Call your __cdecl function here:
		call DoJumpAura_r

		pop edi // obj
		retn
	}
}

void DoHomingAura_Origin(ObjectMaster* a1)
{
	auto target3 = DoHomingAura_t->Target();

	__asm
	{
		mov edi, [a1]
		call target3
	}
}

void DoHomingAura_r(ObjectMaster* obj)
{
	auraStruct* aura = (auraStruct*)obj->Data2.Undefined;

	if (!aura || !MainCharObj1[aura->charID])
		return;

	if (isNotSuperSonic(MainCharObj2[aura->charID]))
	{
		return DoHomingAura_Origin(obj);
	}

	EntityData1* pData = MainCharObj1[aura->charID];
	int rng; // eax
	double v4; // st7
	double v5; // st7
	NJS_MATRIX_PTR* v6; // ebx
	int charID; // ecx
	CharObj2Base* co2; // eax
	char charID2; // cl
	NJS_VECTOR result; // [esp+Ch] [ebp-Ch] BYREF

	aura = (auraStruct*)obj->Data2.Undefined;

	if (pData)
	{
		result.z = 0.0;
		result.x = 0.0;
		rng = rand();
		result.y = (double)rng * 0.000030517578125 + (double)rng * 0.000030517578125 + 1.0;
		njPushUnitMatrix();
		v4 = (double)rand() * 0.000030517578125 * 65536.0;
		if ((int)v4)
		{
			njRotateZ((float*)_nj_current_matrix_ptr_, (int)v4);
		}
		v5 = (double)rand() * 0.000030517578125 * 65536.0;
		v6 = &_nj_current_matrix_ptr_;
		if ((int)v5)
		{
			njRotateX((float*)_nj_current_matrix_ptr_, (int)v5);
		}
		if (pData->Rotation.y)
		{
			njRotateY(*v6, pData->Rotation.y);
		}
		njCalcVector_(&result, &aura->pos, (float*)v6); //njCalcVector
		njPopMatrix(1u);
	}

	charID = aura->charID;
	*(float*)&aura->idk3[1] = 1.2;
	aura->idk3[0] = 1;
	co2 = MainCharObj2[charID];
	charID2 = co2->CharID2;

	if (CharacterModels[19].Model)
		aura->Model = CharacterModels[19].Model;
	else
		return;

	if ((double)rand() * 0.000030517578125 <= 0.5)
	{
		aura->Tex = Texlist_ShadEff.textures + 1;
	}
	else
	{
		aura->Tex = Texlist_ShadEff.textures;
	}

	obj->DisplaySub_Delayed1 = HomingAura_Display;

	return;
}

static void __declspec(naked) DoHomingAuraASM()
{
	__asm
	{
		push edi 
		call DoHomingAura_r
		pop edi
		retn
	}
}

void __cdecl HomingDashAura_Display_r(ObjectMaster* a1)
{
	auraStruct* aura; // esi
	CharObj2Base* v2; // eax
	char charID2; // cl
	int v4; // edx
	void** v5; // eax
	CharObj2Base* co2; // eax
	char charID2Again; // cl
	void** v8; // ecx
	int v10; // eax
	NJS_OBJECT* auraMdl; // esi

	aura = (auraStruct*)a1->Data2.Undefined;
	co2 = MainCharObj2[aura->charID];

	if (isNotSuperSonic(co2))
	{
		ObjectFunc(origin, HomingDashAura_Display_t->Target());
		return origin(a1);
	}

	if (aura->idk == 2)
	{
		v2 = MainCharObj2[aura->charID];
		charID2 = v2->CharID2;
		v4 = *(__int16*)(&aura->charID + 1);

		v5 = &off_A0B1C8 + 2 * v4;
		*(DWORD*)(*(DWORD*)Has_texlist_batadvPlayerChara_in_it.gap0 + 32) = (DWORD)v5;
	}
	else
	{

		co2 = MainCharObj2[aura->charID];
		charID2Again = co2->CharID2;

		v10 = *(__int16*)(&aura->charID + 1);
		v8 = &off_A0B128 + 2 * v10;
		*(DWORD*)(*(DWORD*)Has_texlist_batadvPlayerChara_in_it.gap0 + 32) = (DWORD)v8;
	}

	byte_25EFFCC = 0;
	if (!dword_1A55808)
	{
		sub_420510(0, flt_19341C4, flt_19341C8, dword_19341C0);
	}
	auraMdl = aura->Model;
	sub_42D340();
	ProcessChunkModelsWithCallback(auraMdl, ProcessChunkModel);
	byte_25EFFCC = 1;
	if (!dword_1A55808)
	{
		sub_420510(dword_193411C, flt_19341C4, flt_19341C8, dword_19341C0);
	}
}

char superAuraArray[6] = { 3, 4, 5, 6, 7, 8 };
char superAuraArrayShadow[6] = { 9, 10, 11, 12, 13, 14 };
char auraCount = 0;

void DisplaySuperAura(char character) {

	*(DWORD*)(*(DWORD*)Has_texlist_batadvPlayerChara_in_it.gap0 + 32) = (DWORD)&SSONEFFTEX_TEXLIST;

	if (GameState != GameStates_Pause) {

		//animate Aura using a loop doesn't work for some reason
		if (auraCount == LengthOfArray(superAuraArray) - 1)
			auraCount = 0;
		else
			auraCount++;
	}

	animate_AuraThing = character == Characters_Sonic ? superAuraArray[auraCount] : superAuraArrayShadow[auraCount];
}

void SuperAura_r(ObjectMaster* obj) {

	SonicCharObj2* co2 = (SonicCharObj2*)obj->Data2.Undefined;
	EntityData1* data = obj->Data1.Entity;
	NJS_OBJECT* v24;

	if (co2->base.AnimInfo.Current == 54 || co2->base.AnimInfo.Next == 54 || !isSuper[co2->base.PlayerNum])
	{
		obj->DisplaySub_Delayed4 = nullptr;
		return;
	}

	if (superAuraState == 1)
	{
		if (co2->base.Speed.x >= -5.0 && co2->base.Speed.x < 5.0f && co2->base.Speed.y >= -6.0f && co2->base.Speed.y < 6.0)
			return;
	}

	njSetTexture(co2->TextureList);

	njPushMatrix(CURRENT_MATRIX);

	njTranslate(CURRENT_MATRIX, data->Position.x, data->Position.y, data->Position.z);
	sub_42D340();
	if (data->Rotation.z)
	{
		njRotateZ(CURRENT_MATRIX, data->Rotation.z);
	}
	if (data->Rotation.x)
	{
		njRotateX(CURRENT_MATRIX, data->Rotation.x);
	}
	if (data->Rotation.y != 0x8000)
	{
		njRotateY(CURRENT_MATRIX, 0x8000 - data->Rotation.y);
	}
	njScale(CURRENT_MATRIX, data->Scale.x, data->Scale.y, data->Scale.z);


	DisplaySuperAura(co2->base.CharID2);

	njPushMatrix(CURRENT_MATRIX);
	njTranslate(CURRENT_MATRIX, 0.0, -3.0, 0.0);
	sub_42D340();
	ProcessChunkModelsWithCallback((NJS_OBJECT*)0x170B47C, ProcessChunkModel);// Draw Aura
	njPopMatrix(2u);
}

static NJS_TEXNAME SADXAuratexid0 = { (char*)"S1sonic01", 0, 0 };
static NJS_TEXNAME SADXAuratexid1 = { (char*)"S1sonic02", 0, 0 };
static NJS_TEXNAME SADXAuratexid2 = { (char*)"S1sonic03", 0, 0 };
static NJS_TEXNAME SADXAuratexid3 = { (char*)"S1sonic04", 0, 0 };
static NJS_TEXNAME SADXAuratexid4 = { (char*)"S1sonic05", 0, 0 };
static NJS_TEXNAME SADXAuratexid5 = { (char*)"S1sonic06", 0, 0 };
static NJS_TEXNAME SADXAuratexid6 = { (char*)"S1sonic07", 0, 0 };
static NJS_TEXNAME SADXAuratexid7 = { (char*)"S1sonic08", 0, 0 };


static NJS_TEXLIST SADXSuperAuraTexList[8] = {
	&SADXAuratexid0, 1,
	&SADXAuratexid1, 1,
	&SADXAuratexid2, 1,
	&SADXAuratexid3, 1,
	&SADXAuratexid4, 1,
	&SADXAuratexid5, 1,
	&SADXAuratexid6, 1,
	&SADXAuratexid7, 1
};

static NJS_TEXNAME SADXAuratexid8 = { (char*)"S2sonic01", 0, 0 };
static NJS_TEXNAME SADXAuratexid9 = { (char*)"S2sonic02", 0, 0 };
static NJS_TEXNAME SADXAuratexid10 = { (char*)"S2sonic03", 0, 0 };
static NJS_TEXNAME SADXAuratexid11 = { (char*)"S2sonic04", 0, 0 };
static NJS_TEXNAME SADXAuratexid12 = { (char*)"S2sonic05", 0, 0 };
static NJS_TEXNAME SADXAuratexid13 = { (char*)"S2sonic06", 0, 0 };
static NJS_TEXNAME SADXAuratexid14 = { (char*)"S2sonic07", 0, 0 };
static NJS_TEXNAME SADXAuratexid15 = { (char*)"S2sonic08", 0, 0 };

static NJS_TEXLIST SADXSuperAuraTexList2[8] = {
	&SADXAuratexid8, 1,
	&SADXAuratexid9, 1,
	&SADXAuratexid10, 1,
	&SADXAuratexid11, 1,
	&SADXAuratexid12, 1,
	&SADXAuratexid13, 1,
	&SADXAuratexid14, 1,
	&SADXAuratexid15, 1
};

int timerSuperAura[2] = { 0, 0 };

void SADX_SuperAura(ObjectMaster* obj) {

	SonicCharObj2* Sco2 = (SonicCharObj2*)obj->Data2.Undefined;
	CharObj2Base* co2 = &Sco2->base;
	EntityData1* data = obj->Data1.Entity;
	EntityData1* playerData = MainCharObj1[co2->PlayerNum];
	char pnum = co2->PlayerNum;

	if (co2->AnimInfo.Current == 54 || co2->AnimInfo.Next == 54 || !isSuper[pnum])
	{
		obj->DisplaySub_Delayed4 = nullptr;
		return;
	}

	if (superAuraState == 1)
	{
		if (co2->Speed.x >= -5.0 && co2->Speed.x < 5.0f && co2->Speed.y >= -6.0f && co2->Speed.y < 6.0)
			return;
	}

	if (GameState != GameStates_Pause) {
		timerSuperAura[pnum]++;
	}

	njSetTexture(&SADXSuperAuraTexList[timerSuperAura[pnum] & 7]);
	
	SaveControl3D();
	OnControl3D(NJD_CONTROL_3D_CONSTANT_MATERIAL);
	SetMaterial(1.0, 1.0, 1.0, 1.0);
	njPushMatrix(CURRENT_MATRIX);
	njTranslateV(0, &playerData->Collision->CollisionArray->center);

	njRotateZ_(CURRENT_MATRIX, (unsigned __int16)playerData->Rotation.z);
	njRotateX_(CURRENT_MATRIX, (unsigned __int16)playerData->Rotation.x);
	njRotateY_(CURRENT_MATRIX, (unsigned __int16)-(playerData->Rotation.y));

	njScale(0, -1.0, 1.0, 1.0);

	if (!isBoosting(co2->PlayerNum))
	{
		if (co2->Speed.x <= 7.0)
		{
			DrawObject(SADXSuperAuraModel[0]->getmodel());
		}
		else
		{
			njSetTexture(&SADXSuperAuraTexList[((unsigned __int8)timerSuperAura[pnum] >> 1) & 7]);
			DrawObject(SADXSuperAuraModel[1]->getmodel());
		}
	}
	else 
	{
		njSetTexture(&SADXSuperAuraTexList2[((unsigned __int8)timerSuperAura[pnum] >> 1) & 7]);
		DrawObject(SADXSuperAuraModel[2]->getmodel());
	}

	njPopMatrix(1u);
	ResetMaterial();
	LoadControl3D();
}


void LoadSuperAura(char pID)
{
	if (!superAuraState)
		return;

	MainCharacter[pID]->DisplaySub_Delayed4 = SADXAura ? SADX_SuperAura : SuperAura_r;
	return;
}

void LoadSADXAuraTextures(char charID) {

	if (!SADXAura)
	{
		return;
	}

	timerSuperAura[charID] = 0;

	if (charID == Characters_Sonic) {

		for (uint8_t i = 0; i < LengthOfArray(SADXSuperAuraTexList); i++) {

			SADXSuperAuraTexList[i].textures[0] = SSEff_Texlist.textures[i + 17];
			SADXSuperAuraTexList2[i].textures[0] = SSEff_Texlist.textures[i + 25];
		}
	}
	else
	{
		for (uint8_t i = 0; i < LengthOfArray(SADXSuperAuraTexList); i++) {

			SADXSuperAuraTexList[i].textures[0] = SSHEff_Texlist.textures[i + 17];
			SADXSuperAuraTexList2[i].textures[0] = SSHEff_Texlist.textures[i + 25];
		}
	}
}

void Free_AuraModels()
{
	for (uint8_t i = 0; i < LengthOfArray(SADXSuperAuraModel); i++)
	{
		FreeMDL(SADXSuperAuraModel[i]);
	}
}

//Serie of hack to make every aura display a yellow texture when Super Sonic. (Using Shadow texlist)
void init_AuraHack() {

	//regular sonic aura
	DoSpinDashAura_t = new Trampoline((int)0x7562A0, (int)0x7562A7, DoSpinDashAuraASM);
	DoJumpAura_t = new Trampoline((int)0x756AE0, (int)0x756AE5, DoJumpAuraASM);
	DoHomingAura_t = new Trampoline((int)0x7566C0, (int)0x7566C5, DoHomingAuraASM);
	HomingDashAura_Display_t = new Trampoline((int)0x757040, (int)0x757045, HomingDashAura_Display_r);

	if (SADXAura)
	{
		SADXSuperAuraModel[0] = LoadMDL("SADXAura0", ModelFormat_Chunk);
		SADXSuperAuraModel[1] = LoadMDL("SADXAura1", ModelFormat_Chunk);
		SADXSuperAuraModel[2] = LoadMDL("SADXAura2", ModelFormat_Chunk);
	}

	return;
}