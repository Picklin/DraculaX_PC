#pragma once
#ifndef _WYVERNPROJECTILE_INCLUDE
#define _WYVERNPROJECTILE_INCLUDE

#include "Projectile.h"

class WyvernProjectile : public Projectile
{
public:
	int getDamage() const override;
	const Hitbox getHitbox() const override;

protected:
	const string getSpritesheet() const override;
	const glm::vec2 getSizeInSpritesheet() const override;
	const glm::ivec2 getQuadSize() const override;
	void childUpdate(int deltaTime) override;
	void setAnimations() override;
	glm::vec2 setSpeed() override;
	float setEndTime() override;
	int setEndAnimation() const override;

private:
	Hitbox hitboxes[2];
	int bounceAngle;
	int bounceYDist;
	float bounceXSpeed;
	bool bouncing = false;
};

#endif // !_WYVERNPROJECTILE_INCLUDE