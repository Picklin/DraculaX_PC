#include "Candle.h"
#define MAP_OFFSET glm::vec2(32, 16)

Candle::~Candle()
{
	if (fire)
	{
		fire->free();
		delete fire;
		fireTex->free();
		delete fireTex;
	}
	base->free();
	delete base;
	baseTex->free();
	delete baseTex;
}

Candle* Candle::createCandle(ShaderProgram& shaderProgram, const glm::vec2& position, int itemId)
{
	Candle* candle = new Candle(itemId);
	candle->fireTex = new Texture();
	candle->fireTex->loadFromFile("images/candles/fire1.png", TEXTURE_PIXEL_FORMAT_RGBA);
	candle->fireTex->setMagFilter(GL_NEAREST);
	candle->baseTex = new Texture();
	candle->baseTex->loadFromFile("images/candles/base1.png", TEXTURE_PIXEL_FORMAT_RGBA);
	candle->baseTex->setMagFilter(GL_NEAREST);
	candle->fire = Sprite::createSprite(glm::ivec2(16), glm::vec2(0.5f, 1.f), candle->fireTex, &shaderProgram);
	candle->base = Sprite::createSprite(glm::ivec2(16), glm::vec2(1.f), candle->baseTex, &shaderProgram);
	candle->fire->setNumberAnimations(2);
	candle->fire->setAnimationSpeed(0, 30);
	candle->fire->animatorX(0, 2, 0.f, 0.5f, 0.f);
	candle->fire->changeAnimation(0);
	candle->fire->setPosition(position + MAP_OFFSET);
	candle->base->setPosition(position + MAP_OFFSET);
	candle->setHitbox(candle, position);
	return candle;
}

Candle* Candle::createTorchCandle(ShaderProgram& shaderProgram, const glm::vec2& position, int itemId)
{
	Candle* candle = new Candle(itemId);
	candle->fireTex = new Texture();
	candle->fireTex->loadFromFile("images/candles/torch_fire.png", TEXTURE_PIXEL_FORMAT_RGBA);
	candle->fireTex->setMagFilter(GL_NEAREST);
	candle->baseTex = new Texture();
	candle->baseTex->loadFromFile("images/candles/torch_base.png", TEXTURE_PIXEL_FORMAT_RGBA);
	candle->baseTex->setMagFilter(GL_NEAREST);
	candle->fire = Sprite::createSprite(glm::ivec2(16), glm::vec2(0.125f, 1.f), candle->fireTex, &shaderProgram);
	candle->base = Sprite::createSprite(glm::ivec2(32, 64), glm::vec2(0.5f, 1.f), candle->baseTex, &shaderProgram);
	candle->fire->setNumberAnimations(1);
	candle->fire->setAnimationSpeed(0, 60);
	candle->fire->animatorX(0, 6, 0.f, 0.125f, 0.f);
	candle->fire->addKeyframe(0, glm::vec2(0.5f, 0.f));
	candle->fire->addKeyframe(0, glm::vec2(0.75f, 0.f));
	candle->fire->addKeyframe(0, glm::vec2(0.5f, 0.f));
	candle->fire->addKeyframe(0, glm::vec2(0.875f, 0.f));
	candle->fire->changeAnimation(0);
	candle->base->setNumberAnimations(1);
	candle->base->setAnimationSpeed(0, 60);
	candle->base->addKeyframe(0, glm::vec2(0.f, 0.f));
	candle->base->addKeyframe(0, glm::vec2(0.5f, 0.f));
	candle->base->changeAnimation(0);
	candle->fire->setPosition(position + glm::vec2(8, 16) + MAP_OFFSET);
	candle->base->setPosition(position + MAP_OFFSET);
	glm::ivec2 quadSize = glm::ivec2(candle->baseTex->width(), candle->baseTex->height());
	candle->hitbox.min = position + glm::vec2(5, 23);
	candle->hitbox.max = candle->hitbox.min + glm::vec2(16, 40);
	return candle;
}

