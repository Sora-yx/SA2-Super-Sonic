#include "pch.h"
#include "ss.h"

enum class SSFly {
	Standing = 100,
	Moving,
	AscendingIntro,
	DescendingIntro,
	Ascending,
	Descending,
};

const int boostTimerC = 600;
const int boostReloadC = 1000;

int BoostUseTimer = boostTimerC;
int boostReload = boostReloadC;
const float spdCap = 20.0f;


bool isBoosting(char pnum)
{
	return BoostUseTimer > 0 && BoostUseTimer < boostTimerC && Controllers[pnum].on & boostBtn;
}

Buttons DescendButton()
{
	if (!isBoostAllowed || boostBtn != Buttons_B && boostBtn != Buttons_X) {
		return (Buttons)1026;
	}

	if (boostBtn == Buttons_B)
		return Buttons_X;
	else
		return Buttons_B;
}


bool isFlyMode[2] = { false, false };

void SS_SetFlyNextAction(EntityData1* data1, CharObj2Base* co2, char action, __int16 anim) {
	data1->Action = action;
	co2->AnimInfo.Next = (__int16)anim;
	return;
}

void SuperSonic_CommonPhysics(CharObj2Base* co2, EntityData1* data1, EntityData2* data2)
{
	PGetRotation(data1, data2, co2);
	SuperSonicGetAccel(data1, data2, co2);
	PGetSpeed(data1, co2, data2);
	PSetPosition(data1, data2, co2);
	PResetPosition(data1, data2, co2);
}

void SuperSonic_CommonPhysicsV(CharObj2Base* co2, EntityData1* data1, EntityData2* data2, float A, float B)
{
	co2->Speed.y = FloatCalcResult(co2->Speed.y, A, B);
	PResetAngle_r(data1, co2);
	SuperSonic_CommonPhysics(co2, data1, data2);

}

void SuperSonic_BoostCheck(EntityData1* data, CharObj2Base* co2, char pNum)
{
	if (Controllers[pNum].on & boostBtn)
	{
		if (co2->Speed.x < spdCap)
			co2->Speed.x += 0.0100f;

		BoostUseTimer--;

		if (BoostUseTimer <= 0)
		{
			if (co2->Speed.x > 7.0f)
				co2->Speed.x -= 5.0f;

			boostReload = 0;
			BoostUseTimer = boostTimerC;
			data->Action--;
		}
	}
	else
	{
		data->Action--;
	}
}

void SS_BoostTask(ObjectMaster* obj)
{
	EntityData1* data = obj->Data1.Entity;
	CharObj2Base* co2 = MainCharObj2[data->Index];

	if (!isFlyMode[data->Index])
	{
		DeleteObject_(obj);
		return;
	}

	if (boostReload < boostReloadC)
	{
		boostReload++;

		if (boostReload == boostReloadC - 1)
			SendTimedMessage("BOOST READY!", 120);
	}
	else {

		switch (data->Action)
		{
		case 0:

			if (Controllers[data->Index].press & boostBtn)
			{
				if (co2->Speed.x < spdCap - 5.0f)
					co2->Speed.x += 5.0f;

				data->Action++;
			}

			break;
		case 1:
			SuperSonic_BoostCheck(data, co2, data->Index);
			break;

		}
	}
}

void SS_EnableFly_CheckInput(EntityData1* data1, CharObj2Base* co2, char pID) {

	if (isFlyMode[pID] || co2->AnimInfo.Current == 30)
		return;

	if (Controllers[pID].press & FlightButton)
	{
		co2->Speed.y += 4.0f;
		data1->Status &= ~Status_Ball;

		bool isSonic = co2->CharID2 == Characters_Sonic;

		int rng = rand() % 2;

		if (rng)
			rng = isSonic ? 1577 : 1552;
		else
			rng = isSonic ? 1585 : 1520;

		PlayVoice(3, rng);

		SS_SetFlyNextAction(data1, co2, (char)SSFly::Ascending, (__int16)ssBeginDash2);
		isFlyMode[pID] = true;
		ObjectMaster* boost = LoadObject(2, "boostTask", SS_BoostTask, LoadObj_Data1);
		boost->Data1.Entity->Index = co2->PlayerNum;
		return;
	}
}

char timerFly = 0;

void SS_DisableFly_CheckInput(EntityData1* data1, CharObj2Base* co2, char pID) {

	if (!isFlyMode[pID])
		return;

	if (timerFly > 20)
		timerFly = 20;

	if (++timerFly < 20)
		return;

	if (Controllers[pID].press & FlightButton)
	{
		data1->Action = 10;
		co2->AnimInfo.Next = 15;
		timerFly = 0;
		isFlyMode[pID] = false;
	}

	return;
}

