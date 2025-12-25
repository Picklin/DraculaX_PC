#include "SoundEngine.h"
#include <thread>

SoundEngine::SoundEngine()
{
	engine = createIrrKlangDevice();
	pauseSource = engine->getSoundSource("sfx/Pause.wav");
	pauseSource->grab();
	pickupTrinketSource = engine->getSoundSource("sfx/PickupTrinket.wav");
	pickupTrinketSource->grab();
	startSource = engine->getSoundSource("sfx/Start.wav");
	startSource->grab();
	pauseSound = nullptr;
	pickupTrinketSound = nullptr;
}

void SoundEngine::checkCurrentSound(ISound* sound)
{
	if (sound)
	{
		sound->stop();
		sound->drop();
		sound = nullptr;
	}
}

void SoundEngine::addActiveSound(ISound* sound)
{
	sound->grab();
	activeSounds.push_back(sound);
}

void SoundEngine::fadeOutThreadFunc(ISound* sound, int durationMs)
{
	if (!sound) return;

	float startVolume = sound->getVolume();
	float step = startVolume / (durationMs / 50.0f);

	for (int i = 0; i < (durationMs / 50); ++i) {
		while (paused.load())
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
		}
		float newVolume = sound->getVolume() - step;
		if (newVolume < 0.0f) newVolume = 0.0f;
		sound->setVolume(newVolume);
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}

	sound->stop();
	sound->drop();
}

void SoundEngine::fadeInThreadFunc(ISound* sound, int durationMs)
{
	if (!sound) return;

	sound->setVolume(0.0f);
	sound->setIsPaused(false);

	float step = 1.0f / (durationMs / 50.0f);

	for (int i = 0; i < (durationMs / 50); ++i) {
		while (paused.load())
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
		}
		float newVolume = sound->getVolume() + step;
		if (newVolume > 1.0f) newVolume = 1.0f;
		sound->setVolume(newVolume);
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
}

void SoundEngine::update()
{
	for (auto it = activeSounds.begin(); it != activeSounds.end(); ) {
		if (!(*it) || (*it)->isFinished()) {
			if (*it) {
				(*it)->drop();
			}
			it = activeSounds.erase(it);
		}
		else {
			++it;
		}
	}
}

void SoundEngine::pauseAllSounds()
{
	paused = true;
	for (auto& sound : activeSounds)
	{
		if (sound && !sound->isFinished())
		{
			sound->setIsPaused(true);
		}
	}
}

void SoundEngine::unpauseAllSounds()
{
	paused = false;
	for (auto& sound : activeSounds)
	{
		if (sound && !sound->isFinished())
		{
			sound->setIsPaused(false);
		}
	}
}

void SoundEngine::stopAllSounds()
{
	engine->stopAllSounds();

}

void SoundEngine::playGrabTrinket()
{
	checkCurrentSound(pickupTrinketSound);
	pickupTrinketSound = engine->play2D(pickupTrinketSource, false, false, true);
}

void SoundEngine::playPause()
{
	checkCurrentSound(pauseSound);
	pauseSound = engine->play2D(pauseSource, false, false, true);
}

void SoundEngine::playStart()
{
	engine->play2D(startSource);
}

SoundEngine& SoundEngine::instance()
{
	static SoundEngine se;
	return se;
}
