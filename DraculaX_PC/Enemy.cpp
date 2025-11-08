#include "Enemy.h"

void Enemy::init(const glm::ivec2& tileMapDispl, ShaderProgram& program)
{
	Entity::init(tileMapDispl, program);
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

const vector<Hitbox> Enemy::getVulnerableArea() const
{
	return getHitbox();
}

const pair<bool, vector<Hitbox>> Enemy::getSweetspot()
{
	return pair<bool, vector<Hitbox>>(false, {});
}

void Enemy::takeDmg(int dmg)
{
	currentHealth -= dmg;
	makeHurtSound();
	if (currentHealth <= 0) {
		end();
	}
	sprite->white();
}

const glm::vec2 Enemy::getDistEnemyToPlayer() const
{
	glm::vec2 center = myCenter();
	float Xdist = (position.x + center.x) - (playerPos->x + playerCenter.x);
	float Ydist = (position.y + center.y) - (playerPos->y + playerCenter.y);
	return glm::vec2(Xdist, Ydist);
}