signed int isSuperSonicStanding(CharObj2Base* a1, EntityData1* a2)
{
	if ((a2->Status & 0x4000) == 0
		&& 0.0 != AnalogThings[a1->PlayerNum].magnitude
		&& ((a2->Status & Status_OnPath) != 0 || !a1->DisableControlTimer)
		|| 0.0 != a1->Speed.x)
	{
		return 0;
	}
	SS_SetFlyNextAction(a2, a1, (char)SSFly::Standing, (__int16)superSonicStanding);
	a1->IdleTime = 0;
	return 1;
}

void SS_Standing(EntityData1* data1, CharObj2Base* co2)
{

	float spdY = 0.0;
	char pnum = co2->PlayerNum;

	if ((0.0 != AnalogThings[co2->PlayerNum].magnitude) || (data1->Status & Status_DisableControl))
	{
		SS_SetFlyNextAction(data1, co2, (char)SSFly::Moving, (__int16)superSonicFlying1);
		return;
	}

	if (Jump_Held[pnum])
	{
		spdY = 2.7;
		SS_SetFlyNextAction(data1, co2, (char)SSFly::AscendingIntro, (__int16)ssBeginAscent);
		data1->Status |= Status_Attack;
		if (co2->Speed.y >= 2.7)
		{
			VibeThing(0, 15, 0, 4);
		}
		co2->Speed.y = spdY;
		return;
	}
	if ((Controllers[pnum].on & DescendButton()) == 0)
	{
		return;
	}
	spdY = -4.5;
	SS_SetFlyNextAction(data1, co2, (char)SSFly::DescendingIntro, (__int16)ssBeginDescent);
	data1->Status |= Status_Attack;
	if (co2->Speed.y > spdY)
	{
		co2->Speed.y = spdY;
	}
	VibeThing(0, 15, 0, 4);
	return;
}

void SS_Moving(EntityData1* data1, CharObj2Base* co2)
{
	if (data1->NextAction != 0 || data1->Status & Status_DoNextAction || isSuperSonicStanding(co2, data1))
	{
		return;
	}

	float curSpeed = 0.0;
	char pnum = co2->PlayerNum;

	if (Jump_Held[pnum])
	{
		curSpeed = 2.7;
		SS_SetFlyNextAction(data1, co2, (char)SSFly::Ascending, (__int16)ssBeginDash);
		data1->Status |= Status_Attack;
		if (co2->Speed.y >= 2.7)
		{
			VibeThing(0, 15, co2->PlayerNum, 4);
			return;
		}
	}

	if (( (Controllers[pnum].on & DescendButton()) == 0) || co2->CurrentSurfaceFlags & SurfaceFlag_Solid)
	{
		return;
	}

	SS_SetFlyNextAction(data1, co2, (char)SSFly::Descending, (__int16)ssBeginDash2);
	data1->Status |= Status_Attack;

	VibeThing(0, 15, co2->PlayerNum, 4);
	return;
}

void SS_AscendingIntro(EntityData1* data1, CharObj2Base* co2)
{

	char pnum = co2->PlayerNum;

	if ((data1->Status & Status_DisableControl) == 0
		&& 0.0 != AnalogThings[pnum].magnitude
		&& ((data1->Status & Status_OnPath) != 0 || !co2->DisableControlTimer))
	{
		SS_SetFlyNextAction(data1, co2, (char)SSFly::Ascending, (__int16)ssBeginDash);
		return;
	}
	bool isHeldBtn = Jump_Held[pnum] == 0;
	if (isHeldBtn)
	{
		SS_SetFlyNextAction(data1, co2, (char)SSFly::Standing, (__int16)superSonicStanding);
		data1->Status &= ~Status_Attack;
	}

	return;
}

void SS_DescendingIntro(EntityData1* data1, CharObj2Base* co2)
{

	char pnum = co2->PlayerNum;

	if ((data1->Status & Status_DisableControl) == 0
		&& 0.0 != AnalogThings[pnum].magnitude
		&& ((data1->Status & Status_OnPath) != 0 || !co2->DisableControlTimer))
	{
		SS_SetFlyNextAction(data1, co2, (char)SSFly::Ascending, (__int16)ssBeginDash2);
		return;
	}

	bool isHeldBtn = (Controllers[pnum].on & DescendButton()) == 0;

	if (isHeldBtn)
	{
		SS_SetFlyNextAction(data1, co2, (char)SSFly::Standing, (__int16)superSonicStanding);
		data1->Status &= ~Status_Attack;
	}

	return;
}

