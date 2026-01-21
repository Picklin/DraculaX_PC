#include "Golem.h"
#include "ProjectileManager.h"
#include "SoundEngine.h"
#define OFFSET 56
#define SPEED .5f

enum GolemAnims
{
	IDLE, PUNCH, SHOOT
};

void Golem::init(glm::ivec2& tileMapDispl, ShaderProgram& program)
{
	legsTex = Texture();
	legsTex.loadFromFile("images/enemies/golem/golem_legs.png", TEXTURE_PIXEL_FORMAT_RGBA);
	legsTex.setMagFilter(GL_NEAREST);
	legs = Sprite::createSprite(glm::ivec2(208, 80), glm::vec2(0.2f, 0.25f), &legsTex, &program);
	Enemy::init(tileMapDispl, program);
	int numfrag = 12;
	fragPositions.reserve(numfrag);
	fragCoordPositions.reserve(numfrag);
	fragPositions.emplace_back(glm::vec2(72.f, 104.f));
	fragPositions.emplace_back(glm::vec2(143.f, 104.f));
	fragPositions.emplace_back(glm::vec2(84.f, 70.f));
	fragPositions.emplace_back(glm::vec2(73.f, 87.f));
	fragPositions.emplace_back(glm::vec2(136.f, 87.f));
	fragPositions.emplace_back(glm::vec2(92.f, 10.f));
	fragPositions.emplace_back(glm::vec2(124.f, 10.f));
	fragPositions.emplace_back(glm::vec2(92.f, 42.f));
	fragPositions.emplace_back(glm::vec2(124.f, 42.f));
	fragPositions.emplace_back(glm::vec2(99.f, 6.f));
	fragPositions.emplace_back(glm::vec2(129.f, 54.f));
	fragPositions.emplace_back(glm::vec2(134.f, 20.f));
	fragCoordPositions.emplace_back(glm::vec2(0.250f, 0.250f), glm::vec2(0.375f, 0.375f));	//11
	fragCoordPositions.emplace_back(glm::vec2(0.375f, 0.250f), glm::vec2(0.500f, 0.375f));	//12
	fragCoordPositions.emplace_back(glm::vec2(0.000f, 0.250f), glm::vec2(0.125f, 0.375f));	//9
	fragCoordPositions.emplace_back(glm::vec2(0.250f, 0.125f), glm::vec2(0.375f, 0.250f));	//7
	fragCoordPositions.emplace_back(glm::vec2(0.375f, 0.125f), glm::vec2(0.5f, 0.250f));	//8
	fragCoordPositions.emplace_back(glm::vec2(0.000f, 0.000f), glm::vec2(0.125f, 0.125f));	//1
	fragCoordPositions.emplace_back(glm::vec2(0.125f, 0.000f), glm::vec2(0.250f, 0.125f));	//2
	fragCoordPositions.emplace_back(glm::vec2(0.000f, 0.125f), glm::vec2(0.125f, 0.250f));	//5
	fragCoordPositions.emplace_back(glm::vec2(0.125f, 0.125f), glm::vec2(0.250f, 0.250f));	//6
	fragCoordPositions.emplace_back(glm::vec2(0.375f, 0.000f), glm::vec2(0.500f, 0.125f));	//4
	fragCoordPositions.emplace_back(glm::vec2(0.125f, 0.250f), glm::vec2(0.250f, 0.375f));	//10
	fragCoordPositions.emplace_back(glm::vec2(0.250f, 0.000f), glm::vec2(0.375f, 0.125f));	//3
}

void Golem::render()
{
	if (ended) renderAltern = !renderAltern;
	if (renderAltern)
	{
		legs->render();
		sprite->render();
	}
}

void Golem::setPosition(const glm::vec2& pos)
{
	this->position = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
	legs->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y + OFFSET)));
}

const glm::vec2 Golem::myCenter() const
{
	return glm::vec2(118.f, 70.f);
}

const Hitbox Golem::getHitbox() const
{
	Hitbox hb;
	hb.min = position;
	hb.max = hb.min + glm::vec2(quadSize);
	return hb;
}

const Hitbox Golem::getVulnerableArea() const
{
	Hitbox hb;

	return hb;
}

const glm::vec2 Golem::getPointDirection() const
{
	glm::vec2 dir;
	//dir = *playerPos + glm::vec2(16.f, 16.f) - (getProjPosition());
	//dir = glm::normalize(dir);
	dir = glm::vec2(-1, 1);
	return dir;
}

string Golem::getName() const
{
	return "Golem";
}

const string Golem::getSpritesheet() const
{
	return "images/enemies/golem/golem_torso.png";
}

const glm::vec2 Golem::getSizeInSpritesheet() const
{
	return glm::vec2(0.2f, 0.1f);
}

const glm::ivec2 Golem::getQuadSize() const
{
	return glm::ivec2(208, 112);
}

