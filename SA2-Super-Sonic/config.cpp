#include "pch.h"

Buttons TransformButton = Buttons_B;
Buttons UntransformButton = Buttons_B;
Buttons FlightButton = Buttons_Y;
bool RemoveLimitations = false;
bool AlwaysSuperSonic = false;
int SuperMusicVersion = SA2LiveAndLearn;
int superAuraState = 1;
bool SADXAura = false;
bool isUpgrade = true;
bool isPhysics = true;
bool isFlyAllowed = true;
bool unTransform = true;
bool isBoostAllowed = true;

bool AlwaysSuperShadow = false;
bool AllowSuperAttacks = true;
bool LastStory = false;
bool isJiggle = false;

Buttons boostBtn = Buttons_B;

static const Buttons ButtonsList[]
{
	Buttons_B,
	Buttons_Y,
	Buttons_X,
};

bool isLastStoryBeaten()
{
	if (!LastStory)
		return true;

	return SomeStoryUnlocksOrSomething[2];
}

void ReadConfig(const char* path) {

	const IniFile* config = new IniFile(std::string(path) + "\\config.ini");

	RemoveLimitations = config->getBool("Gameplay", "RemoveLimitations", false);
	AlwaysSuperSonic = config->getBool("Gameplay", "AlwaysSuperSonic", false);
	AlwaysSuperShadow = config->getBool("Gameplay", "AlwaysSuperShadow", false);
	isPhysics = config->getBool("Gameplay", "isPhysics", true);
	isFlyAllowed = config->getBool("Gameplay", "isFly", true);
	unTransform = config->getBool("Gameplay", "unTransform", true);
	AllowSuperAttacks = config->getBool("Gameplay", "AllowSuperAttacks", true);
	LastStory = config->getBool("Gameplay", "LastStory", false);

	TransformButton = ButtonsList[config->getInt("Controls", "TransformButton", 0)];
	FlightButton = ButtonsList[config->getInt("Controls", "FlightButton", 1)];
	boostBtn = ButtonsList[config->getInt("Controls", "boostBtn", 0)];

	superAuraState = config->getInt("Appearance", "superAuraState", 2);
	SADXAura = config->getBool("Appearance", "SADXAura", true);
	isUpgrade = config->getBool("Appearance", "isUpgrade", true);
	isJiggle = config->getBool("Appearance", "isJiggle", true);

	SuperMusicVersion = config->getInt("Audio", "SuperMusicVersion", SA2LiveAndLearn);
	delete config;

	if (AlwaysSuperSonic || AlwaysSuperShadow)
		RemoveLimitations = true;
}