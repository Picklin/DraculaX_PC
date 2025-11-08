#include "EffectsManager.h"

EffectsManager::EffectsManager()
{
	fragsTex = Texture();
	fragsTex.loadFromFile("images/effects/fragments.png", TEXTURE_PIXEL_FORMAT_RGBA);
	fragsTex.setMagFilter(GL_NEAREST);

	pointsTex = Texture();
	pointsTex.loadFromFile("images/effects/points.png", TEXTURE_PIXEL_FORMAT_RGBA);
	pointsTex.setMagFilter(GL_NEAREST);
}

EffectsManager& EffectsManager::instance()
{
	static EffectsManager em;
	return em;
}

void EffectsManager::init(glm::ivec2& tileMapDispl, ShaderProgram& program)
{
	this->tileMapDispl = tileMapDispl;
	this->program = &program;
}

void EffectsManager::update(int deltaTime)
{
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

void EffectsManager::initEffect(Effect* e, const glm::vec2& pos, const glm::vec4& color)
{
	e->init(tileMapDispl, *program);
	e->setPosition(pos);
	e->setColor(color);
}