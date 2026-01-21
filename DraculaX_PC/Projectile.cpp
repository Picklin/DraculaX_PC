#include "Projectile.h"

void Projectile::setPosition(const glm::vec2& pos, const glm::vec2& dir)
{
	position = pos;
	this->dir = dir;
	speed = setSpeed();
	setPivotPoint(myCenter());
	Entity::setPosition(position);
}

void Projectile::childUpdate(int deltaTime)
{
	sprite->update(deltaTime);
	if (tileMap->collisionMoveLeft(getHitbox(), &position.x) || tileMap->collisionMoveRight(getHitbox(), &position.x) || tileMap->collisionMoveDown(getHitbox(), &position.y, quadSize.y) || tileMap->collisionMoveUp(getHitbox(), &position.y)) end();
	else
	{
		position.x += dir.x * speed;
		position.y += dir.y * speed;
		Entity::setPosition(position);
	}
}
