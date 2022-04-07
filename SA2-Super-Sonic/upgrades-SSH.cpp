#include "pch.h"

enum SuperShadowModel {
	SSHRoot = 355,
	SSHRoot2,
	SSHRoot3,
	SSHHead,
	SSHHead2 = 363,
	SSHRightFootToe,
	SSHLeftFootToe,
	SSHRightFootHeel,
	SSHLeftFootHeel,
	SSHRightHandParent = 370,
	SSHLeftHandParent,
	SSHRightArm,
	SSHLeftArm,
	SSHRightHandParent2 = 378,
	SSHLeftHandParent2,
};

static NJS_MATRIX SSH_RightHandMatrice;
static NJS_MATRIX SSH_LeftHandMatrice;
static NJS_MATRIX SSH_LeftFootMatrice;
static NJS_MATRIX SSH_RightFootMatrice;


void SSH_SetMatrixPosition(NJS_OBJECT* mdl)
{
	if (mdl == CharacterModels[SSHRightHandParent2].Model)
	{
		njSetMatrix(SSH_RightHandMatrice, CURRENT_MATRIX);
	}
	if (mdl == CharacterModels[SSHLeftHandParent2].Model)
	{
		njSetMatrix(SSH_LeftHandMatrice, CURRENT_MATRIX);
	}
	if (mdl == CharacterModels[SSHRightFootHeel].Model)
	{
		njSetMatrix(SSH_RightFootMatrice, CURRENT_MATRIX);
	}
	else if (mdl == CharacterModels[SSHLeftFootHeel].Model)
	{
		njSetMatrix(SSH_LeftFootMatrice, CURRENT_MATRIX);
	}
}

void SuperShadow_Callback_r(NJS_OBJECT* mdl) {

	NJS_MATRIX_PTR m = _nj_current_matrix_ptr_;
	SonicCharObj2* wk = SonicCO2PtrExtern;

	char pNum = wk->base.PlayerNum;
	char char2 = wk->base.CharID2;

	if (!isSuper[pNum] || char2 != Characters_Shadow)
	{
		return;
	}

	if (mdl == CharacterModels[SSHRoot2].Model || mdl == CharacterModels[71].Model->child)
	{
		NJS_VECTOR pt{};
		njCalcPoint_(m, &pt, &pt, FALSE);
		njCalcPoint_(MATRIX_1A51A00, (NJS_VECTOR*)&wk->head2_pos, &pt, 0);
		pt = { 1.0f, 0.0f, 0.0f };
		njCalcVector_(&pt, &pt, m);
		njCalcVector_(&pt, &wk->head2_vec, MATRIX_1A51A00);
		SSH_SetMatrixPosition(mdl);
		return;
	}
	if (mdl == CharacterModels[SSHRoot3].Model)
	{
		if (MainCharObj1[wk->base.PlayerNum]->Action == 1)
		{
			njRotateX(m, wk->base.TiltAngle);
		}
	}
	else
	{
		if (mdl != CharacterModels[SSHRightArm].Model)
		{
			if (mdl == CharacterModels[SSHLeftArm].Model)
			{

				if ((MainCharObj1[wk->base.PlayerNum]->Status & Status_HoldObject) != 0)
				{
					njRotateX(m, *((uint32_t*)wk->base.HeldObject->EntityData2 + 12));
				}
			}
			else if (mdl == CharacterModels[SSHHead].Model)
			{
				NJS_VECTOR pt{};
				njCalcPoint_(m, &pt, &pt, FALSE);
				njCalcPoint_(MATRIX_1A51A00, (NJS_VECTOR*)&wk->head0_pos, &pt, 0);
				pt = { 1.0f, 0.0f, 0.0f };
				njCalcVector_(&pt, &pt, m);
				njCalcVector_(&pt, &wk->head0_vec, MATRIX_1A51A00);
			}
			else if (mdl == CharacterModels[SSHHead2].Model)
			{
				NJS_VECTOR pt{};
				njCalcPoint_(m, &pt, &pt, FALSE);
				njCalcPoint_(MATRIX_1A51A00, (NJS_VECTOR*)&wk->head1_pos, &pt, 0);
				pt = { 1.0f, 0.0f, 0.0f };
				njCalcVector_(&pt, &pt, m);
				njCalcVector_(&pt, &wk->head1_vec, MATRIX_1A51A00);
			}
			else if (mdl == CharacterModels[SSHRightHandParent].Model)
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
			else if (mdl == CharacterModels[SSHLeftHandParent].Model)
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
			else if (mdl == CharacterModels[SSHRightFootToe].Model)
			{
				NJS_VECTOR pt{};
				njCalcPoint_(m, &pt, &pt, FALSE);
				njCalcPoint_(MATRIX_1A51A00, (NJS_VECTOR*)&wk->rightfoot_pos, &pt, 0);
				pt = { -1.0f, 0.0f, 0.0f };
				njCalcVector_(&pt, &pt, m);
				njCalcVector_(&pt, &wk->rightfoot_vec, MATRIX_1A51A00);
			}
			else if (mdl == CharacterModels[SSHLeftFootToe].Model)
			{
				NJS_VECTOR pt{};
				njCalcPoint_(m, &pt, &pt, FALSE);
				njCalcPoint_(MATRIX_1A51A00, (NJS_VECTOR*)&wk->leftfoot_pos, &pt, 0);
				pt = { -1.0f, 0.0f, 0.0f };
				njCalcVector_(&pt, &pt, m);
				njCalcVector_(&pt, &wk->leftfoot_vec, MATRIX_1A51A00);
			}
			SSH_SetMatrixPosition(mdl);
			return;
		}
		if ((MainCharObj1[wk->base.PlayerNum]->Status & Status_HoldObject) == 0)
		{
			SSH_SetMatrixPosition(mdl);
			return;
		}
	}
}

