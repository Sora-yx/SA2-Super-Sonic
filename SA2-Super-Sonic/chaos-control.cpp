#include "pch.h"

const int chaosControlCDTimer = 300;
int chaosControlCD[2] = { 0, 0 };
bool isChaosControlEnabled[2] = { false, false };
const int chaosControlDuration = 1200;
int chaosControlTimer = 0;
Trampoline* sub_724A50_t = nullptr;
Trampoline* seagull_t = nullptr;
Trampoline* mhcont_t = nullptr;
Trampoline* mhcont0_t = nullptr;
Trampoline* mhMissile_t = nullptr;

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

	if (chaosControlCD[pnum] < chaosControlCDTimer)
	{
		chaosControlCD[pnum]++;
	}
	else {

		if (!isChaosControlEnabled[0] && !isChaosControlEnabled[1] && Controllers[pnum].press & Buttons_B)
		{
			chaosControlCD[pnum] = 0;
			isChaosControlEnabled[pnum] = true;
			Sonic2PTimeStopMan_Load(co2);
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

void initChaosControl_Hack()
{
	//fix crash when 2P doesn't exist
	sub_724A50_t = new Trampoline((int)0x724A50, (int)0x724A55, sub_724A50_r);
	WriteCall((void*)0x47746F, ChaosControl_FixCamPose);	
	WriteCall((void*)0x477445, ChaosControl_FixCamPose);
	WriteCall((void*)0x4773e9, ChaosControl_FixCamPose);

	//freeze time on objects
	seagull_t = new Trampoline((int)0x6F6070, (int)0x6F6076, seagull_r);
	mhcont_t = new Trampoline((int)0x6F9120, (int)0x6F9125, mhcont_r);
	mhcont0_t = new Trampoline((int)0x6F86E0, (int)0x6F86E5, mhcont0_r);
	mhMissile_t = new Trampoline((int)0x6F4260, (int)0x6F4266, mhmissile_r);
}