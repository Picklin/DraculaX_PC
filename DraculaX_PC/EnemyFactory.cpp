#include "EnemyFactory.h"
#include "Skeleton1.h"
#include "Golem.h"

EnemyFactory::EnemyFactory() : shader(nullptr), map(nullptr), platforms(nullptr), playerPos(nullptr)
{
}

EnemyFactory& EnemyFactory::instance()
{
    static EnemyFactory em;
    return em;
}

void EnemyFactory::init(const glm::ivec2& tileMapDispl, ShaderProgram& shader, TileMap* map, TileMap* platforms)
{
    this->tileMapDispl = tileMapDispl;
    this->shader = &shader;
    this->map = map;
    this->platforms = platforms;
}

void EnemyFactory::setPlayer(glm::vec2* playerPos, const glm::vec2& playerCenter)
{
    this->playerPos = playerPos;
    this->playerCenter = playerCenter;
}

Enemy* EnemyFactory::getEnemy(const glm::vec2& pos, int id)
{
    Enemy* e;
    switch (id)
    {
    case SKELETON1:
        e = new Skeleton1();
        initEnemy(*e, pos);
        e->setTileMap(map);
        e->setPlatforms(platforms);
        return e;
    case GOLEM:
        e = new Golem();
		initEnemy(*e, pos);
        return e;
    default:
        return nullptr;
	}
}

void EnemyFactory::initEnemy(Enemy& e, const glm::vec2& pos)
{
    e.init(tileMapDispl, *shader);
    e.setPosition(pos);
    e.setPlayerPos(playerPos);
    e.setPlayerCenter(playerCenter);
}
