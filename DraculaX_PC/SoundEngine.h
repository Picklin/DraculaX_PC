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
	void playSFX(int sfxId);

private:
	SoundEngine();
	void checkCurrentSound(ISound* sound);
	void addActiveSound(ISound* sound);
	void fadeOutThreadFunc(ISound* sound, int durationMs);
	void fadeInThreadFunc(ISound* sound, int durationMs);
	void loadSFX();
	void loadOSTpaths();

public:
	enum SFX
	{
		CRUCIFIX, DESTROY_BIG_BLOCK, DESTROY_BLOCK, DESTROY_GLASS, DESTROY_GLASS_HEAVY, ENEMY_ATTACK, ENEMY_BIG_DEATH, ENEMY_DEATH_FUNNY, ENEMY_SWIPE,
		EXPLOSION_BIG, EXPLOSION_HUGE, EXPLOSION_HUGE_LONG, EXPLOSION_MEDIUM, EXPLOSION_MINI, FUEGOTE, FUEGUITO,
		HEAL, ONEUP, PAUSE, PICKUP_HEART, PICKUP_HEART_SMALL, PICKUP_TRINKET, START, WHIP
	};

private:
	std::vector<ISoundSource*> stageMusicSources[2];
	std::vector<ISoundSource*> otherMusicSource[2];
	std::vector<std::pair<ISoundSource*, ISound*>> sfx;

	std::vector<ISound*> activeSounds;
	ISound* musicSound;

	ISoundEngine* engine;

	std::atomic<bool> paused;
	bool arrangeMode = false;
};

#endif // !_SOUNDENGINE_INCLUDE