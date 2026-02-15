#include "ProjectileManager.h"
#include "GolemProjectile.h"
#include "WyvernProjectile.h"
#include "WyvernProjectileCool.h"

ProjectileManager& ProjectileManager::instance()
{
    static ProjectileManager pm;
    return pm;
}

ProjectileManager::ProjectileManager() : map(nullptr), program(nullptr), projectiles(nullptr)
{
    enemyProjectiles.push_back([this](const glm::vec2& pos, const glm::vec2& dir) {return getGolemProjectile(pos, dir); });
	enemyProjectiles.push_back([this](const glm::vec2& pos, const glm::vec2& dir) {return getWyvernProjectile(pos, dir); });
	enemyProjectiles.push_back([this](const glm::vec2& pos, const glm::vec2& dir) {return getWyvernProjectileCool(pos, dir); });
}

void ProjectileManager::init(const glm::ivec2& tileMapDispl, ShaderProgram& program, TileMap* map, vector<Projectile*>* projectiles)
{
    this->tileMapDispl = tileMapDispl;
    this->program = &program;
    this->map = map;
    this->projectiles = projectiles;
}

void ProjectileManager::createEnemyProjectile(const glm::vec2& pos, const glm::vec2& dir, int projNum)
{
	projectiles->push_back(getEnemyProjectile(pos, dir, projNum));
}

Projectile* ProjectileManager::getGolemProjectile(const glm::vec2& pos, const glm::vec2& dir)
{
    GolemProjectile* gp = new GolemProjectile();
    gp->init(tileMapDispl, *program, dir);
    gp->setTileMap(*map);
    gp->setPosition(pos);
    return gp;
}

Projectile* ProjectileManager::getWyvernProjectile(const glm::vec2& pos, const glm::vec2& dir)
{
	WyvernProjectile* wp = new WyvernProjectile();
	wp->init(tileMapDispl, *program, dir);
	wp->setTileMap(*map);
	wp->setShader(*program);
	wp->setPosition(pos);
	return wp;
}

Projectile* ProjectileManager::getWyvernProjectileCool(const glm::vec2& pos, const glm::vec2& dir)
{
	WyvernProjectileCool* wpc = new WyvernProjectileCool();
	wpc->init(tileMapDispl, *program, dir);
	wpc->setTileMap(*map);
	wpc->setShader(*program);
	wpc->setPosition(pos);
	return wpc;
}

Projectile* ProjectileManager::getEnemyProjectile(const glm::vec2& pos, const glm::vec2& dir, int projNum)
{
    return enemyProjectiles[projNum](pos, dir);
}
