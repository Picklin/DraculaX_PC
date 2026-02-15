#pragma once
#ifndef _PROJECTILE_INCLUDE
#define _PROJECTILE_INCLUDE

#include "Sprite.h"
#include "Hitbox.h"
#include "TileMap.h"

class Projectile
{
public:
	void setTileMap(TileMap& tileMap);
	void setShader(ShaderProgram& shader);
	void init(const glm::ivec2& tileMapDispl, ShaderProgram& shaderProgram, const glm::vec2& dir);
	void update(int deltaTime);
	virtual void render();
	void setPosition(const glm::vec2& pos);
	void end();
	virtual int getDamage() const = 0;
	virtual const Hitbox getHitbox() const = 0;
	const glm::vec2& getPosition() const;
	bool isEnded() const;
	virtual bool getsRemoved() const;
	virtual bool destroyable() const { return true; }

protected:
	virtual const string getSpritesheet() const = 0;
	virtual const glm::vec2 getSizeInSpritesheet() const = 0;
	virtual const glm::ivec2 getQuadSize() const = 0;
	virtual void childUpdate(int deltaTime) = 0;
	virtual void setAnimations() = 0;
	virtual glm::vec2 setSpeed() = 0;
	virtual float setEndTime();
	virtual int setEndAnimation() const;

protected:
	glm::ivec2 tileMapDispl;
	glm::vec2 position;
	glm::vec2 dir;
	glm::vec2 speed;
	Texture* spritesheet;
	Sprite* sprite;
	TileMap* tileMap;
	ShaderProgram* shader;
	float endTimer;
	bool ended = false;
};

#endif _PROJECTILE