void SS_Ascending(EntityData1* data1, CharObj2Base* co2)
{

	if (isSuperSonicStanding(co2, data1))
	{
		data1->Status &= ~Status_Attack;
		return;
	}

	if (!Jump_Held[co2->PlayerNum])
	{
		data1->Action = (char)SSFly::Moving;
		data1->Status &= ~Status_Attack;
	}

	float spdY = 3.5f;

	if (co2->Speed.y < spdY)
	{
		co2->Speed.y += 0.5f;
	}
	else {
		co2->Speed.y = spdY;
	}

	return;
}

void SS_Descending(EntityData1* data1, CharObj2Base* co2)
{

	if (isSuperSonicStanding(co2, data1))
	{
		data1->Status &= ~Status_Attack;
		return;
	}

	if (( (Controllers[co2->PlayerNum].on & DescendButton()) == 0) || co2->CurrentSurfaceFlags & SurfaceFlag_Solid)
	{
		SS_SetFlyNextAction(data1, co2, (char)SSFly::Moving, ssBeginDash);
		data1->Status &= ~Status_Attack;
	}

	float spdY = -3.5f;

	if (co2->Speed.y > spdY)
	{
		co2->Speed.y -= 0.5f;
	}
	else {
		co2->Speed.y = spdY;
	}

	return;
}

void SuperSonicFly_RunsActions(EntityData1* data1, CharObj2Base* co2)
{
	if (!isSuper[co2->PlayerNum] && data1->Action >= (char)SSFly::Standing)
	{
		data1->Action = Action_Fall;
	}

	switch ((SSFly)data1->Action)
	{
	case SSFly::Standing:
		SS_Standing(data1, co2);
		return;
	case SSFly::Moving:
		SS_Moving(data1, co2);
		return;
	case SSFly::AscendingIntro:
		SS_AscendingIntro(data1, co2);
		return;
	case SSFly::DescendingIntro:
		SS_DescendingIntro(data1, co2);
		return;
	case SSFly::Ascending:
		SS_Ascending(data1, co2);
		return;
	case SSFly::Descending:
		SS_Descending(data1, co2);
		return;
	default:
		isFlyMode[co2->PlayerNum] = false;
		timerFly = 0;
		return;
	}
}

void SuperSonic_DisableFly(EntityData1* data1, CharObj2Base* co2) {
	data1->Action = 10;
	co2->AnimInfo.Next = 15;
	timerFly = 0;
	isFlyMode[co2->PlayerNum] = false;
	return;
}

void SuperSonicFly_ActionsManagement(EntityData1* data1, SonicCharObj2* sCo2, CharObj2Base* co2) {

	if (!data1 || !isFlyAllowed)
		return;

	char pnum = co2->PlayerNum;

	if (TimerStopped != 0 && isFlyMode[pnum])
	{
		SuperSonic_DisableFly(data1, co2);
	}

	if (!isFlyMode[pnum]) {

		if (data1->Action != Action_HomingAttack && data1->Action != Action_Fall || !ControllerEnabled[pnum])
			return;
	}

	if (!sub_721480(co2, data1, 32.0) && isSuper[pnum]) {
		SS_EnableFly_CheckInput(data1, co2, pnum);
		SS_DisableFly_CheckInput(data1, co2, pnum);
	}

	SuperSonicFly_RunsActions(data1, co2);
	return;
}


void SuperSonicFly_MainActions(EntityData1* data1, CharObj2Base* co2, EntityData2* data2)
{
	switch ((SSFly)data1->Action)
	{
	case SSFly::Standing:
	case SSFly::Moving:
		SuperSonic_CommonPhysics(co2, data1, data2);
		break;
	case SSFly::AscendingIntro:
		SuperSonic_CommonPhysicsV(co2, data1, data2, 4.0f, 0.1f);
		break;
	case SSFly::DescendingIntro:
		SuperSonic_CommonPhysicsV(co2, data1, data2, -4.0f, 0.1f);
		break;
	case SSFly::Ascending:
		SuperSonic_CommonPhysicsV(co2, data1, data2, 2.0f, 0.4f);
		break;
	case SSFly::Descending:
		SuperSonic_CommonPhysicsV(co2, data1, data2, -2.0f, 0.4f);
		break;
	}
}

void SuperSonicFly_MainManagement(EntityData1* data1, CharObj2Base* co2, EntityData2* data2) {

	if (!isFlyAllowed || !data1 || !isFlyMode[co2->PlayerNum] || TimerStopped != 0)
		return;

	SuperSonicFly_MainActions(data1, co2, data2);
	return;
}
