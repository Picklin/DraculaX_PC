#pragma once
#ifndef _SKELETON1_INCLUDE
#define _SKELETON1_INCLUDE

#include "Enemy.h"

class Skeleton1 : public Enemy
{
public:
	const glm::vec2 myCenter() const override;

protected:
	string getName() const override;
	const string getSpritesheet() const override;
	const glm::vec2 getSizeInSpritesheet() const override;
	const glm::ivec2 getQuadSize() const override;
	void setAnimations() override;
	int setEndAnimation() override;
	int setEndTime() { return 1000; } //en milisegundos
	void makeEndSound() const override;
	void makeEndEffect() const override;
	void childUpdate(int deltaTime) override;
	int setMaxHealth() { return 5; }
	const Hitbox getHitbox() const override;

private:
	const float jumpVelocityX = 2.f;
	const float jumpVelocityY = 3.f;
	const float gravity = 0.2f;
	float velocityX;
	float velocityY;
	bool jumping = false;
	bool soundmade = false;
};

#endif // !_SKELETON1_INCLUDE