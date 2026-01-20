#include "Orb.h"
#define FALL_SPEED 3.f

void Orb::init(const glm::ivec2& tileMapDispl, ShaderProgram& shader, Texture& tex)
{
	this->tileMapDispl = tileMapDispl;
	this->quadSize = glm::ivec2(16);
	sprite = Sprite::createSprite(glm::ivec2(16), glm::vec2(0.125f, 0.125f), &tex, &shader);
	sprite->setNumberAnimations(2);
	sprite->setAnimationSpeed(0, 20);
	sprite->animatorX(0, 8, 0.f, 0.125f, 0.f);
	sprite->animatorX(0, 8, 0.f, 0.125f, 0.125f);
	sprite->animatorX(0, 8, 0.f, 0.125f, 0.250f);
	sprite->animatorX(0, 8, 0.f, 0.125f, 0.375f);
	sprite->setAnimationSpeed(1, 20);
	sprite->animatorX(1, 8, 0.f, 0.125f, 0.500f);
	sprite->animatorX(1, 5, 0.f, 0.125f, 0.625f);
	sprite->setTransition(0, 1);
	sprite->changeAnimation(0);
}

void Orb::update(int deltaTime)
{
	sprite->update(deltaTime);
	if (sprite->animation() == 1)
	{
		position.y += FALL_SPEED;
		tileMap->collisionMoveDown(getHitbox(), &position.y, quadSize.y);
		setPosition(position);
	}
}

void Orb::grab(GUI& gui)
{
	gui.stageClear();
	end();
}

bool Orb::isGrabable() const
{
	return sprite->animation() == 1;
}

void Orb::makeEndSound() const
{
	SoundEngine::instance().playSFX(SoundEngine::PICKUP_TRINKET);
}
