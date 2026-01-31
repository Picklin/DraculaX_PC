#include "Skeleton1.h"
#include "SoundEngine.h"
#define SPEED 0.25f
#define FALL_SPEED 4.F

enum anims
{
	WALK, JUMP, ATTACK, DIE
};

string Skeleton1::getName() const
{
	return "Skeleton1";
}

const string Skeleton1::getSpritesheet() const
{
	return "images/enemies/skeleton1.png";
}

const glm::vec2 Skeleton1::getSizeInSpritesheet() const
{
	return glm::vec2(0.1f, 0.25f);
}

const glm::ivec2 Skeleton1::getQuadSize() const
{
	return glm::ivec2(56, 50);
}

void Skeleton1::setAnimations()
{
	sprite->setNumberAnimations(4);

	sprite->setAnimationSpeed(WALK, 6);
	sprite->animatorX(WALK, 10, 0.f, 0.1f, 0.f);

	sprite->setAnimationSpeed(JUMP, 10);
	sprite->animatorX(JUMP, 10, 0.f, 0.1f, 0.25f);
	for (int i = 0; i < 2; i++) sprite->addKeyframe(JUMP, glm::vec2(0.9f, 0.25f));
	sprite->animatorX(JUMP, 10, 0.9f, -0.1f, 0.25f);

	sprite->setAnimationSpeed(ATTACK, 10);
	sprite->addKeyframe(ATTACK, glm::vec2(0.f, 0.5f));
	for (int i = 0; i < 3; i++) sprite->addKeyframe(ATTACK, glm::vec2(0.1f, 0.5f));
	sprite->animatorX(ATTACK, 4, 0.2f, 0.1f, 0.5f);
	for (int i = 0; i < 3; i++) sprite->addKeyframe(ATTACK, glm::vec2(0.5f, 0.5f));
	sprite->addKeyframe(ATTACK, glm::vec2(0.6f, 0.5f));
	sprite->addKeyframe(ATTACK, glm::vec2(0.7f, 0.5f));

	sprite->setAnimationSpeed(DIE, 8);
	for (int i = 0; i < 5; i++) sprite->animatorX(DIE, 2, 0.f, 0.1f, 0.75f);

	sprite->setTransition(ATTACK, WALK);
	sprite->setTransition(JUMP, WALK);

	sprite->changeAnimation(WALK);
}

int Skeleton1::setEndAnimation()
{
	return DIE;
}

int Skeleton1::setEndTime()
{
	return 1000;
}

void Skeleton1::makeEndSound() const
{
	SoundEngine::instance().playSFX(SoundEngine::EXPLOSION_MINI);
	SoundEngine::instance().playSFX(SoundEngine::SKEL_DEATH1);
}

void Skeleton1::makeEndEffect() const
{
	EffectsManager::instance().createMiniExplosions(&position, myCenter(), 21, 32, 32, glm::vec3(1.f));
}

void Skeleton1::childUpdate(int deltaTime)
{
	int animation = sprite->animation();
	int frame = sprite->getCurrentKeyframe();
	if (animation == WALK)
	{
		attacking = false;
		jumping = false;
		position.y += FALL_SPEED;
		if (!tileMap->collisionMoveDown(getHitbox(), &position.y, quadSize.y))
		{
			platforms->collisionMoveDown(getHitbox(), &position.y, quadSize.y);
		}
		if (tileMap->collisionMoveLeft({ position + glm::vec2(8.f, 0.f), position + glm::vec2(quadSize-1) }, &position.x))
		{
			sprite->changeAnimation(JUMP);
		}
		else
		{
			glm::vec2 dist = getDistEnemyToPlayer();
			if (dist.x < 44.f && dist.x > 0.f && abs(dist.y) < 16.f)
			{
				sprite->changeAnimation(ATTACK);
				soundmade = false;
			}
			else
			{
				position.x -= SPEED;
				setPosition(position);
			}
		}
	}
	else if (animation == JUMP && frame == 8)
	{
		jumping = true;
		velocityX = jumpVelocityX;
		velocityY = jumpVelocityY;
	}
	else if (jumping)
	{
		velocityY -= gravity;
		position.x -= velocityX;
		position.y -= velocityY;

		jumping = velocityY > 0 || (!tileMap->collisionMoveDown(getHitbox(), &position.y, quadSize.y) && !platforms->collisionMoveDown(getHitbox(), &position.y, quadSize.y));

		setPosition(position);
	}
	else if (animation == ATTACK && frame == 4 && !soundmade)
	{
		SoundEngine::instance().playSFX(SoundEngine::ENEMY_ATTACK);
		attacking = true;
		soundmade = true;
	}
	sprite->update(deltaTime);
}

const Hitbox Skeleton1::getHitbox() const
{
	Hitbox hb;
	hb.min = position + glm::vec2(18-18*attacking,6);
	hb.max = hb.min + glm::vec2(20, 44);
	return hb;
}
