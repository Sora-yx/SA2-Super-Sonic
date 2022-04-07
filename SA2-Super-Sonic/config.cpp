#include "pch.h"

Buttons TransformButton = Buttons_B;
Buttons FlightButton = Buttons_Y;
bool RemoveLimitations = false;
bool AlwaysSuperSonic = false;
int SuperMusicVersion = SA2LiveAndLearn;
int superAuraState = 1;
bool isUpgrade = true;
bool isPhysics = true;
bool isFlyAllowed = true;
bool unTransform = true;

bool AlwaysSuperShadow = false;

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
	isFlyAllowed = config->getBool("Gameplay", "isFly", true);
	unTransform = config->getBool("Gameplay", "unTransform", true);

	TransformButton = ButtonsList[config->getInt("Controls", "TransformButton", 0)];
	FlightButton = ButtonsList[config->getInt("Controls", "FlightButton", 1)];

	superAuraState = config->getInt("Appearance", "superAuraState", 2);
	isUpgrade = config->getBool("Appearance", "isUpgrade", true);

	SuperMusicVersion = config->getInt("Audio", "SuperMusicVersion", SA2LiveAndLearn);
	delete config;

	if (AlwaysSuperSonic)
		RemoveLimitations = true;
}