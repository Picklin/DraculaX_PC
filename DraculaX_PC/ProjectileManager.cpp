#include "ProjectileManager.h"

ProjectileManager& ProjectileManager::instance()
{
    static ProjectileManager pm;
    return pm;
}

ProjectileManager::ProjectileManager() : map(nullptr), program(nullptr), projectiles(nullptr)
{
    enemyProjectiles.push_back([this](const glm::vec2& pos, const glm::vec2& dir) {return getGolemProjectile(pos, dir); });
}

void ProjectileManager::init(glm::ivec2& tileMapDispl, ShaderProgram& program, TileMap* map, vector<Projectile*>* projectiles)
{
    this->tileMapDispl = tileMapDispl;
    this->program = &program;
    this->map = map;
    this->projectiles = projectiles;
}

void ProjectileManager::createGolemProjectile(const glm::vec2& pos, const glm::vec2& dir)
{
    projectiles->push_back(getGolemProjectile(pos, dir  ));
}

Projectile* ProjectileManager::getGolemProjectile(const glm::vec2& pos, const glm::vec2& dir)
{
    GolemProjectile* gp = new GolemProjectile();
    gp->init(tileMapDispl, *program);
    gp->setTileMap(map);
    gp->setPosition(pos, dir);
    return gp;
}

Projectile* ProjectileManager::getEnemyProjectile(const glm::vec2& pos, const glm::vec2& dir, int projNum)
{
    return enemyProjectiles[projNum](pos, dir);
}
