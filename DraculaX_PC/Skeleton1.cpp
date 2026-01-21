#include "Skeleton1.h"
#include "SoundEngine.h"
#define SPEED 0.25f

enum anims
{
	WALK, JUMP, ATTACK, L_DIE, HEADSHOT
};

const glm::vec2 Skeleton1::myCenter() const
{
	return glm::vec2(38.f, 25.f);
}

string Skeleton1::getName() const
{
	return "Endoskeleton";
}

const string Skeleton1::getSpritesheet() const
{
	return "images/enemies/endoskeleton/endoskeleton_green.png";
}

const glm::vec2 Skeleton1::getSizeInSpritesheet() const
{
	return glm::vec2(0.1f, 0.25f);
}

const glm::ivec2 Skeleton1::getQuadSize() const
{
	return glm::ivec2(64, 50);
}

void Skeleton1::setAnimations()
{
	sprite->setNumberAnimations(5);

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

	sprite->setAnimationSpeed(L_DIE, 8);
	for (int i = 0; i < 5; i++) sprite->animatorX(L_DIE, 2, 0.f, 0.1f, 0.75f);

	sprite->setAnimationSpeed(HEADSHOT, 0);
	sprite->addKeyframe(HEADSHOT, glm::vec2(0.2f, 0.75f));

	sprite->setTransition(ATTACK, WALK);
	sprite->setTransition(JUMP, WALK);

	sprite->changeAnimation(WALK);
}

int Skeleton1::setEndAnimation()
{
	return L_DIE;
}

void Skeleton1::makeEndSound() const
{
	SoundEngine::instance().playSFX(SoundEngine::EXPLOSION_MINI);
}

void Skeleton1::makeEndEffect() const
{
	EffectsManager::instance().createMiniExplosions(&position, myCenter(), 21, 32, 32, glm::vec4(1.f));
}

void Skeleton1::childUpdate(int deltaTime)
{
	int animation = sprite->animation();
	int frame = sprite->getCurrentKeyframe();
	if (animation == WALK)
	{
		attacking = false;
		jumping = false;
		if (tileMap->collisionMoveLeft({ position + glm::vec2(8.f, 0.f), position + glm::vec2(quadSize) }, &position.x))
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

		jumping = velocityY > 0 || !tileMap->collisionMoveDown(getHitbox(), &position.y, quadSize.y);

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
	hb.min = position;
	hb.max = hb.min + glm::vec2(quadSize);
	return hb;
}
