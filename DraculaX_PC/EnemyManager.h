#pragma once
#ifndef _ENEMYMANAGER_INCLUDE
#define _ENEMYMANAGER_INCLUDE

#include "Skeleton1.h"
#include "Golem.h"
#include <functional>

class EnemyManager
{
public:
	static EnemyManager& instance();
	void init(const glm::ivec2& tileMapDispl, ShaderProgram& shader, TileMap* map, TileMap* platforms);
	void setPlayer(glm::vec2* playerPos, const glm::vec2& playerCenter);
	Enemy* getEnemy(const glm::vec2& pos, int id);

private:
	EnemyManager();
	void initEnemy(Enemy& e, const glm::vec2& pos);
	Enemy* createSkeleton1(const glm::vec2& pos);
	Enemy* createGolem(const glm::vec2& pos);

public:
	enum EnemyId
	{
		SKELETON1, GOLEM,
	};

private:
	glm::ivec2 tileMapDispl;
	ShaderProgram* shader;
	TileMap* map;
	TileMap* platforms;
	glm::vec2* playerPos;
	glm::vec2 playerCenter;

	using EnemyCreator = std::function<Enemy* (const glm::vec2&)>;
	vector<EnemyCreator> enemyCreator;
};

#endif // !_ENEMYMANAGER_INCLUDE