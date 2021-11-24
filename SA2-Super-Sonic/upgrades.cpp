#include "pch.h"

extern NJS_TEXLIST Sonic_Texlist;


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

void SuperSonic_Callback_r(NJS_OBJECT* mdl) {

    if (!isSuper)
        return;


    SonicCharObj2* co2; // esi
    NJS_VECTOR v3; // ecx
    NJS_VECTOR v4; // eax
    NJS_VECTOR v6; // ecx
    NJS_VECTOR v7; // eax
    NJS_VECTOR v9; // ecx
    NJS_VECTOR v10; // eax
    NJS_VECTOR v12; // ecx
    NJS_VECTOR v13; // eax
    NJS_VECTOR v14; // eax
    NJS_VECTOR v15; // eax
    NJS_VECTOR v17; // ecx
    NJS_VECTOR v18; // eax
    NJS_VECTOR v19; // eax
    NJS_VECTOR v20; // eax
    NJS_VECTOR v22; // ecx
    NJS_VECTOR v23; // eax
    SonicCharObj2* co2Copy; // esi
    NJS_VECTOR v25; // ecx
    NJS_VECTOR v26; // eax
    ObjectMaster* heldObj; // edx
    float v28; // eax
    NJS_MATRIX_PTR* ModelMatrice; // edi
    NJS_VECTOR result; // [esp+4h] [ebp-18h] BYREF
    NJS_VECTOR posResult; // [esp+10h] [ebp-Ch] BYREF

    char pNum = SonicCO2PtrExtern->base.PlayerNum;


    posResult.z = 0.0;
    posResult.y = 0.0;
    posResult.x = 0.0;


    if (mdl == CharacterModels[SSRoot2].Model || mdl == CharacterModels[6].Model->child) //6 is jumpball
    {
        result.x = 1.0;
        result.z = 0.0;
        result.y = 0.0;
        njCalcVector(CURRENT_MATRIX, &posResult, &posResult, 0);
        co2 = SonicCO2PtrExtern;
        njCalcVector(flt_1A51A00, (NJS_VECTOR*)&SonicCO2PtrExtern->gap224[12], &v3, 0);
        njCalcPoint(&result, &result, CURRENT_MATRIX);
        njCalcPoint(&v4, (NJS_VECTOR*)&co2->gap224[168], flt_1A51A00);
        goto LABEL_48;
    }
    if (mdl == CharacterModels[SSRoot3].Model)
    {
        if (MainCharObj1[pNum]->Action != 1)
        {
            goto LABEL_48;
        }
        v28 = *(float*)&SonicCO2PtrExtern->base.field_2C;
    }
    else
    {
        if (mdl != CharacterModels[SSRightArm].Model)
        {
            if (mdl == CharacterModels[SSLeftArm].Model)
            {
                if ((MainCharObj1[pNum]->Status & Status_HoldObject) != 0)
                {
                    heldObj = SonicCO2PtrExtern->base.HeldObject;
                    if (heldObj->EntityData2->field_30)
                    {
                        njRotateX(CURRENT_MATRIX, -(heldObj->EntityData2->field_30));
                    }
                }
            }
            else if (mdl == CharacterModels[SSHead].Model)
            {
                result.x = 1.0;
                result.z = 0.0;
                result.y = 0.0;
                njCalcVector(CURRENT_MATRIX, &posResult, &posResult, 0);
                co2Copy = SonicCO2PtrExtern;
                njCalcVector(flt_1A51A00, &SonicCO2PtrExtern->HeadNodePos, &v25, 0);
                njCalcPoint(&result, &result, CURRENT_MATRIX);
                njCalcPoint(&v26, (NJS_VECTOR*)&co2Copy->gap224[144], flt_1A51A00);
            }
            else if (mdl == CharacterModels[SSHead2].Model)
            {
                result.x = 1.0;
                result.z = 0.0;
                result.y = 0.0;
                njCalcVector(CURRENT_MATRIX, &posResult, &posResult, 0);
                co2Copy = SonicCO2PtrExtern;
                njCalcVector(flt_1A51A00, (NJS_VECTOR*)SonicCO2PtrExtern->gap224, &v22, 0);
                njCalcPoint(&result, &result, CURRENT_MATRIX);
                njCalcPoint(&v23, (NJS_VECTOR*)&co2Copy->gap224[156], flt_1A51A00);
            }
            else if (mdl == CharacterModels[SSRightHandParent].Model)
            {
                njCalcVector(CURRENT_MATRIX, &posResult, &posResult, 0);
                co2Copy = SonicCO2PtrExtern;
                njCalcVector(flt_1A51A00, (NJS_VECTOR*)&SonicCO2PtrExtern->gap1BC[40], &v17, 0);
                result.z = 1.0;
                result.y = 0.0;
                result.x = 0.0;
                njCalcPoint(&result, &result, CURRENT_MATRIX);
                njCalcPoint(&v18, (NJS_VECTOR*)&co2Copy->gap224[72], flt_1A51A00);
                result.y = -1.0;
                result.x = 0.0;
                result.z = 0.0;
                njCalcPoint(&v19, &v19, CURRENT_MATRIX);
                njCalcPoint(&v20, (NJS_VECTOR*)&co2Copy->gap224[96], flt_1A51A00);
            }
            else if (mdl == CharacterModels[SSLeftHandParent].Model)
            {
                njCalcVector(CURRENT_MATRIX, &posResult, &posResult, 0);
                co2Copy = SonicCO2PtrExtern;
                njCalcVector(flt_1A51A00, (NJS_VECTOR*)&SonicCO2PtrExtern->gap1BC[52], &v12, 0);
                result.z = -1.0;
                result.y = 0.0;
                result.x = 0.0;
                njCalcPoint(&result, &result, CURRENT_MATRIX);
                njCalcPoint(&v13, (NJS_VECTOR*)&co2Copy->gap224[84], flt_1A51A00);
                result.y = -1.0;
                result.x = 0.0;
                result.z = 0.0;
                njCalcPoint(&v14, &v14, CURRENT_MATRIX);
                njCalcPoint(&v15, (NJS_VECTOR*)&co2Copy->gap224[108], flt_1A51A00);
            }
            else if (mdl == CharacterModels[SSRightFootToe].Model)
            {
                result.x = -1.0;
                result.z = 0.0;
                result.y = 0.0;
                njCalcVector(CURRENT_MATRIX, &posResult, &posResult, 0);
                co2Copy = SonicCO2PtrExtern;
                njCalcVector(flt_1A51A00, (NJS_VECTOR*)&SonicCO2PtrExtern->gap1BC[64], &v9, 0);
                njCalcPoint(&result, &result, CURRENT_MATRIX);
                njCalcPoint(&v10, (NJS_VECTOR*)&co2Copy->gap224[120], flt_1A51A00);
            }
            else if (mdl == CharacterModels[SSLeftFootToe].Model)
            {
                result.x = -1.0;
                result.z = 0.0;
                result.y = 0.0;
                njCalcVector(CURRENT_MATRIX, &posResult, &posResult, 0);
                co2Copy = SonicCO2PtrExtern;
                njCalcVector(flt_1A51A00, (NJS_VECTOR*)&SonicCO2PtrExtern->gap1BC[76], &v6, 0);
                njCalcPoint(&result, &result, CURRENT_MATRIX);
                njCalcPoint(&v7, (NJS_VECTOR*)&co2Copy->gap224[132], flt_1A51A00);
            }
            goto LABEL_48;
        }
        if ((MainCharObj1[pNum]->Status & Status_HoldObject) == 0)
        {
            goto LABEL_48;
        }
        v28 = SonicCO2PtrExtern->base.HeldObject->EntityData2->field_30;
    }
    if (v28 != 0.0)
    {
        njRotateX(CURRENT_MATRIX, v28);
    }
LABEL_48:
    if (mdl == CharacterModels[SSRightHandParent2].Model)
    {
        ModelMatrice = (NJS_MATRIX_PTR*)&SS_RightHandMatrice;
        goto LABEL_56;
    }
    if (mdl == CharacterModels[SSLeftHandParent2].Model)
    {
        ModelMatrice = (NJS_MATRIX_PTR*)&SS_LeftHandMatrice;
    LABEL_56:
        memcpy(ModelMatrice, CURRENT_MATRIX, 0x30u);
    }
    if (mdl == CharacterModels[SSRightFootHeel].Model)
    {
        memcpy(&SS_RightFootMatrice, CURRENT_MATRIX, 0x30u);
    }
    else if (mdl == CharacterModels[SSLeftFootHeel].Model)
    {
        memcpy(&SS_LeftFootMatrice, CURRENT_MATRIX, 0x30u);
    }
}

