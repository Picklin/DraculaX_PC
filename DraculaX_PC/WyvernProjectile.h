#pragma once
#ifndef _WYVERNPROJECTILE_INCLUDE
#define _WYVERNPROJECTILE_INCLUDE

#include "Projectile.h"
#include "TileMap.h"

class WyvernProjectile : public Projectile
{
public:
	WyvernProjectile();
	void setTileMap(TileMap& tileMap);
	void setShader(ShaderProgram& shader);
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
	float setEndTime() override;

private:
	TileMap* tileMap;
	ShaderProgram* shader;
	Hitbox hitboxes[2];
	int bounceAngle;
	int bounceYDist;
	const int bounceYDists[5];
	int startY;
	float bounceXSpeed;
	const float xSpeeds[5];
	bool bouncing = false;
	bool renderAlternate = true;
};

#endif // !_WYVERNPROJECTILE_INCLUDE