Candle* Candle::createPilarCandle(ShaderProgram& shaderProgram, const glm::vec2& position, int itemId)
{
	Candle* candle = new Candle(itemId);
	candle->fireTex = new Texture();
	candle->fireTex->loadFromFile("images/candles/fire2.png", TEXTURE_PIXEL_FORMAT_RGBA);
	candle->fireTex->setMagFilter(GL_NEAREST);
	candle->baseTex = new Texture();
	candle->baseTex->loadFromFile("images/candles/base2.png", TEXTURE_PIXEL_FORMAT_RGBA);
	candle->baseTex->setMagFilter(GL_NEAREST);
	candle->fire = Sprite::createSprite(glm::ivec2(16), glm::vec2(0.25f, 1.f), candle->fireTex, &shaderProgram);
	candle->base = Sprite::createSprite(glm::ivec2(16, 32), glm::vec2(1.f), candle->baseTex, &shaderProgram);
	candle->fire->setNumberAnimations(1);
	candle->fire->setAnimationSpeed(0, 30);
	candle->fire->animatorX(0, 4, 0.f, 0.25f, 0.f);
	candle->fire->changeAnimation(0);
	candle->fire->setPosition(position - glm::vec2(0, 16) + MAP_OFFSET);
	candle->base->setPosition(position + MAP_OFFSET);
	candle->setHitbox(candle, position);
	return candle;
}

/*void Candle::init(ShaderProgram& shaderProgram, const glm::vec2& position)
{
	fireTex = new Texture();
	fireTex->loadFromFile("images/gui&items/torch_fire.png", TEXTURE_PIXEL_FORMAT_RGBA);
	fireTex->setMagFilter(GL_NEAREST);
	baseTex = new Texture();
	baseTex->loadFromFile("images/gui&items/torch_base.png", TEXTURE_PIXEL_FORMAT_RGBA);
	baseTex->setMagFilter(GL_NEAREST);
	fire = Sprite::createSprite(glm::ivec2(16), glm::vec2(0.125f, 1.f), fireTex, &shaderProgram);
	base = Sprite::createSprite(glm::ivec2(32, 64), glm::vec2(0.5f, 1.f), baseTex, &shaderProgram);
	fire->setNumberAnimations(1);
	fire->setAnimationSpeed(0, 60);
	fire->animatorX(0, 6, 0.f, 0.125f, 0.f);
	fire->addKeyframe(0, glm::vec2(0.5f, 0.f));
	fire->addKeyframe(0, glm::vec2(0.75f, 0.f));
	fire->addKeyframe(0, glm::vec2(0.5f, 0.f));
	fire->addKeyframe(0, glm::vec2(0.875f, 0.f));
	fire->changeAnimation(0);
	base->setNumberAnimations(1);
	base->setAnimationSpeed(0, 60);
	base->addKeyframe(0, glm::vec2(0.f, 0.f));
	base->addKeyframe(0, glm::vec2(0.5f, 0.f));
	base->changeAnimation(0);
	fire->setPosition(position + glm::vec2(8, 16));
	base->setPosition(position);
}*/

void Candle::update(int deltaTime)
{
	if (destroyed)
	{
		endTimer -= deltaTime;
		if (endTimer < 0) endTimer = 0;
	}
	fire->update(deltaTime);
	base->update(deltaTime);
}

void Candle::render()
{
	fire->render();
	base->render();
}

void Candle::destroy()
{
	destroyed = true;
	endTimer = 0;
}

bool Candle::isDestroyed() const
{
	return destroyed;
}

bool Candle::getsRemoved() const
{
	return destroyed && endTimer == 0;
}

int Candle::getDropId() const
{
	return dropId;
}

Hitbox Candle::getHitbox() const
{
	return hitbox;
}

glm::vec2 Candle::getDropPosition() const
{
	glm::vec2 pos = hitbox.min;
	return pos;
}

void Candle::setHitbox(Candle* candle, const glm::vec2& position)
{
	glm::ivec2 quadSize = glm::ivec2(candle->baseTex->width(), candle->baseTex->height());
	candle->hitbox.min = position;
	candle->hitbox.max = position + glm::vec2(quadSize - 1);
}
