#include "pch.h"

std::string currentSuperMusic;
std::string lastMusic;

Trampoline* PlaySong_Queue_t;
Trampoline* PlayMusic_t;

std::string SuperSonicMusic_Array[8] = {
	"none", "sonic2", "sonic3", "s&k",
	"sprsonic", "sa2Event", "BOSS_07", "mania"
};

std::string GetSuperSonic_Music() {

	if (SuperMusicVersion == Random)
		return SuperSonicMusic_Array[rand() % 7 + 1];

	return SuperSonicMusic_Array[(SuperMusicVersion)];
}

void Play_SuperSonicMusic() {

	if (SuperMusicVersion == None)
		return;

	std::string music = GetSuperSonic_Music() + ".adx";
	currentSuperMusic = music;
	PlayMusic(music.c_str());
	return;
}

void CheckSuperMusic_Restart(int playerID) {

	if (SuperMusicVersion == None || !IsIngame() || !MainCharObj2[playerID])
		return;

	if (MainCharObj2[playerID]->Upgrades & Upgrades_SuperSonic) {
		Play_SuperSonicMusic();
	}
}

bool isSuperMusicPlaying() {

	for (uint8_t i = 0; i < LengthOfArray(SuperSonicMusic_Array); i++)
	{
		if (currentSuperMusic == SuperSonicMusic_Array[i] + ".adx")
		{
			return true;
		}
	}

	return false;
}

void RestoreMusic() {

	if (SuperMusicVersion == None)
		return;

	if (isSuperMusicPlaying()) {

		currentSuperMusic = "";
		StopMusic();
		PlayMusic(lastMusic.c_str());
		ResetMusic();
	}

	return;
}


void PlaySongQueue_Origin(const char* song)
{
	auto target2 = PlaySong_Queue_t->Target();

	__asm
	{
		mov eax, [song]
		call target2
	}
}


void PlaySong_Queue_r(const char* song)
{

	if (isSuper && lastMusic != "rndclear" && IsIngame()) {
		return;
	}

	PlaySongQueue_Origin(song);

	if (!isSuperMusicPlaying()) {
		lastMusic = song;
	}

	return;
}

static void __declspec(naked) PlaySong_QueueASM()
{
	__asm
	{
		push eax // char *song

		// Call your __cdecl function here:
		call PlaySong_Queue_r

		pop eax // char *song
		retn
	}
}


void PlayMusic_Origin(const char* song)
{
	auto target = PlayMusic_t->Target();

	__asm
	{
		mov edi, [song]
		call target
	}
}


void PlayMusic_r(const char* song)
{

	if (isSuper && lastMusic != "rndclear" && IsIngame()) {
		return;
	}

	PlayMusic_Origin(song);

	if (!isSuperMusicPlaying()) {
		lastMusic = song;
	}

	return;
}

static void __declspec(naked) PlayMusicASM()
{
	__asm
	{
		push edi // char *song

		// Call your __cdecl function here:
		call PlayMusic_r

		pop edi // char *song
		retn
	}
}


void init_MusicHack() {
	PlayMusic_t = new Trampoline((int)0x442CF0, (int)0x442CF5, PlayMusicASM);
	PlaySong_Queue_t = new Trampoline((int)0x443530, (int)0x443538, PlaySong_QueueASM);
}