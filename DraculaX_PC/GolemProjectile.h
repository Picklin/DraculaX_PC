#pragma once
#ifndef _GOLEMPROJECTILE_INCLUDE
#define _GOLEMPROJECTILE_INCLUDE

#include "Projectile.h"

class GolemProjectile : public Projectile
{
public:
	const virtual Hitbox getHitbox() const;
	int getDamage() const override;

protected:
	string getName() const;
	const string getSpritesheet() const;
	const glm::vec2 getSizeInSpritesheet() const;
	const glm::ivec2 getQuadSize() const;
	void setAnimations();
	int setSpeed() { return 1; }
};

#endif // !_GOLEMPROJECTILE_INCLUDE