#include "pch.h"

std::string currentSuperMusic;
std::string lastMusic;

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

void PlaySong_Queue_r(const char* song)
{

	if (isSuper && lastMusic != "rndclear" && IsIngame()) {
		return;
	}

	if (sub_458970())
	{
		strcpy_s(CurrentSongName, 32, song);
	}
	else
	{
		PlayMusic(song);
		ResetMusic();
	}

	if (!isSuperMusicPlaying()) {
		lastMusic = song;
	}

	return;
}

static void __declspec(naked) PlaySong_QueueASM()
{
	__asm
	{
		push eax
		call PlaySong_Queue_r
		pop eax 
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

	if (SuperMusicVersion == None)
		return;

	PlayMusic_t = new Trampoline((int)0x442CF0, (int)0x442CF5, PlayMusicASM);
	WriteJump((void*)0x443530, PlaySong_QueueASM);
}