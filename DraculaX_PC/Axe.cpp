#include "Axe.h"
#include "TextureManager.h"

namespace
{
	const float gravity = 0.2f;
}

void Axe::init(const glm::ivec2& tileMapDispl, ShaderProgram& shaderProgram, int lookingDirection)
{
	quadSize = glm::ivec2(32, 32);
	tex = TextureManager::instance().getTexture("subweapons");
	sprite = Sprite::createSprite(quadSize, glm::vec2(0.125f, 1.f), tex, &shaderProgram);
	sprite->setNumberAnimations(1);
	sprite->setAnimationSpeed(0, 30);
	sprite->animatorX(0, 8, 0.f, 0.125f, 0.f);
	sprite->changeAnimation(0);
	this->lookingDirection = lookingDirection;
	this->tileMapDispl = tileMapDispl;
	this->shader = &shaderProgram;
	velocityY = -6.5f;
}

void Axe::update(int deltaTime)
{
	//cout << position.x << ", " << position.y << endl;
	position.x += 1.5f * lookingDirection;
	position.y += velocityY;
	velocityY += gravity;
	timeElapsed += deltaTime;
	setPosition(position);
	sprite->update(deltaTime);
}

void Axe::render()
{
	shader->setUniform1i("flip", lookingDirection == -1);
	shader->setUniform1f("frameWidth", .125f);
	sprite->render();
}

int Axe::getDamage() const
{
	return 7;
}

const Hitbox Axe::getHitbox() const
{
	Hitbox hb;
	hb.min = position;
	hb.max = hb.min + glm::vec2(quadSize-1);
	return hb;
}

bool Axe::getsRemoved() const
{
	return timeElapsed > 1500;
}
