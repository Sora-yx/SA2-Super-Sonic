#include "pch.h"


Buttons TransformButton = Buttons_B;
bool RemoveLimitations = false;
bool AlwaysSuperSonic = false;
int SuperMusicVersion = SA2LiveAndLearn;
int superAuraState;


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
	superAuraState = config->getInt("General", "superAuraState", 1);

	SuperMusicVersion = config->getInt("Audio", "SuperMusicVersion", SA2LiveAndLearn);
	delete config;

	if (AlwaysSuperSonic)
		RemoveLimitations = true;

}