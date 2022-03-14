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

bool isFlyMode = false;

void SS_SetFlyNextAction(EntityData1* data1, CharObj2Base* co2, char action, int anim) {
	data1->Action = action;
	co2->AnimInfo.Next = anim;
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

void SS_EnableFly_CheckInput(EntityData1* data1, CharObj2Base* co2, char pID) {

	if (isFlyMode || co2->AnimInfo.Current == 30)
		return;

	if (Controllers[pID].press & FlightButton)
	{
		co2->Speed.y += 4.0f;
		data1->Status &= ~Status_Ball;

		int rng = rand() % 2;

		if (rng)
			rng = 1577;
		else
			rng = 1585;
		PlayVoice(3, rng);

		SS_SetFlyNextAction(data1, co2, (char)SSFly::Ascending, ssBeginDash2);
		isFlyMode = true;
		return;
	}
}

char timerFly = 0;

void SS_DisableFly_CheckInput(EntityData1* data1, CharObj2Base* co2, char pID) {

	if (!isFlyMode)
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
		isFlyMode = false;
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
	SS_SetFlyNextAction(a2, a1, (char)SSFly::Standing, superSonicStanding);
	a1->IdleTime = 0;
	return 1;
}

void SS_Standing(EntityData1* data1, CharObj2Base* co2)
{

	float spdY = 0.0;
	char pnum = co2->PlayerNum;

	if ((0.0 != AnalogThings[co2->PlayerNum].magnitude) || (data1->Status & Status_DisableControl))
	{
		SS_SetFlyNextAction(data1, co2, (char)SSFly::Moving, superSonicFlying1);
		return;
	}

	if (Jump_Held[pnum])
	{
		spdY = 2.7;
		SS_SetFlyNextAction(data1, co2, (char)SSFly::AscendingIntro, ssBeginAscent);
		data1->Status |= Status_Attack;
		if (co2->Speed.y >= 2.7)
		{
			VibeThing(0, 15, 0, 4);
		}
		co2->Speed.y = spdY;
		return;
	}
	if (!Action_Held[pnum])
	{
		return;
	}
	spdY = -4.5;
	SS_SetFlyNextAction(data1, co2, (char)SSFly::DescendingIntro, ssBeginDescent);
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
		SS_SetFlyNextAction(data1, co2, (char)SSFly::Ascending, ssBeginDash);
		data1->Status |= Status_Attack;
		if (co2->Speed.y >= 2.7)
		{
			VibeThing(0, 15, co2->PlayerNum, 4);
			return;
		}
	}

	if (!Action_Held[pnum])
	{
		return;
	}

	SS_SetFlyNextAction(data1, co2, (char)SSFly::Descending, ssBeginDash2);
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
		SS_SetFlyNextAction(data1, co2, (char)SSFly::Ascending, ssBeginDash);
		return;
	}
	bool isHeldBtn = Jump_Held[pnum] == 0;
	if (isHeldBtn)
	{
		SS_SetFlyNextAction(data1, co2, (char)SSFly::Standing, superSonicStanding);
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
		SS_SetFlyNextAction(data1, co2, (char)SSFly::Ascending, ssBeginDash2);
		return;
	}

	bool isHeldBtn = Action_Held[pnum] == 0;

	if (isHeldBtn)
	{
		SS_SetFlyNextAction(data1, co2, (char)SSFly::Standing, superSonicStanding);
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

	if (!Action_Held[co2->PlayerNum])
	{
		data1->Action = (char)SSFly::Moving;
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
		isFlyMode = false;
		timerFly = 0;
		return;
	}
}

void SuperSonic_DisableFly(EntityData1* data1, CharObj2Base* co2) {
	data1->Action = 10;
	co2->AnimInfo.Next = 15;
	timerFly = 0;
	isFlyMode = false;
	return;
}

void SuperSonicFly_ActionsManagement(EntityData1* data1, SonicCharObj2* sCo2, CharObj2Base* co2) {

	if (!data1 || !isFly)
		return;

	if (TimerStopped != 0 && isFlyMode)
	{
		SuperSonic_DisableFly(data1, co2);
	}

	if (!isFlyMode) {

		if (data1->Action != Action_HomingAttack && data1->Action != Action_Fall || !ControllerEnabled[co2->PlayerNum])
			return;
	}

	if (!sub_721480(co2, data1, 32.0) && isSuper[co2->PlayerNum]) {
		SS_EnableFly_CheckInput(data1, co2, co2->PlayerNum);
		SS_DisableFly_CheckInput(data1, co2, co2->PlayerNum);
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
		return;
	case SSFly::AscendingIntro:
		SuperSonic_CommonPhysicsV(co2, data1, data2, 4.0f, 0.1f);
		return;
	case SSFly::DescendingIntro:
		SuperSonic_CommonPhysicsV(co2, data1, data2, -4.0f, 0.1f);
		return;
	case SSFly::Ascending:
		SuperSonic_CommonPhysicsV(co2, data1, data2, 2.0f, 0.4f);
		return;
	case SSFly::Descending:
		SuperSonic_CommonPhysicsV(co2, data1, data2, -2.0f, 0.4f);
		return;
	}
}

void SuperSonicFly_MainManagement(EntityData1* data1, CharObj2Base* co2, EntityData2* data2) {

	if (!data1 || !isFlyMode || !isFly || TimerStopped != 0)
		return;

	SuperSonicFly_MainActions(data1, co2, data2);
	return;
}
