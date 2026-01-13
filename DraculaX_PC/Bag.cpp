#include "Bag.h"

Bag::Bag(int scoreAmount)
{
	this->scoreAmount = scoreAmount;
}

void Bag::initOneThousandBag(const glm::ivec2& tileMapDispl, ShaderProgram& shader, Texture& itemsTex)
{
	this->tileMapDispl = tileMapDispl;
	this->quadSize = glm::ivec2(16, 16);
	sprite = Sprite::createSprite(quadSize, glm::vec2(0.f, 0.f), glm::vec2(0.0625f, 0.0625f), &itemsTex, &shader);
	sprite->setNumberAnimations(1);
	sprite->setAnimationSpeed(0, 60);
	sprite->addKeyframe(0, glm::vec2(0.75f, 0.125f));
	sprite->addKeyframe(0, glm::vec2(0.8125f, 0.125f));
	sprite->addKeyframe(0, glm::vec2(0.875f, 0.125f));
	sprite->changeAnimation(0);
}

void Bag::update(int deltaTime)
{
	position.y += 3.f;
	if (!tileMap->collisionMoveDown(getHitbox(), &position.y, quadSize.y - 1) && platforms != nullptr)
	{
		platforms->collisionMoveDown(getHitbox(), &position.y, quadSize.y - 1);
	}
	setPosition(position);
	if (scoreAmount == 1000) sprite->update(deltaTime);
}

void Bag::grab(GUI& gui)
{
	gui.gainScore(scoreAmount);
	end();
}

void Bag::makeEndSound() const
{
	SoundEngine::instance().playSFX(SoundEngine::PICKUP_TRINKET);
}

int Bag::setEndTimer()
{
	return 0;
}
