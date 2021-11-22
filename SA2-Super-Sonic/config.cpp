#include "pch.h"


Buttons TransformButton = Buttons_Y;
bool RemoveLimitations = false;
bool AlwaysSuperSonic = false;
int SuperMusicVersion = SA2LiveAndLearn;

static const Buttons ButtonsList[]
{
	Buttons_B,
	Buttons_Y,
	Buttons_X,
};



void ReadConfig(const char* path) {

	const IniFile* config = new IniFile(std::string(path) + "\\config.ini");

	TransformButton = ButtonsList[config->getInt("General", "TransformButton", 1)];
	RemoveLimitations = config->getBool("General", "RemoveLimitations", false);
	AlwaysSuperSonic = config->getBool("General", "AlwaysSuperSonic", false);

	SuperMusicVersion = config->getInt("Audio", "SuperMusicVersion", SA2LiveAndLearn);
	delete config;

	if (AlwaysSuperSonic)
		RemoveLimitations = true;

}