DataPointer(int, dword_25F02D8, 0x25F02D8);

DataPointer(int, dword_1DEB6A4, 0x1DEB6A4);



void DisplaySuperSonic_Upgrade(SonicCharObj2* sonicCO2) {

    if (!sonicCO2)
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



DataPointer(NJS_MATRIX, sonic_RightHandMatrix, 0x1A51A3C);
DataPointer(NJS_MATRIX, sonic_LeftHandMatrix, 0x1A51AA0);
DataPointer(NJS_MATRIX, sonic_LeftFootMatrix, 0x1A51A6C);
DataPointer(NJS_MATRIX, sonic_RightFootMatrix, 0x1A519D0);


void Sonic_Callback_r(NJS_OBJECT* mdl) {


    NJS_MATRIX_PTR* matrice; // ebx
    SonicCharObj2* co2; // esi
    NJS_VECTOR v3; // ecx
    NJS_VECTOR v4; // eax
    NJS_VECTOR v6; // ecx
    NJS_VECTOR v7; // eax
    NJS_VECTOR v9; // ecx
    NJS_VECTOR v10; // eax
    NJS_VECTOR v12; // ecx
    NJS_VECTOR v13; // eax
    NJS_VECTOR v14; // eax
    NJS_VECTOR v15; // eax
    NJS_VECTOR v17; // ecx
    NJS_VECTOR v18; // eax
    NJS_VECTOR v19; // eax
    NJS_VECTOR v20; // eax
    NJS_VECTOR v22; // ecx
    NJS_VECTOR v23; // eax
    SonicCharObj2* co2Copy; // esi
    NJS_VECTOR v25; // ecx
    NJS_VECTOR v26; // eax
    ObjectMaster* heldObj; // edx
    float v28; // eax
    NJS_MATRIX_PTR* ModelMatrice; // edi
    NJS_VECTOR result; // [esp+4h] [ebp-18h] BYREF
    NJS_VECTOR posResult; // [esp+10h] [ebp-Ch] BYREF

    char pNum = SonicCO2PtrExtern->base.PlayerNum;

    matrice = &_nj_current_matrix_ptr_;
    posResult.z = 0.0;
    posResult.y = 0.0;
    posResult.x = 0.0;


    if (mdl == CharacterModels[1].Model || mdl == CharacterModels[6].Model->child) //6 is jumpball
    {
        result.x = 1.0;
        result.z = 0.0;
        result.y = 0.0;
        njCalcVector(CURRENT_MATRIX, &posResult, &posResult, 0);
        co2 = SonicCO2PtrExtern;
        njCalcVector(flt_1A51A00, (NJS_VECTOR*)&SonicCO2PtrExtern->gap224[12], &v3, 0);
        njCalcPoint(&result, &result, (float*)matrice);
        njCalcPoint(&v4, (NJS_VECTOR*)&co2->gap224[168], flt_1A51A00);
        goto LABEL_48;
    }
    if (mdl == CharacterModels[2].Model)
    {
        if (MainCharObj1[pNum]->Action != 1)
        {
            goto LABEL_48;
        }
        v28 = *(float*)&SonicCO2PtrExtern->base.field_2C;
    }
    else
    {
        if (mdl != CharacterModels[17].Model)
        {
            if (mdl == CharacterModels[18].Model)
            {
                if ((MainCharObj1[pNum]->Status & Status_HoldObject) != 0)
                {
                    heldObj = SonicCO2PtrExtern->base.HeldObject;
                    if (heldObj->EntityData2->field_30)
                    {
                        njRotateX(CURRENT_MATRIX, -(heldObj->EntityData2->field_30));
                    }
                }
            }
            else if (mdl == CharacterModels[3].Model)
            {
                result.x = 1.0;
                result.z = 0.0;
                result.y = 0.0;
                njCalcVector(CURRENT_MATRIX, &posResult, &posResult, 0);
                co2Copy = SonicCO2PtrExtern;
                njCalcVector(flt_1A51A00, &SonicCO2PtrExtern->HeadNodePos, &v25, 0);
                njCalcPoint(&result, &result, (float*)matrice);
                njCalcPoint(&v26, (NJS_VECTOR*)&co2Copy->gap224[144], flt_1A51A00);
            }
            else if (mdl == CharacterModels[8].Model)
            {
                result.x = 1.0;
                result.z = 0.0;
                result.y = 0.0;
                njCalcVector(CURRENT_MATRIX, &posResult, &posResult, 0);
                co2Copy = SonicCO2PtrExtern;
                njCalcVector(flt_1A51A00, (NJS_VECTOR*)SonicCO2PtrExtern->gap224, &v22, 0);
                njCalcPoint(&result, &result, (float*)matrice);
                njCalcPoint(&v23, (NJS_VECTOR*)&co2Copy->gap224[156], flt_1A51A00);
            }
            else if (mdl == CharacterModels[15].Model)
            {
                njCalcVector(CURRENT_MATRIX, &posResult, &posResult, 0);
                co2Copy = SonicCO2PtrExtern;
                njCalcVector(flt_1A51A00, (NJS_VECTOR*)&SonicCO2PtrExtern->gap1BC[40], &v17, 0);
                result.z = 1.0;
                result.y = 0.0;
                result.x = 0.0;
                njCalcPoint(&result, &result, (float*)matrice);
                njCalcPoint(&v18, (NJS_VECTOR*)&co2Copy->gap224[72], flt_1A51A00);
                result.y = -1.0;
                result.x = 0.0;
                result.z = 0.0;
                njCalcPoint(&v19, &v19, (float*)matrice);
                njCalcPoint(&v20, (NJS_VECTOR*)&co2Copy->gap224[96], flt_1A51A00);
            }
            else if (mdl == CharacterModels[16].Model)
            {
                njCalcVector(CURRENT_MATRIX, &posResult, &posResult, 0);
                co2Copy = SonicCO2PtrExtern;
                njCalcVector(flt_1A51A00, (NJS_VECTOR*)&SonicCO2PtrExtern->gap1BC[52], &v12, 0);
                result.z = -1.0;
                result.y = 0.0;
                result.x = 0.0;
                njCalcPoint(&result, &result, (float*)matrice);
                njCalcPoint(&v13, (NJS_VECTOR*)&co2Copy->gap224[84], flt_1A51A00);
                result.y = -1.0;
                result.x = 0.0;
                result.z = 0.0;
                njCalcPoint(&v14, &v14, (float*)matrice);
                njCalcPoint(&v15, (NJS_VECTOR*)&co2Copy->gap224[108], flt_1A51A00);
            }
            else if (mdl == CharacterModels[9].Model)
            {
                result.x = -1.0;
                result.z = 0.0;
                result.y = 0.0;
                njCalcVector(CURRENT_MATRIX, &posResult, &posResult, 0);
                co2Copy = SonicCO2PtrExtern;
                njCalcVector(flt_1A51A00, (NJS_VECTOR*)&SonicCO2PtrExtern->gap1BC[64], &v9, 0);
                njCalcPoint(&result, &result, (float*)matrice);
                njCalcPoint(&v10, (NJS_VECTOR*)&co2Copy->gap224[120], flt_1A51A00);
            }
            else if (mdl == CharacterModels[10].Model)
            {
                result.x = -1.0;
                result.z = 0.0;
                result.y = 0.0;
                njCalcVector(CURRENT_MATRIX, &posResult, &posResult, 0);
                co2Copy = SonicCO2PtrExtern;
                njCalcVector(flt_1A51A00, (NJS_VECTOR*)&SonicCO2PtrExtern->gap1BC[76], &v6, 0);
                njCalcPoint(&result, &result, (float*)matrice);
                njCalcPoint(&v7, (NJS_VECTOR*)&co2Copy->gap224[132], flt_1A51A00);
            }
            goto LABEL_48;
        }
        if ((MainCharObj1[pNum]->Status & Status_HoldObject) == 0)
        {
            goto LABEL_48;
        }
        v28 = SonicCO2PtrExtern->base.HeldObject->EntityData2->field_30;
    }
    if (v28 != 0.0)
    {
        njRotateX(CURRENT_MATRIX, v28);
    }
LABEL_48:
    if (mdl == CharacterModels[23].Model)
    {
        ModelMatrice = (NJS_MATRIX_PTR*)&sonic_RightHandMatrix;
        goto LABEL_56;
    }
    if (mdl == CharacterModels[24].Model)
    {
        ModelMatrice = (NJS_MATRIX_PTR*)&sonic_LeftHandMatrix;
    LABEL_56:
        memcpy(ModelMatrice, CURRENT_MATRIX, 0x30u);
    }
    if (mdl == CharacterModels[11].Model)
    {
        memcpy(&sonic_RightFootMatrix, CURRENT_MATRIX, 0x30u);
    }
    else if (mdl == CharacterModels[12].Model)
    {
        memcpy(&sonic_LeftFootMatrix, CURRENT_MATRIX, 0x30u);
    }
}
