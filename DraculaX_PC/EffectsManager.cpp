#include "EffectsManager.h"

EffectsManager::EffectsManager()
{
}

EffectsManager& EffectsManager::instance()
{
	static EffectsManager em;
	return em;
}

void EffectsManager::init(const glm::ivec2& tileMapDispl, ShaderProgram& shader)
{
	this->tileMapDispl = tileMapDispl;
	this->shader = &shader;
}

void EffectsManager::update(int deltaTime)
{
	updateExplosions(deltaTime);
	for (unsigned int i = 0; i < effects.size(); i++)
	{
		effects[i]->update(deltaTime);
		if (effects[i]->isEnded())
		{
			delete effects[i];
			effects.erase(effects.begin() + i);
		}
	}
	for (unsigned int i = 0; i < backgroundEffects.size(); i++)
	{
		backgroundEffects[i]->update(deltaTime);
		if (backgroundEffects[i]->isEnded())
		{
			delete backgroundEffects[i];
			backgroundEffects.erase(backgroundEffects.begin() + i);
		}
	}
}

void EffectsManager::render()
{
	for (auto effect : effects) effect->render();
}

void EffectsManager::free()
{
	for (auto effect : effects)
	{
		delete effect;
	}
	effects.clear();
}

void EffectsManager::freeTextures()
{
	for (auto effect : effects)
	{
		effect->free();
		delete effect;
	}
	effects.clear();
}

void EffectsManager::createFueguito(const glm::vec2& pos, const glm::vec4& color, bool background)
{
	Fueguito* f = new Fueguito();
	initEffect(f, pos, color);
	(background) ? backgroundEffects.push_back(f) : effects.push_back(f);
}

void EffectsManager::createFuegote(const glm::vec2& pos, const glm::vec4& color, bool background)
{
	Fuegote* fue = new Fuegote();
	initEffect(fue, pos, color);
	(background) ? backgroundEffects.push_back(fue) : effects.push_back(fue);
}

void EffectsManager::createExplosions(const glm::vec2* pos, const glm::vec2& entityOffset, int numExplosions, int timeBetweenExplosions, int area, const glm::vec4& color)
{
	ExplosionClusterSettings explSettings;
	explSettings.numExplosions = numExplosions;
	explSettings.offset = area;
	explSettings.bigExplosion = true;
	explSettings.explosionColor = color;
	explSettings.entityPos = pos;
	explSettings.entityOffset = entityOffset;
	explSettings.timeBetweenExplosions = timeBetweenExplosions;
	explSet.push_back(explSettings);
}

void EffectsManager::createMiniExplosions(const glm::vec2* pos, const glm::vec2& entityOffset, int numExplosions, int timeBetweenExplosions, int area, const glm::vec4& color)
{
	ExplosionClusterSettings miniExplSettings;
	miniExplSettings.numExplosions = numExplosions;
	miniExplSettings.offset = area;
	miniExplSettings.bigExplosion = false;
	miniExplSettings.explosionColor = color;
	miniExplSettings.entityPos = pos;
	miniExplSettings.entityOffset = entityOffset;
	miniExplSettings.timeBetweenExplosions = timeBetweenExplosions;
	explSet.push_back(miniExplSettings);
}

void EffectsManager::initEffect(Effect* e, const glm::vec2& pos, const glm::vec4& color)
{
	e->init(tileMapDispl, *shader);
	e->setPosition(pos);
	e->setColor(color);
}

void EffectsManager::updateExplosions(int deltaTime)
{
	for (unsigned int i = 0; i < explSet.size(); i++)
	{
		explSet[i].explosionsTimer -= deltaTime;
		if (explSet[i].explosionsTimer <= 0)
		{
			if (explSet[i].bigExplosion)
			{
				createFuegote(*explSet[i].entityPos + explSet[i].entityOffset + glm::vec2((rand() % explSet[i].offset) - (explSet[i].offset / 2), (rand() % explSet[i].offset) - (explSet[i].offset / 2)), explSet[i].explosionColor, explSet[i].background);
				createFueguito(*explSet[i].entityPos + explSet[i].entityOffset + glm::vec2((rand() % explSet[i].offset) - (explSet[i].offset / 2), (rand() % explSet[i].offset) - (explSet[i].offset / 2)), explSet[i].explosionColor, explSet[i].background);
			}
			else createFueguito(*explSet[i].entityPos + explSet[i].entityOffset + glm::vec2((rand() % explSet[i].offset) - (explSet[i].offset / 2), (rand() % explSet[i].offset) - (explSet[i].offset / 2)), explSet[i].explosionColor, explSet[i].background);
			explSet[i].numExplosions--;
			explSet[i].explosionsTimer = explSet[i].timeBetweenExplosions;
		}
		if (explSet[i].numExplosions == 0) explSet.erase(explSet.begin() + i);
	}
}