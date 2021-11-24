#include "pch.h"

//serie of hacks to display a yellow aura when using Super Sonic, done by using shadow texlist.

Trampoline* DoSpinDashAura_t;
Trampoline* DoJumpAura_t;
Trampoline* DoHomingAura_t;
Trampoline* HomingDashAura_Display_t;


bool isNotSuperSonic(CharObj2Base* co2)
{
	char charID2 = co2->CharID2;

	if (charID2 == Characters_Amy || charID2 == Characters_MetalSonic || co2->CharID == Characters_Shadow || !isSuper)
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
		njCalcPoint(&a1, &aura->pos, v4); //njcalcvector
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
		njCalcPoint(&result, &aura->pos, (float*)v6); //njCalcVector
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
		push edi // obj

		// Call your __cdecl function here:
		call DoHomingAura_r

		pop edi // obj
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

DataPointer(WORD, word_170ACEE, 0x170ACEE);
void SuperAura_r(ObjectMaster* obj) {

	SonicCharObj2* co2 = (SonicCharObj2*)obj->Data2.Undefined;
	EntityData1* data = obj->Data1.Entity;
	NJS_OBJECT* v24;

	if (co2->base.AnimInfo.Current == 54 || co2->base.AnimInfo.Next == 54)
	{
		obj->DisplaySub_Delayed4 = nullptr;
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

	njSetTexture(&SSONEFFTEX_TEXLIST);

	word_170ACEE = word_170ACEE & 0xC000 | ((co2->base.CharID != Characters_SuperShadow ? 0 : 6)
		+ 3);

	njPushMatrix(CURRENT_MATRIX);
	njTranslate(CURRENT_MATRIX, 0.0, -3.0, 0.0);
	sub_42D340();
	ProcessChunkModelsWithCallback((NJS_OBJECT*)0x170B47C, ProcessChunkModel);// Draw Aura
	njPopMatrix(1u);

	int v12 = 255 * (FrameCountIngame - 14400);
	int v13;
	int v14;
	int v15;
	int v16;


		v13 = v12 / 18000 + 128;
		if (v13 <= 255)
		{
			v14 = ((v13 | ((v13 | (v13 << 8)) << 8)) << 8) | v13;
			if (!v14)
			{
				njPopMatrix(1u);
			}
		}
		else
		{
			v14 = -1;
		}
		v15 = 49;
		v16 = 0;
		do
		{
			if ((*(unsigned int*)((char*)(void*)0x170CF1C + v16) & 0xFF000000) == -16777216)
			{
				*(void**)((char*)(void*)0x170B7AC + v16) = (void*)v14;
			}
			--v15;
			v16 += 16;
		} while (v15 > 0);
		*(DWORD*)(*(DWORD*)Has_texlist_batadvPlayerChara_in_it.gap0 + 32) = (DWORD)&SSONEFFTEX_TEXLIST;
		//DrawUV((int*)0x16F7C5C, (NJS_OBJECT*)0x70BAB4, *(float*)&FrameCountIngame);
		njPopMatrix(1u);

		njPushMatrixEx();

		float v23[12];

		njTranslate(CURRENT_MATRIX, 0, 0, 0);
		njRotateZ(CURRENT_MATRIX, 55296);
		njTranslate(CURRENT_MATRIX, 0.0, -14.0f, 0.0);
		v23[0] = 1.2;
		v23[1] = 0.0;
		v23[2] = 0.0;
		v23[3] = 0.0;
		v23[4] = 0.0;
		v23[5] = 1.3;
		v23[6] = 0.0;
		v23[7] = 0.0;
		v23[8] = 0.0;
		v23[9] = 0.0;
		v23[11] = 0.0;
		v23[10] = 1.2;
		//sub_426E40(v18, v17, v23);
		njTranslate(CURRENT_MATRIX, 0.0, 5.0, 0.0);
		sub_42D340();
		ProcessChunkModelsWithCallback((NJS_OBJECT*)0x170BACC, ProcessChunkModel);
		njPopMatrixEx();
	
}

void LoadSuperAura(char pID)
{
	MainCharacter[pID]->DisplaySub_Delayed4 = SuperAura_r;
	return;
}

//Serie of hack to make the aura display to a different texture when Super Sonic.
void initAura_Hack() {

	//regular sonic aura
	DoSpinDashAura_t = new Trampoline((int)0x7562A0, (int)0x7562A7, DoSpinDashAuraASM);
	DoJumpAura_t = new Trampoline((int)0x756AE0, (int)0x756AE5, DoJumpAuraASM);
	DoHomingAura_t = new Trampoline((int)0x7566C0, (int)0x7566C5, DoHomingAuraASM);
	HomingDashAura_Display_t = new Trampoline((int)0x757040, (int)0x757045, HomingDashAura_Display_r);

	return;
}