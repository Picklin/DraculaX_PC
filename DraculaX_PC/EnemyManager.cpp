#include "EnemyManager.h"

EnemyManager::EnemyManager() : shader(nullptr), map(nullptr), platforms(nullptr), playerPos(nullptr)
{
    enemyCreator.push_back([this](const glm::vec2& pos) {return this->createSkeleton1(pos); });
    enemyCreator.push_back([this](const glm::vec2& pos) {return this->createGolem(pos); });
}

EnemyManager& EnemyManager::instance()
{
    static EnemyManager em;
    return em;
}

void EnemyManager::init(glm::ivec2& tileMapDispl, ShaderProgram& shader, TileMap* map, TileMap* platforms)
{
    this->tileMapDispl = tileMapDispl;
    this->shader = &shader;
    this->map = map;
    this->platforms = platforms;
}

void EnemyManager::setPlayer(glm::vec2* playerPos, const glm::vec2& playerCenter)
{
    this->playerPos = playerPos;
    this->playerCenter = playerCenter;
}

Enemy* EnemyManager::getEnemy(const glm::vec2& pos, int id)
{
    return enemyCreator[id](pos);
}

Enemy* EnemyManager::createSkeleton1(const glm::vec2& pos)
{
    Skeleton1* t800 = new Skeleton1();
    initEnemy(*t800, pos);
    t800->setTileMap(map);
    t800->setPlatforms(platforms);
    return t800;
}

Enemy* EnemyManager::createGolem(const glm::vec2& pos)
{
    Golem* g = new Golem();
    g->init(tileMapDispl, *shader);
    g->setPosition(pos);
    g->setPlayerPos(playerPos);
    g->setPlayerCenter(playerCenter);
    return g;
}

void EnemyManager::initEnemy(Enemy& e, const glm::vec2& pos)
{
    e.init(tileMapDispl, *shader);
    e.setPosition(pos);
    e.setPlayerPos(playerPos);
    e.setPlayerCenter(playerCenter);
}
