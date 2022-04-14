#include "pch.h"

int windCutterCD[2] = { 0, 0 };
const int windCutterTimer = 2400;  //40 seconds
bool iswindCutter[2] = { false, false };

void Check_SonicWind(CharObj2Base* co2)
{
	char pnum = co2->PlayerNum;

	if (CurrentLevel != LevelIDs_SonicVsShadow1 && CurrentLevel != LevelIDs_SonicVsShadow2)
		return;

	if (RingCount[pnum] >= 40 && windCutterCD[pnum] == windCutterTimer)
	{
		if (MainCharacter[pnum] && MainCharObj2[pnum]->CharID2 == Characters_Sonic)
			SendTimedMessage("SONIC WIND READY!", 120);

		if (MainCharacter[pnum] && MainCharObj2[pnum]->CharID2 == Characters_Shadow)
			SendTimedMessage("CHAOS SPEAR READY!", 120);

		windCutterCD[pnum]++;
		return;
	}
	else if (windCutterCD[pnum] < windCutterTimer)
	{
		windCutterCD[pnum]++;
	}
	else if (RingCount[pnum] >= 40 && Controllers[pnum].on & Buttons_Y && Controllers[pnum].press & Buttons_B)
	{
		iswindCutter[pnum] = true;
		Sonic2PWindCutterMan_Load(co2);
		windCutterCD[pnum] = 0;
		return;
	}
}

void SetSonicWindTexture()
{
	if (!TwoPlayerMode) {

		if (MainCharacter[0] && iswindCutter[0])
		{
			if (MainCharObj2[0]->CharID2 == Characters_Sonic)
				njSetTexture(getSonicTexlist());
			else if (MainCharObj2[0]->CharID2 == Characters_Shadow)
			{
				njSetTexture(getShadowTexlist());
			}

			iswindCutter[0] = false;
		}
	}

	sub_42D340();
}

void ResetWindCutter(char pnum)
{
	iswindCutter[pnum] = false;
	windCutterCD[pnum] = 0;
}


void init_SonicWindHack()
{
	if (!AllowSuperAttacks)
		return;

	WriteCall((void*)0x75AF85, SetSonicWindTexture);
}
