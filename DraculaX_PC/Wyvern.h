#pragma once
#ifndef _WYVERN_INCLUDE
#define _WYVERN_INCLUDE

#include "Sprite.h"
#include "Hitbox.h"

class Wyvern
{
public:
	~Wyvern();
	void init(const glm::ivec2& tileMapDispl, ShaderProgram& shader, const glm::vec2& pos);
	void setPosition(const glm::vec2& pos);
	void update(int deltaTime);
	void render();
	void takeDmg(int dmg);
	const Hitbox getHitbox() const;
	int getPoints() const;
	bool battleStarted() const;
	bool isWounded() const;
	bool isEnded() const;
	bool isRemoved() const;
	glm::vec2* getPosition() { return &position; }

private:
	void calcIncrement(float& valToInc, float targetVal, float factor);

private:
	Texture tex;
	glm::ivec2 tileMapDispl;
	glm::vec2 position;
	Sprite* sprite;
	ShaderProgram* shader;
	int currentHP;
	float startY;
	float lungeAngle;
	float lungeAngleStep;
	float lungeDist;
	float moveSpeed;
	float woundedCooldown;
	float attackCooldown;
	float alpha;
	float colorValue;
	float deathTimeElapsed;
	bool appeared;
	bool lunging;
	bool attacking;
	bool firing;
	bool ended;
	bool removed;
	bool flip;
};

#endif // !_WYVERN_INCLUDE