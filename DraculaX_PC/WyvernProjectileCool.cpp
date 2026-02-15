#include "WyvernProjectileCool.h"

void WyvernProjectileCool::render()
{
	shader->setUniform1f("frameWidth", 0.5f);
	shader->setUniform1i("flip", dir.x > 0);
	if (!grounded)
	{
		sprite->render();
	}
	else
	{
		if (renderAlternate) sprite->render();
		renderAlternate = !renderAlternate;
	}
	shader->setUniform1i("flip", false);
}

int WyvernProjectileCool::getDamage() const
{
	return 20;
}

const Hitbox WyvernProjectileCool::getHitbox() const
{
	Hitbox hb;
	hb.min = position + glm::vec2(32 - 32 * (dir.x < 0), 32);
	hb.max = hb.min + glm::vec2(31);
	return hb;
}

const string WyvernProjectileCool::getSpritesheet() const
{
	return "images/bosses/wyvern/wyv_coolProj.png";
}

const glm::vec2 WyvernProjectileCool::getSizeInSpritesheet() const
{
	return glm::vec2(0.5f, 1.f);
}

const glm::ivec2 WyvernProjectileCool::getQuadSize() const
{
	return glm::ivec2(64);
}

void WyvernProjectileCool::childUpdate(int deltaTime)
{
	if (!grounded)
	{
		position += speed * dir;
		if (tileMap->collisionMoveDown(getHitbox()))
		{
			grounded = true;
			sprite->changeAnimation(1);
		}
	}
	else position.x += speed.x * dir.x;
	setPosition(position);
}

void WyvernProjectileCool::setAnimations()
{
	sprite->setNumberAnimations(2);
	sprite->setAnimationSpeed(0, 0);
	sprite->addKeyframe(0, glm::vec2(0.5f, 0.f));
	sprite->setAnimationSpeed(1, 0);
	sprite->addKeyframe(1, glm::vec2(0.0f, 0.f));
	sprite->changeAnimation(0);
}

glm::vec2 WyvernProjectileCool::setSpeed()
{
	return glm::vec2(2, 4);
}
