#pragma once
#ifndef _PROJECTILE_INCLUDE
#define _PROJECTILE_INCLUDE

#include "Entity.h"
class Projectile : public Entity
{
public:
	virtual void setPosition(const glm::vec2& pos, const glm::vec2& dir);
	virtual int getDamage() const = 0;

protected:
	virtual int setSpeed() { return 8; }
	virtual void childUpdate(int deltaTime);

protected:
	int speed;
	glm::vec2 dir;
};

#endif _PROJECTILE