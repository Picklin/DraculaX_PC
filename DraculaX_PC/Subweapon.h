#pragma once
#ifndef _SUBWEAPON_INCLUDE
#define _SUBWEAPON_INCLUDE

#include "Sprite.h"
#include "Hitbox.h"

class Subweapon
{
public:
	~Subweapon();
	virtual void init(const glm::ivec2& tileMapDispl, ShaderProgram& shaderProgram, int lookingDirection) = 0;
	virtual void update(int deltaTime) = 0;
	virtual void render() = 0;
	void setPosition(const glm::vec2& pos);
	virtual int getDamage() const = 0;
	virtual const Hitbox getHitbox() const = 0;
	virtual bool getsRemoved() const = 0;

protected:
	glm::ivec2 quadSize;
	glm::ivec2 tileMapDispl;
	glm::vec2 position;
	Texture* tex;
	Sprite* sprite;
	ShaderProgram* shader;
	int lookingDirection;
};

#endif // !_SUBWEAPON_INCLUDE