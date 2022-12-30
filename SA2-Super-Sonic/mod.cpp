#include "pch.h"
#include <fstream>
#include "ss.h"

HelperFunctions HelperFunctionsGlobal;
const char* error = "[SA2 Super Sonic]: WARNING: Your version of the Mod Loader is old, the mod won't work properly.\nPlease update your Mod Loader for the best experience.";
time_t t;
HMODULE SA2Anim = NULL;
HMODULE SA1Char = NULL;

extern "C" {
	__declspec(dllexport) void __cdecl Init(const char* path, const HelperFunctions& helperFunctions)
	{
		Sleep(10);

		srand((unsigned)time(&t));
		HelperFunctionsGlobal = helperFunctions;

		if (HelperFunctionsGlobal.Version < 8) {
			PrintDebug(error);
			MessageBoxA(MainWindowHandle, error, "SA2 Super Sonic", MB_ICONWARNING);
		}

		ReadConfig(path); //get mod settings by the user

		init_SuperSonic();
		init_MusicHack();
		init_PhysicsHack();
		init_AuraHack();
		init_SuperShadow();
		init_WaterHack();

		SA2Anim = GetModuleHandle(L"SA2-Anim-Break");
		SA1Char = GetModuleHandle(L"SA2-SA1-Chars");
	}

	__declspec(dllexport) void __cdecl OnFrame() {
		DisplayTimed_Message_OnFrames();
	}

	__declspec(dllexport) void __cdecl OnExit()
	{
		FreeWaterMDL();
		Free_AuraModels();
	}

	__declspec(dllexport) ModInfo SA2ModInfo = { ModLoaderVer };
}