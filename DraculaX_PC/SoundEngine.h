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

	void playGrabTrinket();
	void playHeal();
	void playPause();
	void playStart();
	void playWhip();
	void setMusicMode(bool arranged);
	void loadOSTpaths();
	void playStageSong(int stageNum);

private:
	SoundEngine();
	void checkCurrentSound(ISound* sound);
	void addActiveSound(ISound* sound);
	void fadeOutThreadFunc(ISound* sound, int durationMs);
	void fadeInThreadFunc(ISound* sound, int durationMs);

private:
	std::vector<ISoundSource*> stageMusicSources[2];
	std::vector<ISoundSource*> otherMusicSource[2];
	std::vector<ISound*> activeSounds;
	ISound* musicSound;

	ISoundEngine* engine;

	ISoundSource* healSource;
	ISound* healSound;

	ISoundSource* pauseSource;
	ISound* pauseSound;

	ISoundSource* pickupTrinketSource;
	ISound* pickupTrinketSound;

	ISoundSource* startSource;

	ISoundSource* whipSource;
	ISound* whipSound;
	std::atomic<bool> paused;
	bool arrangeMode = false;
};

#endif // !_SOUNDENGINE_INCLUDE