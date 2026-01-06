#include "SoundEngine.h"
#include <thread>
#include <filesystem>

namespace fs = std::filesystem;
namespace
{
	std::vector<std::string> songPaths[2];
	const std::string stageMusicRoot[2]
	{
		"[Orig] Akumajo Dracula X OST/Stage Music/", "[Arra] Akumajo Dracula X OST/Stage Music/"
	};
}

SoundEngine::SoundEngine()
{
	engine = createIrrKlangDevice();
	loadSFX();
	loadOSTpaths();
}

void SoundEngine::checkCurrentSound(ISound* sound)
{
	if (sound)
	{
		sound->stop();
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

void SoundEngine::loadSFX()
{
	for (const auto& entrada : fs::directory_iterator("sfx/"))
	{
		std::string path = entrada.path().string();
		sfx.push_back(std::pair<ISoundSource*, ISound*>(engine->addSoundSourceFromFile(path.c_str()), nullptr));
	}
}

void SoundEngine::loadOSTpaths()
{
	for (int i = 0; i < 2; i++)
	{
		for (const auto& entrada : fs::directory_iterator(stageMusicRoot[i]))
		{
			songPaths[i].push_back(entrada.path().string());
		}
		stageMusicSources[i].resize(songPaths[i].size());
	}
}

void SoundEngine::playStageSong(int stageNum)
{
	auto& stageSong = stageMusicSources[arrangeMode][stageNum];
	if (stageSong == nullptr)
	{
		std::string path = songPaths[arrangeMode][stageNum];
		stageSong = engine->addSoundSourceFromFile(path.c_str());
		stageSong->setDefaultVolume(0.25f);
		stageSong->grab();
	}
	checkCurrentSound(musicSound);
	musicSound = engine->play2D(stageSong, true, false, true);
	addActiveSound(musicSound);
}

void SoundEngine::playSFX(int sfxId)
{
	checkCurrentSound(sfx[sfxId].second);
	sfx[sfxId].second = engine->play2D(sfx[sfxId].first, false, false, true);
	addActiveSound(sfx[sfxId].second);
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

void SoundEngine::setMusicMode(bool arranged)
{
	arrangeMode = arranged;
}

SoundEngine& SoundEngine::instance()
{
	static SoundEngine se;
	return se;
}
