#pragma once
#ifndef _ENEMY_INCLUDE
#define _ENEMY_INCLUDE

#include "EffectsManager.h"
#include "ItemManager.h"

class Enemy : public Entity
{
public:
	virtual void init(const glm::ivec2& tileMapDispl, ShaderProgram& shader);
	virtual void render();
	virtual void end();
	void setPlayerPos(glm::vec2* playerPos);
	void setPlayerCenter(const glm::vec2& playerCenter) { this->playerCenter = playerCenter; }
	virtual const vector<Hitbox> getVulnerableArea() const;
	virtual const pair<bool, vector<Hitbox>> getSweetspot();
	virtual void sweetspotHitAction() {};
	virtual void takeDmg(int dmg);
	virtual int getDamageFromAttack() const { return 10; }
	int getContactDamage() const { return contactDamage; }
	virtual int getScore() const { return 50; }
	virtual Item* getDrop() const { return ItemManager::instance().getCommonRandomItem(position + myCenter()); }
	bool isAttacking() const { return attacking; }
	//cuando ha sido eliminado de la memoria
	virtual Effect* makeDeathEffect() const = 0;
	virtual void makeProjectileSound() const {};

protected:
	virtual int setMaxHealth() = 0;
	virtual int setContactDamage() { return 10; }
	virtual void makeHurtSound() const {}
	//enemyPos - playerPos
	const glm::vec2 getDistEnemyToPlayer() const;

protected:
	int MAXHEALTH;
	int contactDamage;
	int currentHealth;
	glm::vec2* playerPos = nullptr;
	glm::vec2 playerCenter;
	bool attacking = false;
	bool renderAltern = true;
	bool flicker = false;
	float shootCooldown;
	enum typeProj { GOLEM };
};

#endif // !_ENEMY_INCLUDE