#include "pch.h"

void njCnkMotion(NJS_OBJECT* obj, NJS_MOTION* mot, float frame)
{
	*(int*)0x25EFE54 = 0x25EFE60;
	njSetMotion(mot, frame);
	MotionDrawCallback = (ObjectFuncPtr)0x42E660;
	DrawObjMotion(obj);
}

const char* ModelFormatStrings[]{
	"collision",
	"chunk",
	"battle"
};

//Load Object File
ModelInfo* LoadMDL(const char* name, ModelFormat format) {
	std::string fullPath;

	fullPath = "resource\\gd_PC\\Models\\";
	

	fullPath += name;

	switch (format) {
	case ModelFormat_Basic:
		fullPath += ".sa1mdl";
		break;
	case ModelFormat_Chunk:
		fullPath += ".sa2mdl";
		break;
	case ModelFormat_SA2B:
		fullPath += ".sa2bmdl";
		break;
	}

	const char* foo = fullPath.c_str();

	ModelInfo* temp = new ModelInfo(HelperFunctionsGlobal.GetReplaceablePath(foo));

	if (temp->getformat() == format) {
		PrintDebug("[SA2 Super Sonic] Loaded %s model: %s.", ModelFormatStrings[(int)format - 1], name);
	}
	else {
		PrintDebug("[SA2 Super Sonic] Failed loading %s model: %s.", ModelFormatStrings[(int)format - 1], name);
	}

	return temp;
}

void FreeMDL(ModelInfo* pointer)
{
	if (pointer)
		delete(pointer);
}

void DoNextAction_r(int playerNum, char action, int unknown)
{
	EntityData1* v3; // eax

	v3 = MainCharObj1[playerNum];
	if (v3)
	{
		v3->Status |= Status_DoNextAction;
		v3->NextAction = action;
		MainCharObj2[playerNum]->field_28 = unknown;
	}
}

bool __cdecl IsIngame()
{
	if (GameState != GameStates_Ingame && GameState != GameStates_LoadFinished)
		return false;

	return true;
}

//just so I don't have to type "HelperFunctions.X"
void DisplayDebugStringFormatted(Int loc, const char* Format, ...) {
	return HelperFunctionsGlobal.DisplayDebugStringFormatted(loc, Format);
}

void SetDebugFontColor(int color) {
	return HelperFunctionsGlobal.SetDebugFontColor(color);
}

void SetDebugFontScale(float size) {
	return HelperFunctionsGlobal.SetDebugFontSize(size);
}

int DebugMessageTimer = 0;
std::string debugText = "";

void DisplayTimed_Message_OnFrames()
{
	if (DebugMessageTimer && debugText != "")
	{
		SetDebugFontScale(25);
		int DebugRightPos = (int)((float)HorizontalResolution / 25);
		SetDebugFontColor(0xFFBFBFBF);
		DisplayDebugStringFormatted(NJM_LOCATION(DebugRightPos - strlen(debugText.c_str()), 6), debugText.c_str());
		SetDebugFontColor(0xFFBFBFBF);
		DebugMessageTimer--;
	}
}

void SendTimedMessage(std::string msg, int timer)
{
	DebugMessageTimer = timer;
	debugText = msg;
}

typedef AnimationIndex* (CALLBACK* anim) (void);

AnimationIndex* getCharAnim_r()
{
	if (!SA2Anim)
		return nullptr;

	anim Obj = (anim)GetProcAddress(SA2Anim, "GetCharacterAnim_r");

	if (Obj)
	{
		AnimationIndex* Result = Obj();
		return Result;
	}

	return nullptr;
}

typedef int (*padd) (void);

int getNumber_r()
{
	if (!SA2Anim)
		return 0;

	padd Obj = (padd) GetProcAddress(SA2Anim, "getCharAnimSize");

	if (Obj)
	{
		int iResult = Obj();
		return iResult;
	}

	return 0;
}

typedef bool (*isChar) (uint8_t charID);

bool isSA1Char(uint8_t charID)
{
	if (!SA1Char)
		return false;

	isChar Obj = (isChar)GetProcAddress(SA1Char, "isSA1Char");

	if (Obj)
	{
		bool iResult = Obj(charID);
		return iResult;
	}

	PrintDebug("SA1 Char Anim: Failed to get character");
	return false;
}

