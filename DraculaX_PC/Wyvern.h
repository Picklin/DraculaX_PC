#pragma once
#ifndef _WYVERN_INCLUDE
#define _WYVERN_INCLUDE

#include "Sprite.h"
#include "Hitbox.h"

class Wyvern
{
public:
	void init(const glm::ivec2& tileMapDispl, ShaderProgram& shader);
	void setPosition(const glm::vec2& pos);
	void update(int deltaTime);
	void render();
	void takeDmg(int dmg);
	const Hitbox getHitbox() const;
	int getPoints() const;
	bool isEnded() const;
	bool isRemoved() const;

private:
	Texture tex;
	glm::ivec2 tileMapDispl;
	glm::ivec2 position;
	Sprite* sprite;
	int currentHP;
	bool appeared;
	bool attacking;
	bool firing;
	bool ended;
	bool removed;
};

#endif // !_WYVERN_INCLUDE