void DisplaySuperShadow_Upgrade(EntityData1* data1, SonicCharObj2* sonicCO2) {

	if (!sonicCO2 || !isUpgrade || AltCostume[sonicCO2->base.PlayerNum] != 0)
		return;

	int curAnim = sonicCO2->base.AnimInfo.Current;

	if (data1->Status & Status_Ball || curAnim == 30 || curAnim == 11 || curAnim == 12 || curAnim == 100)
		return;

	njSetTexture(getShadowTexlist());
	njPushMatrixEx();

	NJS_OBJECT* FlameRingMDL = CharacterModels[100].Model;

	int upgrade = sonicCO2->base.Upgrades;

	if ((upgrade & (Upgrades_ShadowFlameRing)) != 0)
	{
		if (FlameRingMDL)
		{
			NJS_VECTOR FRPos = { FlameRingMDL->child->pos[0], FlameRingMDL->child->pos[1], FlameRingMDL->child->pos[2] };
			memcpy(CURRENT_MATRIX, &SSH_LeftHandMatrice, 0x30u);
			DrawChunkModel(FlameRingMDL->getchunkmodel());// Display FlameRing Model on Shadow
			signed int v30 = dword_25F02D8;
			signed int v31 = *(DWORD*)&sonicCO2->field_36A[6];
			dword_1DEB6A4 = dword_25F02D8;

			SetMaterial(1, njCos(v31) * -1.0, 0, 0);
			njTranslateEx(&FRPos);
			DrawChunkModel(FlameRingMDL->child->getchunkmodel());// Display FlameRing animation on Shadow
			ResetMaterial();
			dword_25F02D8 = dword_1DEB6A4;
			sub_426420(8, 0);
			*(DWORD*)(*(DWORD*)Has_texlist_batadvPlayerChara_in_it.gap0 + 12) = *(DWORD*)(*(DWORD*)Has_texlist_batadvPlayerChara_in_it.gap0 + 12) & 0xF7FFFFFF | 0x14000000;
		}
	}

	if ((upgrade & Upgrades_ShadowAirShoes) != 0 && CharacterModels[79].Model)
	{
		NJS_OBJECT* ShoesMDL = CharacterModels[79].Model;
		if (ShoesMDL) {
			memcpy(CURRENT_MATRIX, &SSH_LeftFootMatrice, 0x30u);
			DrawObjWithCallBack(ShoesMDL);
			memcpy(CURRENT_MATRIX, &SSH_RightFootMatrice, 0x30u);
			DrawObjWithCallBack(CharacterModels[78].Model);
		}
	}

	njPopMatrixEx();
}