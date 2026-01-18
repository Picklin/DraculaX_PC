#include "SoundEngine.h"
#include <thread>
#include <filesystem>

namespace fs = std::filesystem;
namespace
{
	std::vector<std::string> songPaths[2];
	std::vector<std::string> otherMusicPaths[2];
}

SoundEngine::SoundEngine()
{
	engine = createIrrKlangDevice();
	loadSFX();
	loadMusic();
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
	sfx[DASH].first->setDefaultVolume(0.75f);
}

void SoundEngine::loadMusic()
{
	const std::string stageMusicRoot[2]
	{
		"[Orig] Akumajo Dracula X OST/Stage Music/", "[Arra] Akumajo Dracula X OST/Stage Music/"
	};
	const std::string otherMusicRoot[2]
	{
		"[Orig] Akumajo Dracula X OST/Other Music/", "[Arra] Akumajo Dracula X OST/Other Music/"
	};
	for (int i = 0; i < 2; ++i)
	{
		loadMusicPaths(songPaths[i], stageMusicRoot[i]);
		stageMusicSources[i].resize(songPaths[i].size(), nullptr);
	}
	for (int i = 0; i < 2; ++i)
	{
		loadMusicPaths(otherMusicPaths[i], otherMusicRoot[i]);
		otherMusicSources[i].resize(otherMusicPaths[i].size(), nullptr);
	}
	overtureSource = engine->addSoundSourceFromFile("[Orig] Akumajo Dracula X OST/1. Overture.flac");
	overtureSource->setDefaultVolume(0.25f);
	musicSound = nullptr;
}

void SoundEngine::loadMusicPaths(std::vector<std::string>& pathsContainer, const std::string& root)
{
	for (const auto& entrada : fs::directory_iterator(root))
	{
		pathsContainer.push_back(entrada.path().string());
	}
}

void SoundEngine::playMusic(const std::string& path, ISoundSource* source, bool loop)
{
	if (source == nullptr)
	{
		source = engine->addSoundSourceFromFile(path.c_str());
		source->setDefaultVolume(0.25f);
	}
	checkCurrentSound(musicSound);
	musicSound = engine->play2D(source, loop, false, true);
	addActiveSound(musicSound);
}

void SoundEngine::stopMusic(const std::string& path)
{
	engine->removeSoundSource(path.c_str());
	musicSound = nullptr;
}

void SoundEngine::playStageSong(int stageNum)
{
	playMusic(songPaths[arrangeMode][stageNum], stageMusicSources[arrangeMode][stageNum], true);
}

void SoundEngine::stopStageSong(int stageNum)
{
	stopMusic(songPaths[arrangeMode][stageNum]);
}

void SoundEngine::playNonStageSong(int songId)
{
	playMusic(otherMusicPaths[arrangeMode][songId], otherMusicSources[arrangeMode][songId], true);
}

void SoundEngine::playNonStageSong(int songId, bool loop)
{
	playMusic(otherMusicPaths[arrangeMode][songId], otherMusicSources[arrangeMode][songId], loop);
}

void SoundEngine::stopNonStageSong(int songId)
{
	stopMusic(otherMusicPaths[arrangeMode][songId]);
}

void SoundEngine::playSFX(int sfxId)
{
	checkCurrentSound(sfx[sfxId].second);
	sfx[sfxId].second = engine->play2D(sfx[sfxId].first, false, false, true);
	addActiveSound(sfx[sfxId].second);
}

void SoundEngine::playOverture()
{
	checkCurrentSound(musicSound);
	musicSound = engine->play2D(overtureSource, false, false, true);
	addActiveSound(musicSound);
}

void SoundEngine::fadeOutMusic()
{
	std::thread(&SoundEngine::fadeOutThreadFunc, this, musicSound, 2000).detach();
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
