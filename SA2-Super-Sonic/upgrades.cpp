#include "pch.h"

extern NJS_TEXLIST Sonic_Texlist;
Trampoline* Sonic_CallBack_t;

enum SuperSonicModel {
	SSRoot = 328,
	SSRoot2,
	SSRoot3,
	SSHead = 331,
	SSHead2 = 336,
	SSRightFootToe,
	SSLeftFootToe,
	SSRightFootHeel,
	SSLeftFootHeel,
	SSRightHandParent = 343,
	SSLeftHandParent,
	SSRightArm,
	SSLeftArm,
	SSRightHandParent2 = 351,
	SSLeftHandParent2,
};

static NJS_MATRIX SS_RightHandMatrice;
static NJS_MATRIX SS_LeftHandMatrice;
static NJS_MATRIX SS_LeftFootMatrice;
static NJS_MATRIX SS_RightFootMatrice;

void SS_SetMatrixPosition(NJS_OBJECT* mdl)
{
	if (mdl == CharacterModels[SSRightHandParent2].Model)
	{
		njSetMatrix(SS_RightHandMatrice, CURRENT_MATRIX);
	}
	if (mdl == CharacterModels[SSLeftHandParent2].Model)
	{
		njSetMatrix(SS_LeftHandMatrice, CURRENT_MATRIX);
	}
	if (mdl == CharacterModels[SSRightFootHeel].Model)
	{
		njSetMatrix(SS_RightFootMatrice, CURRENT_MATRIX);
	}
	else if (mdl == CharacterModels[SSLeftFootHeel].Model)
	{
		njSetMatrix(SS_LeftFootMatrice, CURRENT_MATRIX);
	}
}


void SuperSonic_Callback_r(NJS_OBJECT* mdl) {

	NJS_MATRIX_PTR m = _nj_current_matrix_ptr_;
	SonicCharObj2* wk = SonicCO2PtrExtern;

	char pNum = wk->base.PlayerNum;
	char char2 = wk->base.CharID2;

	if (!isSuper || char2 != Characters_Sonic)
	{
		return;
	}

	if (mdl == CharacterModels[SSRoot2].Model || mdl == CharacterModels[6].Model->child)
	{
		NJS_VECTOR pt{};
		njCalcPoint_(m, &pt, &pt, FALSE);
		njCalcPoint_(MATRIX_1A51A00, (NJS_VECTOR*)&wk->head2_pos, &pt, 0);
		pt = { 1.0f, 0.0f, 0.0f };
		njCalcVector_(&pt, &pt, m);
		njCalcVector_(&pt, &wk->head2_vec, MATRIX_1A51A00);
		SS_SetMatrixPosition(mdl);
		return;
	}
	if (mdl == CharacterModels[SSRoot3].Model)
	{
		if (MainCharObj1[wk->base.PlayerNum]->Action == 1)
		{
			njRotateX(m, wk->base.TiltAngle);
		}
	}
	else
	{
		if (mdl != CharacterModels[SSRightArm].Model)
		{
			if (mdl == CharacterModels[SSLeftArm].Model)
			{

				if ((MainCharObj1[wk->base.PlayerNum]->Status & Status_HoldObject) != 0)
				{
					njRotateX(m, *((uint32_t*)wk->base.HeldObject->EntityData2 + 12));
				}
			}
			else if (mdl == CharacterModels[SSHead].Model)
			{
				NJS_VECTOR pt{};
				njCalcPoint_(m, &pt, &pt, FALSE);
				njCalcPoint_(MATRIX_1A51A00, (NJS_VECTOR*)&wk->head0_pos, &pt, 0);
				pt = { 1.0f, 0.0f, 0.0f };
				njCalcVector_(&pt, &pt, m);
				njCalcVector_(&pt, &wk->head0_vec, MATRIX_1A51A00);
			}
			else if (mdl == CharacterModels[SSHead2].Model)
			{
				NJS_VECTOR pt{};
				njCalcPoint_(m, &pt, &pt, FALSE);
				njCalcPoint_(MATRIX_1A51A00, (NJS_VECTOR*)&wk->head1_pos, &pt, 0);
				pt = { 1.0f, 0.0f, 0.0f };
				njCalcVector_(&pt, &pt, m);
				njCalcVector_(&pt, &wk->head1_vec, MATRIX_1A51A00);
			}
			else if (mdl == CharacterModels[SSRightHandParent].Model)
			{
				NJS_VECTOR pt{};
				njCalcPoint_(m, &pt, &pt, FALSE);
				njCalcPoint_(MATRIX_1A51A00, (NJS_VECTOR*)&wk->righthand_pos, &pt, 0);
				pt = { 0.0f, 0.0f, 1.0f };
				njCalcVector_(&pt, &pt, m);
				njCalcVector_(&pt, &wk->righthand_vec0, MATRIX_1A51A00);
				pt = { 0.0f, -1.0f, 0.0f };
				njCalcVector_(&pt, &pt, m);
				njCalcVector_(&pt, &wk->righthand_vec1, MATRIX_1A51A00);
			}
			else if (mdl == CharacterModels[SSLeftHandParent].Model)
			{
				NJS_VECTOR pt{};
				njCalcPoint_(m, &pt, &pt, FALSE);
				njCalcPoint_(MATRIX_1A51A00, (NJS_VECTOR*)&wk->lefthand_pos, &pt, 0);
				pt = { 0.0f, 0.0f, -1.0f };
				njCalcVector_(&pt, &pt, m);
				njCalcVector_(&pt, &wk->lefthand_vec0, MATRIX_1A51A00);
				pt = { 0.0f, -1.0f, 0.0f };
				njCalcVector_(&pt, &pt, m);
				njCalcVector_(&pt, &wk->lefthand_vec1, MATRIX_1A51A00);
			}
			else if (mdl == CharacterModels[SSRightFootToe].Model)
			{
				NJS_VECTOR pt{};
				njCalcPoint_(m, &pt, &pt, FALSE);
				njCalcPoint_(MATRIX_1A51A00, (NJS_VECTOR*)&wk->rightfoot_pos, &pt, 0);
				pt = { -1.0f, 0.0f, 0.0f };
				njCalcVector_(&pt, &pt, m);
				njCalcVector_(&pt, &wk->rightfoot_vec, MATRIX_1A51A00);
			}
			else if (mdl == CharacterModels[SSLeftFootToe].Model)
			{
				NJS_VECTOR pt{};
				njCalcPoint_(m, &pt, &pt, FALSE);
				njCalcPoint_(MATRIX_1A51A00, (NJS_VECTOR*)&wk->leftfoot_pos, &pt, 0);
				pt = { -1.0f, 0.0f, 0.0f };
				njCalcVector_(&pt, &pt, m);
				njCalcVector_(&pt, &wk->leftfoot_vec, MATRIX_1A51A00);
			}
			SS_SetMatrixPosition(mdl);
			return;
		}
		if ((MainCharObj1[wk->base.PlayerNum]->Status & Status_HoldObject) == 0)
		{
			SS_SetMatrixPosition(mdl);
			return;
		}
	}
}


