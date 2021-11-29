#include "pch.h"

Buttons TransformButton = Buttons_B;
Buttons FlightButton = Buttons_Y;
bool RemoveLimitations = false;
bool AlwaysSuperSonic = false;
int SuperMusicVersion = SA2LiveAndLearn;
int superAuraState = 1;
bool isUpgrade = true;
bool isPhysics = true;
bool isFly = true;

static const Buttons ButtonsList[]
{
	Buttons_B,
	Buttons_Y,
	Buttons_X,
};


void ReadConfig(const char* path) {

	const IniFile* config = new IniFile(std::string(path) + "\\config.ini");

	RemoveLimitations = config->getBool("Gameplay", "RemoveLimitations", false);
	AlwaysSuperSonic = config->getBool("Gameplay", "AlwaysSuperSonic", false);
	isPhysics = config->getBool("Gameplay", "isPhysics", true);
	isFly = config->getBool("Gameplay", "isFly", true);

	TransformButton = ButtonsList[config->getInt("Controls", "TransformButton", 0)];
	FlightButton = ButtonsList[config->getInt("Controls", "FlightButton", 1)];

	superAuraState = config->getInt("Appearance", "superAuraState", 2);
	isUpgrade = config->getBool("Appearance", "isUpgrade", true);

	SuperMusicVersion = config->getInt("Audio", "SuperMusicVersion", SA2LiveAndLearn);
	delete config;

	if (AlwaysSuperSonic)
		RemoveLimitations = true;
}