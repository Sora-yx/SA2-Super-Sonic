#include "pch.h"


Buttons TransformButton = Buttons_B;
bool RemoveLimitations = false;
bool AlwaysSuperSonic = false;
int SuperMusicVersion = SA2LiveAndLearn;
int superAuraState = 1;
bool isUpgrade = true;

static const Buttons ButtonsList[]
{
	Buttons_B,
	Buttons_Y,
	Buttons_X,
};


void ReadConfig(const char* path) {

	const IniFile* config = new IniFile(std::string(path) + "\\config.ini");
	TransformButton = ButtonsList[config->getInt("General", "TransformButton", 0)];
	RemoveLimitations = config->getBool("General", "RemoveLimitations", false);
	AlwaysSuperSonic = config->getBool("General", "AlwaysSuperSonic", false);

	superAuraState = config->getInt("Appearance", "superAuraState", 2);
	isUpgrade = config->getBool("Appearance", "isUpgrade", true);

	SuperMusicVersion = config->getInt("Audio", "SuperMusicVersion", SA2LiveAndLearn);
	delete config;

	if (AlwaysSuperSonic)
		RemoveLimitations = true;
}