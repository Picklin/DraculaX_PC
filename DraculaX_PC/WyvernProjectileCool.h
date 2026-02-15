#pragma once
#ifndef _WYVERNPROJECTILECOOL_INCLUDE
#define _WYVERNPROJECTILECOOL_INCLUDE

#include "Projectile.h"

class WyvernProjectileCool : public Projectile
{
public:
	void render() override;
	int getDamage() const override;
	const Hitbox getHitbox() const override;

protected:
	const string getSpritesheet() const override;
	const glm::vec2 getSizeInSpritesheet() const override;
	const glm::ivec2 getQuadSize() const override;
	void childUpdate(int deltaTime) override;
	void setAnimations() override;
	glm::vec2 setSpeed() override;

private:
	bool grounded = false;
	bool renderAlternate = true;
};

#endif // !_WYVERNPROJECTILECOOL_INCLUDE