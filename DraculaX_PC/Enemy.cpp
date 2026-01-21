#include "Enemy.h"

void Enemy::init(const glm::ivec2& tileMapDispl, ShaderProgram& shader)
{
	Entity::init(tileMapDispl, shader);
	MAXHEALTH = setMaxHealth();
	contactDamage = setContactDamage();
	currentHealth = MAXHEALTH;
}

void Enemy::render()
{
	if (flicker) renderAltern = !renderAltern;
	if (renderAltern)
	{
		sprite->render();
	}
}

void Enemy::end()
{
	Entity::end();
}

void Enemy::setPlayerPos(glm::vec2* playerPos)
{
	this->playerPos = playerPos;
}

const Hitbox Enemy::getVulnerableArea() const
{
	return getHitbox();
}

const pair<bool, Hitbox> Enemy::getSweetspot()
{
	return pair<bool, Hitbox>(false, {});
}

void Enemy::takeDmg(int dmg)
{
	currentHealth -= dmg;
	makeHurtSound();
	if (currentHealth <= 0) {
		end();
	}
	sprite->invertColor();
	timeWounded = setWoundedCooldown();
}

bool Enemy::wounded() const
{
	return timeWounded >= 0;
}

int Enemy::setWoundedCooldown()
{
	return 1000;
}

const glm::vec2 Enemy::getDistEnemyToPlayer() const
{
	glm::vec2 center = myCenter();
	float Xdist = (position.x + center.x) - (playerPos->x + playerCenter.x);
	float Ydist = (position.y + center.y) - (playerPos->y + playerCenter.y);
	return glm::vec2(Xdist, Ydist);
}