void DisplaySuperSonic_Upgrade(SonicCharObj2* sonicCO2) {

	if (!sonicCO2 || !isUpgrade)
		return;

	int curAnim = sonicCO2->base.AnimInfo.Current;

	if (curAnim == 30 || curAnim == 11 || curAnim == 12 || curAnim == 100)
		return;

	njSetTexture(&Sonic_Texlist);
	njPushMatrixEx();

	NJS_OBJECT* FlameRingMDL = CharacterModels[25].Model;
	NJS_VECTOR FRPos = { FlameRingMDL->child->pos[0], FlameRingMDL->child->pos[1], FlameRingMDL->child->pos[2] };
	int upgrade = sonicCO2->base.Upgrades;

	if ((upgrade & (Upgrades_SonicFlameRing)) != 0)
	{
		if (FlameRingMDL)
		{
			memcpy(CURRENT_MATRIX, &SS_LeftHandMatrice, 0x30u);
			DrawChunkModel(FlameRingMDL->getchunkmodel());// Display FlameRing Model on Sonic
			signed int v30 = dword_25F02D8;
			signed int v31 = *(DWORD*)&sonicCO2->field_36A[6];
			dword_1DEB6A4 = dword_25F02D8;

			SetMaterial(1, njCos(v31) * -1.0, 0, 0);
			njTranslateEx(&FRPos);
			DrawChunkModel(FlameRingMDL->child->getchunkmodel());// Display FlameRing animation on Sonic
			ResetMaterial();
			dword_25F02D8 = dword_1DEB6A4;
			sub_426420(8, 0);
			*(DWORD*)(*(DWORD*)Has_texlist_batadvPlayerChara_in_it.gap0 + 12) = *(DWORD*)(*(DWORD*)Has_texlist_batadvPlayerChara_in_it.gap0 + 12) & 0xF7FFFFFF | 0x14000000;
		}
	}
	if ((upgrade & Upgrades_SonicBounceBracelet) != 0)
	{
		NJS_OBJECT* BounceMDL = CharacterModels[26].Model;

		if (BounceMDL)
		{
			memcpy(CURRENT_MATRIX, &SS_RightHandMatrice, 0x30u);
			DrawChunkModel(BounceMDL->getchunkmodel());
			signed int v36 = *(DWORD*)&sonicCO2->field_36A[10];
			SetMaterial(1, njCos(v36) * -1.0, 0, 0);
			NJS_OBJECT* BounceChild = CharacterModels[26].Model->child;
			njTranslateEx((NJS_VECTOR*)BounceChild->pos);

			if (BounceChild->ang[2])
			{
				njRotateZ((float*)_nj_current_matrix_ptr_, BounceChild->ang[2]);
			}
			if (BounceChild->ang[1])
			{
				njRotateY(CURRENT_MATRIX, BounceChild->ang[1]);
			}
			if (BounceChild->ang[0])
			{
				njRotateX(CURRENT_MATRIX, BounceChild->ang[0]);
			}
			DrawChunkModel(BounceChild->getchunkmodel());
			ResetMaterial();
		}
	}
	if ((upgrade & Upgrades_SonicMagicGloves) != 0 && CharacterModels[27].Model)
	{
		NJS_OBJECT* GloveMDL = CharacterModels[27].Model;
		memcpy(CURRENT_MATRIX, &SS_RightHandMatrice, 0x30u);
		DrawObjWithCallBack(GloveMDL);
	}
	if ((upgrade & Upgrades_SonicLightShoes) != 0 && CharacterModels[14].Model)
	{
		NJS_OBJECT* ShoesMDL = CharacterModels[14].Model;
		memcpy(CURRENT_MATRIX, &SS_LeftFootMatrice, 0x30u);
		DrawObjWithCallBack(ShoesMDL);
		memcpy(CURRENT_MATRIX, &SS_RightFootMatrice, 0x30u);
		DrawObjWithCallBack(CharacterModels[13].Model);
	}

	njPopMatrixEx();
}