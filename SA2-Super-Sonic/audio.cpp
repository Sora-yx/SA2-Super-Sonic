#include "pch.h"

std::string currentSuperMusic;
std::string lastMusic;

static UsercallFuncVoid(PlayMusic_t, (const char* a1), (a1), 0x442CF0, rEDI);
static UsercallFuncVoid(PlaySongQueue_t, (const char* a1), (a1), 0x443530, rEAX);

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
	if ((isSuper[0] || isSuper[1]) && lastMusic != "rndclear" && IsIngame()) {
		return;
	}

	PlaySongQueue_t.Original(song);

	if (!isSuperMusicPlaying()) {
		lastMusic = song;
	}
}

void PlayMusic_r(const char* song)
{
	if ((isSuper[0] || isSuper[1]) && lastMusic != "rndclear" && IsIngame()) {
		return;
	}

	PlayMusic_t.Original(song);

	if (!isSuperMusicPlaying()) {
		lastMusic = song;
	}

	return;
}

void init_MusicHack() {
	if (SuperMusicVersion == None)
		return;

	PlayMusic_t.Hook(PlayMusic_r);
	PlaySongQueue_t.Hook(PlaySong_Queue_r);
}