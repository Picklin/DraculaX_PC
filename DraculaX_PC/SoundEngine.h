#pragma once
#ifndef _SOUNDENGINE_INCLUDE
#define _SOUNDENGINE_INCLUDE

#include <memory>
#include <atomic>
#include <vector>
#include <irrKlang.h>
using namespace irrklang;

class SoundEngine
{
public:
	static SoundEngine& instance();

	void update();

	void pauseAllSounds();
	void unpauseAllSounds();
	void stopAllSounds();

	void grabTrinket();
	void playStart();

private:
	SoundEngine();
	void checkCurrentSound(ISound* sound);
	void addActiveSound(ISound* sound);
	void fadeOutThreadFunc(ISound* sound, int durationMs);
	void fadeInThreadFunc(ISound* sound, int durationMs);

private:
	std::atomic<bool> paused;

	std::vector<ISound*> activeSounds;

	ISoundEngine* engine;

	ISoundSource* trinketSoundSource;
	ISound* trinketSound;

	ISoundSource* startSource;
};

#endif // !_SOUNDENGINE_INCLUDE