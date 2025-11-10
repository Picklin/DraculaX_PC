#include "EnemyManager.h"

EnemyManager::EnemyManager() : shader(nullptr), map(nullptr), platforms(nullptr), playerPos(nullptr)
{
    
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

void EnemyManager::initEnemy(Enemy& e, const glm::vec2& pos)
{
    e.init(tileMapDispl, *shader);
    e.setPosition(pos);
    e.setPlayerPos(playerPos);
    e.setPlayerCenter(playerCenter);
}
