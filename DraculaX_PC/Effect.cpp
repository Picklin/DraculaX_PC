#include "Effect.h"
#include "TextureManager.h"
#include <iostream>

Effect::~Effect()
{
	if (sprite != NULL)
	{
		sprite->free();
		delete sprite;
	}
}

void Effect::init(const glm::ivec2& tileMapDispl, ShaderProgram& shaderProgram)
{
	this->tileMapDispl = tileMapDispl;
	this->shader = &shaderProgram;
	string name = getName();
	if (TextureManager::instance().exists(name))
	{
		spritesheet = TextureManager::instance().getTexture(name);
	}
	else
	{
		spritesheet = new Texture();
		spritesheet->loadFromFile(getSpritesheet(), TEXTURE_PIXEL_FORMAT_RGBA);
		spritesheet->setMagFilter(GL_NEAREST);
		TextureManager::instance().addTexture(name, spritesheet);
	}
	glm::vec2 sizeinspritesheet = getSizeInSpritesheet();
	quadSize.x = int(spritesheet->width() * sizeinspritesheet.x);
	quadSize.y = int(spritesheet->height() * sizeinspritesheet.y);
	sprite = Sprite::createSprite(quadSize, sizeinspritesheet, spritesheet, &shaderProgram);
	setAnimations();
	ended = false;
}

void Effect::init(const glm::ivec2& tileMapDispl, ShaderProgram& shaderProgram, const TexInfo& ti)
{
	this->tileMapDispl = tileMapDispl;
	this->shader = &shaderProgram;

	quadSize.x = int(ti.tex->width() * (ti.bottomRight.x - ti.topLeft.x));
	quadSize.y = int(ti.tex->height() * (ti.bottomRight.y - ti.topLeft.y));
	sprite = Sprite::createSprite(quadSize, ti.topLeft, ti.bottomRight, ti.tex, shader);
}

void Effect::update(int deltaTime)
{
	if (!ended)
	{
		sprite->update(deltaTime);
		childUpdate(deltaTime);
	}
}

void Effect::render()
{
	if (flicker) renderAltern = !renderAltern;
	if (renderAltern) sprite->render();
}

void Effect::setPosition(const glm::vec2& pos)
{
	this->position = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)) - myCenter());
}

void Effect::setFlicker()
{
	flicker = true;
}

void Effect::free()
{
	string name = getName();
	TextureManager::instance().removeTexture(name);
}
