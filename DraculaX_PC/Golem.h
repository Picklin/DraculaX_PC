#pragma once
#ifndef _GOLEM_INCLUDE
#define _GOLEM_INCLUDE

#include "Enemy.h"

class Golem : public Enemy
{
public:
	void init(glm::ivec2& tileMapDispl, ShaderProgram& program);
	void render();
	void setPosition(const glm::vec2& pos);
	const glm::vec2 myCenter() const;
	const Hitbox getHitbox() const;
	const Hitbox getVulnerableArea() const;
	int getDamageFromAttack() const { return 20; }
	int getScore() const { return 100; }
	const glm::vec2 getPointDirection() const;

protected:
	string getName() const;
	const string getSpritesheet() const;
	const glm::vec2 getSizeInSpritesheet() const;
	const glm::ivec2 getQuadSize() const;
	void setAnimations();
	int setEndTime() { return 1536; }
	void makeEndSound() const;
	void makeEndEffect() const;
	void childUpdate(int deltaTime);
	int setMaxHealth() { return 35; }
	int setContactDamage() { return 10; }
	void makeHurtSound() const;

private:
	const glm::vec2 getRotatedDirection(const glm::vec2& point, float angle) const;

private:
	Texture legsTex;
	Sprite* legs;

	float bodyTemp = 1.f;
	float punchCooldown = 0.f;
	bool shot = false;
	bool soundmade = false;

	vector<glm::vec2> fragPositions;
	vector<pair<glm::vec2, glm::vec2>> fragCoordPositions;
};

#endif // !_GOLEM_INCLUDE