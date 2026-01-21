#pragma once
#ifndef _ENEMY_INCLUDE
#define _ENEMY_INCLUDE

#include "EffectsManager.h"
#include "Hitbox.h"
#include "Entity.h"

class Enemy : public Entity
{
public:
	virtual void init(const glm::ivec2& tileMapDispl, ShaderProgram& shader);
	virtual void render();
	virtual void end();
	void setPlayerPos(glm::vec2* playerPos);
	void setPlayerCenter(const glm::vec2& playerCenter) { this->playerCenter = playerCenter; }
	virtual const Hitbox getVulnerableArea() const;
	virtual const pair<bool, Hitbox> getSweetspot();
	virtual void sweetspotHitAction() {};
	virtual void takeDmg(int dmg);
	virtual int getDamageFromAttack() const { return 10; }
	int getContactDamage() const { return contactDamage; }
	virtual int getScore() const { return 50; }
	bool isAttacking() const { return attacking; }
	bool wounded() const;
	virtual void makeProjectileSound() const {};

protected:
	virtual int setMaxHealth() = 0;
	virtual int setContactDamage() { return 10; }
	virtual int setWoundedCooldown();
	virtual void makeHurtSound() const {}
	//enemyPos - playerPos
	const glm::vec2 getDistEnemyToPlayer() const;

protected:
	glm::vec2 playerCenter;
	glm::vec2* playerPos = nullptr;
	int MAXHEALTH;
	int contactDamage;
	int currentHealth;
	int timeWounded = -1;
	float shootCooldown = 0.f;
	bool attacking = false;
	bool renderAltern = true;
	bool flicker = false;
};

#endif // !_ENEMY_INCLUDE