#include "pch.h"

const int chaosControlCDTimer = 3600; //1:00
int chaosControlCD[2] = { 0, 0 };
bool isChaosControlEnabled[2] = { false, false };
const int chaosControlDuration = 1200;
int chaosControlTimer = 0;

Trampoline* sub_724A50_t = nullptr;
Trampoline* Sonic2PTimeStopMan_t = nullptr;

Trampoline* seagull_t = nullptr;
Trampoline* mhcont_t = nullptr;
Trampoline* mhcont0_t = nullptr;
Trampoline* mhMissile_t = nullptr;

void __cdecl Sonic2PTimeStopMan_Load_r(CharObj2Base* co2)
{
	ObjectMaster* obj;
	CharObj2Base* objData;
	int v5;

	char pnum = co2->PlayerNum;

	TimeStopped |= 1 << 1;

	obj = AllocateObjectMaster(Sonic2PTimeStopMan, 2, "Sonic2PTimeStopMan");
	if (obj)
	{
		if (!isSuper[pnum])
		{
			DeleteObject_(obj);
			return;
		}

		objData = (CharObj2Base*)AllocateObjUnknownB();
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

	if (RingCount[pnum] >= 60 && chaosControlCD[pnum] == chaosControlCDTimer)
	{
		SendTimedMessage("CHAOS CONTROL READY!", 120);
		chaosControlCD[pnum]++;
		return;
	}
	else if (chaosControlCD[pnum] < chaosControlCDTimer)
	{
		chaosControlCD[pnum]++;
	}
	else {

		if (RingCount[pnum] >= 60 && !isChaosControlEnabled[0] && !isChaosControlEnabled[1] && Controllers[pnum].on & Buttons_Y && Controllers[pnum].press & Buttons_X)
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
			ResetCam(CameraData.gap1AC[168], 0);

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

	auto original = reinterpret_cast<decltype(sub_724A50_r)*>(sub_724A50_t->Target());
	original(a1);
}

void ChaosControl_FixCamPose(ObjectMaster* obj)
{
	if (!TwoPlayerMode)
		ResetCam(CameraData.gap1AC[168], 0);

	DeleteObject_(obj);
}

void __cdecl seagull_r(ObjectMaster* a1)
{
	if (TimeStopped != 0)
		return;

	auto original = reinterpret_cast<decltype(seagull_r)*>(seagull_t->Target());
	original(a1);
}

void __cdecl mhcont_r(ObjectMaster* a1)
{
	if (TimeStopped != 0)
		return;

	auto original = reinterpret_cast<decltype(mhcont_r)*>(mhcont_t->Target());
	original(a1);
}

void __cdecl mhcont0_r(ObjectMaster* a1)
{
	if (TimeStopped != 0)
		return;

	auto original = reinterpret_cast<decltype(mhcont0_r)*>(mhcont0_t->Target());
	original(a1);
}

void __cdecl mhmissile_r(ObjectMaster* a1)
{
	if (TimeStopped != 0)
		return;

	auto original = reinterpret_cast<decltype(mhmissile_r)*>(mhMissile_t->Target());
	original(a1);
}

void __cdecl Sonic2PTimeStopMan_r(ObjectMaster* a1)
{
	CharObj2Base* co2;
	int v2; 

	co2 = a1->Data2.Character;
	v2 = (unsigned __int8)co2->CharID; //this is actually player num in this scenario
	
	if (!TwoPlayerMode && (!isSuper[v2] || chaosControlTimer >= chaosControlDuration))
	{
		a1->MainSub = DeleteObject_;
		return;
	}

	auto original = reinterpret_cast<decltype(Sonic2PTimeStopMan_r)*>(Sonic2PTimeStopMan_t->Target());
	original(a1);
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

void initChaosControl_Hack()
{
	if (!AllowSuperAttacks)
		return;

	//fix crash when 2P doesn't exist
	sub_724A50_t = new Trampoline((int)0x724A50, (int)0x724A55, sub_724A50_r);

	//reset cam
	WriteCall((void*)0x47746F, ChaosControl_FixCamPose);
	WriteCall((void*)0x477445, ChaosControl_FixCamPose);
	WriteCall((void*)0x4773e9, ChaosControl_FixCamPose);

	//freeze time on objects
	seagull_t = new Trampoline((int)0x6F6070, (int)0x6F6076, seagull_r);
	mhcont_t = new Trampoline((int)0x6F9120, (int)0x6F9125, mhcont_r);
	mhcont0_t = new Trampoline((int)0x6F86E0, (int)0x6F86E5, mhcont0_r);
	mhMissile_t = new Trampoline((int)0x6F4260, (int)0x6F4266, mhmissile_r);

	WriteCall((void*)0x724A2A, RenderNumberASM);
	Sonic2PTimeStopMan_t = new Trampoline((int)Sonic2PTimeStopMan, (int)Sonic2PTimeStopMan + 0x7, Sonic2PTimeStopMan_r);
}