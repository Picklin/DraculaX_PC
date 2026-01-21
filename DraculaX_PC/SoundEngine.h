#pragma once
#ifndef _SOUNDENGINE_INCLUDE
#define _SOUNDENGINE_INCLUDE

#include <memory>
#include <atomic>
#include <vector>
#include <irrKlang.h>
#include <string>
using namespace irrklang;

class SoundEngine
{
public:
	static SoundEngine& instance();

	void update();

	void pauseAllSounds();
	void unpauseAllSounds();
	void stopAllSounds();

	void setMusicMode(bool arranged);
	void playStageSong(int stageNum);
	//void stopStageSong(int stageNum);
	void playNonStageSong(int songId);
	void playNonStageSong(int songId, bool loop);
	//void stopNonStageSong(int songId);
	void playSFX(int sfxId);
	void playLoopedSFX(int sfxId);
	void stopLoopedSFX(int sfxId);
	void playOverture();
	void fadeOutMusic(int fadeDurationMillisecs);

private:
	SoundEngine();
	void checkCurrentSound(ISound* sound);
	void addActiveSound(ISound* sound);
	void fadeOutThreadFunc(ISound* sound, int durationMs);
	void fadeInThreadFunc(ISound* sound, int durationMs);
	void loadSFX();
	void loadMusic();
	void loadMusicPaths(std::vector<std::string>& pathsContainer, const std::string& root);
	void playMusic(const std::string& path, ISoundSource* source, bool loop);
	void stopMusic(const std::string& path);

public:
	enum SFX
	{
		AXE, WYVERN_APPEAR, WYVERN_ATTACK, WYVERN_DEATH, WYVERN_WINGS, CRUCIFIX, DASH, DESTROY_BIG_BLOCK, DESTROY_BLOCK, DESTROY_GLASS, DESTROY_GLASS_HEAVY, 
		ENEMY_ATTACK, ENEMY_BIG_DEATH, SKEL_DEATH_FUNNY, SKEL_DEATH1, ENEMY_HURT, ENEMY_SHOOT1, ENEMY_SWIPE,
		EXPLOSION_BIG, EXPLOSION_HUGE, EXPLOSION_HUGE_LONG, EXPLOSION_MEDIUM, EXPLOSION_MINI, FUEGOTE, FUEGUITO,
		HEAL, HIT_GROUND, HURT_RICHTER, KONAMI_LOGO, ONEUP, ORB, PAUSE, PICKUP_HEART, PICKUP_HEART_SMALL, PICKUP_TRINKET, START, WHIP
	};
	enum RecurrentSongs
	{
		REQUIEM, FORMER_ROOM, BOSS, STAGE_CLEAR, POISON_MIND, DANCE_OF_ILLUSION, ALL_CLEAR, PLAYER_OUT, GAME_OVER
	};

private:
	std::vector<ISoundSource*> stageMusicSources[2];
	std::vector<ISoundSource*> otherMusicSources[2];
	std::vector<std::pair<ISoundSource*, ISound*>> sfx;
	std::vector<ISound*> activeSounds;
	ISoundSource* overtureSource;
	ISound* musicSound;
	ISoundEngine* engine;
	std::atomic<bool> paused;
	bool arrangeMode = false;
};

#endif // !_SOUNDENGINE_INCLUDE