#include "pch.h"

const int chaosControlCDTimer = 2400; //40 seconds
int chaosControlCD[2] = { chaosControlCDTimer + 1};
bool isChaosControlEnabled[2] = { false, false };
const int chaosControlDuration = 1200;
int chaosControlTimer = 0;

TaskHook sub_724A50_t(0x724A50);
TaskHook Sonic2PTimeStopMan_t(Sonic2PTimeStopMan);

TaskHook seagull_t(0x6F6070);
TaskHook mhcont_t(0x6F9120);
TaskHook mhcont0_t(0x6F86E0);
TaskHook mhMissile_t(0x6F4260);
TaskHook CE_Truck_t(0x5E5030);

void __cdecl Sonic2PTimeStopMan_Load_r(CharObj2Base* co2)
{
	char pnum = co2->PlayerNum;

	TimeStopped |= 1 << 1;

	auto obj = AllocateObjectMaster(Sonic2PTimeStopMan, 2, "Sonic2PTimeStopMan");
	if (obj)
	{
		if (!isSuper[pnum])
		{
			DeleteObject_(obj);
			return;
		}

		auto objData = (CharObj2Base*)AllocateObjUnknownB();
		if (objData)
		{
			obj->Data2.Undefined = objData;
			objData->PlayerNum = pnum;
			objData->CharID = 0;
			objData->Costume = 1;
			objData->ActionWindowItems[0] = 9882;
			objData->ActionWindowItems[1] = 7;

			obj->DisplaySub_Delayed3 = ChaosControl_Display;
			obj->DeleteSub = (ObjectFuncPtr)0x724A50;
		}
		else
		{
			DeleteObject_(obj);
			obj = 0;
		}
	}

	LoadChaosControlEffect(co2->PlayerNum);

	if (co2->CharID2)
	{
		if (co2->CharID2 == 1)
		{
			PlaySoundProbably(8205, 0, 0, 0);
			PlayVoice(2, 1524);
			PoseEffectMan_Load_(co2->PlayerNum, 182);
		}
	}
	else
	{
		Play3DSound_Pos(8211, &MainCharObj1[0]->Position, 1, 0, 600);

		PlayVoice(2, 1525);
		PoseEffectMan_Load_(co2->PlayerNum, 118);
	}
}

void ResetChaosControl(char pnum)
{
	isChaosControlEnabled[pnum] = false;
	chaosControlCD[pnum] = 0;
	chaosControlTimer = 0;
	TimeStopped = 0;
}

void Check_ChaosControlCD(CharObj2Base* co2)
{
	if (!co2 || isChaosControlEnabled[co2->PlayerNum])
		return;

	char pnum = co2->PlayerNum;

	if (chaosControlCD[pnum] == chaosControlCDTimer)
	{
		SendTimedMessage("CHAOS CONTROL READY!", 120);
		chaosControlCD[pnum]++;
		return;
	}
	else if (chaosControlCD[pnum] < chaosControlCDTimer)
	{
		chaosControlCD[pnum]++;
	}
	else 
	{
		if (!isChaosControlEnabled[0] && !isChaosControlEnabled[1] && Controllers[pnum].on & Buttons_Y && Controllers[pnum].press & Buttons_X)
		{
			MainCharObj1[pnum]->Status &= ~Status_Ball;
			chaosControlCD[pnum] = 0;
			isChaosControlEnabled[pnum] = true;
			Sonic2PTimeStopMan_Load_r(co2);
			PlayJingle("sng_jin_ccslow.adx");
		}
	}
}

void ChaosControl_ManageDuration(CharObj2Base* co2)
{
	if (!co2 || !isChaosControlEnabled[co2->PlayerNum])
		return;

	if (chaosControlTimer < chaosControlDuration)
	{
		if (Controllers[co2->PlayerNum].on & Buttons_L && Controllers[co2->PlayerNum].on & Buttons_R)
			chaosControlTimer = chaosControlDuration;

		chaosControlTimer++;
	}
	else
	{
		StopMusic();
		ResetChaosControl(co2->PlayerNum);

		if (!TwoPlayerMode)
			ReleaseCamera(CameraData[0].currentCameraSlot, 0);

		return;
	}
}