void Golem::setAnimations()
{
	sprite->setNumberAnimations(3);

	sprite->setAnimationSpeed(IDLE, 10);
	sprite->addKeyframe(IDLE, glm::vec2(0.f, 0.8f));
	sprite->addKeyframe(IDLE, glm::vec2(0.2f, 0.8f));
	sprite->addKeyframe(IDLE, glm::vec2(0.4f, 0.8f));
	sprite->addKeyframe(IDLE, glm::vec2(0.6f, 0.8f));
	sprite->addKeyframe(IDLE, glm::vec2(0.8f, 0.8f));
	sprite->addKeyframe(IDLE, glm::vec2(0.f, 0.9f));
	sprite->addKeyframe(IDLE, glm::vec2(0.2f, 0.9f));
	sprite->addKeyframe(IDLE, glm::vec2(0.4f, 0.9f));
	sprite->addKeyframe(IDLE, glm::vec2(0.6f, 0.9f));
	sprite->addKeyframe(IDLE, glm::vec2(0.8f, 0.9f));

	sprite->setAnimationSpeed(PUNCH, 24);
	sprite->addKeyframe(PUNCH, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(PUNCH, glm::vec2(0.2f, 0.f));
	sprite->addKeyframe(PUNCH, glm::vec2(0.4f, 0.f));
	sprite->addKeyframe(PUNCH, glm::vec2(0.6f, 0.f));
	sprite->addKeyframe(PUNCH, glm::vec2(0.8f, 0.f));
	sprite->addKeyframe(PUNCH, glm::vec2(0.f, 0.1f));
	sprite->addKeyframe(PUNCH, glm::vec2(0.2f, 0.1f));
	sprite->addKeyframe(PUNCH, glm::vec2(0.4f, 0.1f));
	sprite->addKeyframe(PUNCH, glm::vec2(0.6f, 0.1f));
	sprite->addKeyframe(PUNCH, glm::vec2(0.8f, 0.1f));
	sprite->addKeyframe(PUNCH, glm::vec2(0.f, 0.2f));
	sprite->addKeyframe(PUNCH, glm::vec2(0.2f, 0.2f));
	sprite->addKeyframe(PUNCH, glm::vec2(0.4f, 0.2f));
	sprite->addKeyframe(PUNCH, glm::vec2(0.6f, 0.2f));
	sprite->addKeyframe(PUNCH, glm::vec2(0.8f, 0.2f));
	sprite->addKeyframe(PUNCH, glm::vec2(0.f, 0.3f));
	sprite->addKeyframe(PUNCH, glm::vec2(0.2f, 0.3f));
	sprite->addKeyframe(PUNCH, glm::vec2(0.4f, 0.3f));
	sprite->addKeyframe(PUNCH, glm::vec2(0.6f, 0.3f));
	sprite->addKeyframe(PUNCH, glm::vec2(0.8f, 0.3f));
	sprite->addKeyframe(PUNCH, glm::vec2(0.f, 0.4f));
	sprite->addKeyframe(PUNCH, glm::vec2(0.2f, 0.4f));
	sprite->addKeyframe(PUNCH, glm::vec2(0.4f, 0.4f));
	sprite->addKeyframe(PUNCH, glm::vec2(0.6f, 0.4f));
	sprite->addKeyframe(PUNCH, glm::vec2(0.8f, 0.4f));
	sprite->addKeyframe(PUNCH, glm::vec2(0.f, 0.5f));
	sprite->addKeyframe(PUNCH, glm::vec2(0.2f, 0.5f));
	sprite->addKeyframe(PUNCH, glm::vec2(0.4f, 0.5f));
	sprite->addKeyframe(PUNCH, glm::vec2(0.6f, 0.5f));
	sprite->addKeyframe(PUNCH, glm::vec2(0.8f, 0.5f));

	sprite->setAnimationSpeed(SHOOT, 10);
	sprite->addKeyframe(SHOOT, glm::vec2(0.f, 0.6f));
	sprite->addKeyframe(SHOOT, glm::vec2(0.2f, 0.6f));
	sprite->addKeyframe(SHOOT, glm::vec2(0.4f, 0.6f));
	sprite->addKeyframe(SHOOT, glm::vec2(0.6f, 0.6f));
	sprite->addKeyframe(SHOOT, glm::vec2(0.8f, 0.6f));
	sprite->addKeyframe(SHOOT, glm::vec2(0.f, 0.7f));
	sprite->addKeyframe(SHOOT, glm::vec2(0.2f, 0.7f));
	sprite->addKeyframe(SHOOT, glm::vec2(0.4f, 0.7f));
	sprite->addKeyframe(SHOOT, glm::vec2(0.6f, 0.7f));

	sprite->setTransition(PUNCH, IDLE);
	sprite->setTransition(SHOOT, IDLE);

	sprite->changeAnimation(IDLE);

	legs->setNumberAnimations(1);
	legs->setAnimationSpeed(0, 8);
	legs->addKeyframe(0, glm::vec2(0.f, 0.f));
	legs->addKeyframe(0, glm::vec2(0.2f, 0.f));
	legs->addKeyframe(0, glm::vec2(0.4f, 0.f));
	legs->addKeyframe(0, glm::vec2(0.6f, 0.f));
	legs->addKeyframe(0, glm::vec2(0.8f, 0.f));
	legs->addKeyframe(0, glm::vec2(0.f, 0.25f));
	legs->addKeyframe(0, glm::vec2(0.2f, 0.25f));
	legs->addKeyframe(0, glm::vec2(0.4f, 0.25f));
	legs->addKeyframe(0, glm::vec2(0.6f, 0.25f));
	legs->addKeyframe(0, glm::vec2(0.8f, 0.25f));
	legs->addKeyframe(0, glm::vec2(0.f, 0.5f));
	legs->addKeyframe(0, glm::vec2(0.2f, 0.5f));
	legs->addKeyframe(0, glm::vec2(0.f, 0.5f));
	legs->addKeyframe(0, glm::vec2(0.8f, 0.25f));
	legs->addKeyframe(0, glm::vec2(0.6f, 0.25f));
	legs->addKeyframe(0, glm::vec2(0.4f, 0.25f));
	legs->addKeyframe(0, glm::vec2(0.2f, 0.25f));
	legs->addKeyframe(0, glm::vec2(0.f, 0.25f));
	legs->addKeyframe(0, glm::vec2(0.8f, 0.f));
	legs->addKeyframe(0, glm::vec2(0.6f, 0.f));
	legs->addKeyframe(0, glm::vec2(0.4f, 0.f));
	legs->addKeyframe(0, glm::vec2(0.2f, 0.f));
	legs->changeAnimation(0);
}

void Golem::makeEndSound() const
{
	SoundEngine::instance().playSFX(SoundEngine::ENEMY_BIG_DEATH);
}

void Golem::makeEndEffect() const
{
	EffectsManager::instance().createExplosions(&position, myCenter(), 8, 125, 64, glm::vec4(1.f));
	EffectsManager::instance().createExplosions(&position, myCenter(), 8, 125, 64, glm::vec4(1.f));
}

void Golem::childUpdate(int deltaTime)
{
	int anim = sprite->animation();
	if (anim == IDLE)
	{
		attacking = false;
		float xDist = position.x + myCenter().x - playerPos->x;
		if (xDist < 112.f) position.x += SPEED;
		else if (xDist > 144.f) position.x -= SPEED;
		float yDist = playerPos->y - position.y;
		if (xDist < 128.f && yDist <= 32.f && punchCooldown >= 4.f)
		{
			sprite->changeAnimation(PUNCH);
			attacking = true;
			punchCooldown = 0.f;
			soundmade = false;
			SoundEngine::instance().playSFX(SoundEngine::ENEMY_ATTACK);
		}
		else if (shootCooldown <= 0)
		{
			sprite->changeAnimation(SHOOT);
			shootCooldown = 4.f;
			soundmade = false;
		}
		shootCooldown -= deltaTime / 1000.f;
		punchCooldown += deltaTime / 1000.f;
	}
	else if (anim == PUNCH && sprite->getCurrentKeyframe() == 8 && !soundmade)
	{
		SoundEngine::instance().playSFX(SoundEngine::ENEMY_SWIPE);
		soundmade = true;
	}
	shot = (anim == SHOOT && sprite->getCurrentKeyframe() == 8 && !shot);
	if (shot)
	{
		glm::vec2 projPos = position + glm::vec2(105.f, 26.f);
		glm::vec2 projDir = getPointDirection();
		for (int i = 0; i < 3; i++)
		{
			ProjectileManager::instance().createGolemProjectile(projPos, getRotatedDirection(projDir, 20.f - 20.f * i));
		}
		if (!soundmade)
		{
			SoundEngine::instance().playSFX(SoundEngine::ENEMY_SHOOT1);
			soundmade = true;
		}
	}
	setPosition(position);
	sprite->setColor(glm::vec4(1.f, 1.f * bodyTemp, 1.f * bodyTemp, 1.f));
	legs->setColor(glm::vec4(1.f, 1.f * bodyTemp, 1.f * bodyTemp, 1.f));
	sprite->update(deltaTime);
	legs->update(deltaTime);
}

void Golem::makeHurtSound() const
{
	SoundEngine::instance().playSFX(SoundEngine::ENEMY_HURT);
}

const glm::vec2 Golem::getRotatedDirection(const glm::vec2& dir, float angle) const
{
	glm::mat4 matRotation = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.f, 0.f, 1.f));
	glm::vec2 rotatedDir = glm::vec2(matRotation * glm::vec4(dir, 0.f, 0.f));
	return rotatedDir;
}
