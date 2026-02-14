#pragma once
#ifndef _PROJECTILEMANAGER_INCLUDE
#define _PROJECTILEMANAGER_INCLUDE

#include "Projectile.h"
#include "TileMap.h"
#include <functional>

class ProjectileManager
{
public:
	static ProjectileManager& instance();
	void init(const glm::ivec2& tileMapDispl, ShaderProgram& program, TileMap* map, vector<Projectile*>* projectiles);
	void createEnemyProjectile(const glm::vec2& pos, const glm::vec2& dir, int projNum);
	Projectile* getEnemyProjectile(const glm::vec2& pos, const glm::vec2& dir, int projNum);

public:
	enum typeProj { GOLEM, WYVERN };

private:
	ProjectileManager();
	Projectile* getGolemProjectile(const glm::vec2& pos, const glm::vec2& dir);
	Projectile* getWyvernProjectile(const glm::vec2& pos, const glm::vec2& dir);

private:
	vector<Projectile*>* projectiles;
	glm::ivec2 tileMapDispl;
	TileMap* map;
	ShaderProgram* program;
	using ProjectileCreator = std::function<Projectile* (const glm::vec2&, const glm::vec2&)>;
	vector<ProjectileCreator> enemyProjectiles;
	bool alternator = false;
};

#endif // !_PROJECTILEMANAGER_INCLUDE