void ChaosControl_Management(CharObj2Base* co2)
{
	Check_ChaosControlCD(co2);
	ChaosControl_ManageDuration(co2);
}

void __cdecl sub_724A50_r(ObjectMaster* a1)
{
	char pid = *((unsigned __int8*)a1->Data2.Undefined + 1);

	if (!TwoPlayerMode)
	{
		if (isChaosControlEnabled[pid])
			TimeStopped &= ~(1 << pid);

		if (MainCharObj2[pid] && !MainCharObj2[pid]->CharID)
			sub_437E10(a1);

		return;
	}

	sub_724A50_t.Original(a1);
}

void ChaosControl_FixCamPose(ObjectMaster* obj)
{
	if (!TwoPlayerMode)
		ReleaseCamera(CameraData[0].currentCameraSlot, 0);

	DeleteObject_(obj);
}

void __cdecl seagull_r(ObjectMaster* a1)
{
	if (TimeStopped != 0)
		return;

	seagull_t.Original(a1);
}

void __cdecl mhcont_r(ObjectMaster* a1)
{
	if (TimeStopped != 0)
		return;

	mhcont_t.Original(a1);
}

void __cdecl mhcont0_r(ObjectMaster* a1)
{
	if (TimeStopped != 0)
		return;

	mhcont0_t.Original(a1);
}

void __cdecl mhmissile_r(ObjectMaster* a1)
{
	if (TimeStopped != 0)
		return;

	mhMissile_t.Original(a1);
}

void __cdecl CETruck_r(ObjectMaster* a1)
{
	if (TimeStopped != 0)
		return;

	CE_Truck_t.Original(a1);
}

void __cdecl Sonic2PTimeStopMan_r(ObjectMaster* a1)
{

	auto co2 = a1->Data2.Character;
	int pnum = (unsigned __int8)co2->CharID; //this is actually player num in this scenario

	if (!TwoPlayerMode && (!isSuper[pnum] || chaosControlTimer >= chaosControlDuration))
	{
		a1->MainSub = DeleteObject_;
		return;
	}

	Sonic2PTimeStopMan_t.Original(a1);
}

void RenderNumber_r(const Number* a1)
{
	if (!TwoPlayerMode)
		return;

	RenderNumber(a1);
	return;
}

static void __declspec(naked) RenderNumberASM()
{
	__asm
	{
		push eax
		call RenderNumber_r
		pop eax
		retn
	}
}

void PatchChaosSpearTextures(NJS_OBJECT* object, int(__cdecl* callback)(NJS_CNK_MODEL*))
{
	NJS_TEXLIST* tex = getShadowTexlist();

	if (tex)
		njSetTexture(tex);

	ProcessChunkModelsWithCallback(object, callback);
}

void PatchSonicWindTextures(NJS_OBJECT* object, int(__cdecl* callback)(NJS_CNK_MODEL*))
{
	NJS_TEXLIST* tex = getSonicTexlist();

	if (tex)
		njSetTexture(tex);

	ProcessChunkModelsWithCallback(object, callback);
}

void initChaosControl_Hack()
{
	if (!AllowSuperAttacks)
		return;

	//fix crash when 2P doesn't exist
	sub_724A50_t.Hook(sub_724A50_r);

	//reset cam
	WriteCall((void*)0x47746F, ChaosControl_FixCamPose);
	WriteCall((void*)0x477445, ChaosControl_FixCamPose);
	WriteCall((void*)0x4773e9, ChaosControl_FixCamPose);

	//freeze time on objects
	seagull_t.Hook(seagull_r);
	mhcont_t.Hook(mhcont_r);
	mhcont0_t.Hook(mhcont0_r);
	mhMissile_t.Hook(mhmissile_r);
	CE_Truck_t.Hook(CETruck_r);

	WriteCall((void*)0x724A2A, RenderNumberASM);
	Sonic2PTimeStopMan_t.Hook(Sonic2PTimeStopMan_r);

	//patches 2P
	WriteCall((void*)0x75BCDC, PatchChaosSpearTextures);	
	WriteCall((void*)0x75AF90, PatchSonicWindTextures);
}