#pragma once
#ifndef _GOLEMPROJECTILE_INCLUDE
#define _GOLEMPROJECTILE_INCLUDE

#include "Projectile.h"
#include "TileMap.h"

class GolemProjectile : public Projectile
{
public:
	void setTileMap(TileMap* tileMap);
	int getDamage() const override;
	const virtual Hitbox getHitbox() const;
	bool getsRemoved() const override;

protected:
	const string getSpritesheet() const override;
	const glm::vec2 getSizeInSpritesheet() const override;
	const glm::ivec2 getQuadSize() const override;
	void childUpdate(int deltaTime) override;
	void setAnimations();
	glm::vec2 setSpeed() override;
	int setEndAnimation() const override;

private:
	TileMap* tileMap;
};

#endif // !_GOLEMPROJECTILE_INCLUDE