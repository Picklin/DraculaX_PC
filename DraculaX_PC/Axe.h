#pragma once
#ifndef _AXE_INCLUDE
#define _AXE_INCLUDE

#include "Subweapon.h"
class Axe : public Subweapon
{
public:
	void init(const glm::ivec2& tileMapDispl, ShaderProgram& shaderProgram, int lookingDirection) override;
	void update(int deltaTime) override;
	void render() override;
	int getDamage() const override;
	const Hitbox getHitbox() const override;
	bool getsRemoved() const override;

private:
	int timeElapsed = 0;
	float velocityY;
};

#endif // !_AXE_INCLUDE