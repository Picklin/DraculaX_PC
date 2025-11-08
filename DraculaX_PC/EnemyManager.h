#pragma once
#ifndef _ENEMYMANAGER_INCLUDE
#define _ENEMYMANAGER_INCLUDE

#include "Enemy.h"
#include <functional>

enum EnemyId
{
	ENDOSKELETON, GOLEM, GUNNER, SNIPER, SNIPER2
};

class EnemyManager
{
public:
	static EnemyManager& instance();
	void init(glm::ivec2& tileMapDispl, ShaderProgram& program, TileMap* map, TileMap* platforms);
	void setPlayer(glm::vec2* playerPos, const glm::vec2& playerCenter);
	Enemy* getEnemy(const glm::vec2& pos, int id);

private:
	EnemyManager();
	void initEnemy(Enemy& e, const glm::vec2& pos);

private:
	glm::ivec2 tileMapDispl;
	ShaderProgram* program;
	TileMap* map;
	TileMap* platforms;
	glm::vec2* playerPos;
	glm::vec2 playerCenter;

	using EnemyCreator = std::function<Enemy* (const glm::vec2&)>;
	vector<EnemyCreator> enemyCreator;
};

#endif // !_ENEMYMANAGER_INCLUDE