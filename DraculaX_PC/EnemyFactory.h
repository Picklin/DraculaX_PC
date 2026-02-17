#pragma once
#ifndef _ENEMYFACTORY_INCLUDE
#define _ENEMYFACTORY_INCLUDE

#include "Enemy.h"

class EnemyFactory
{
public:
	static EnemyFactory& instance();
	void init(const glm::ivec2& tileMapDispl, ShaderProgram& shader, TileMap* map, TileMap* platforms);
	void setPlayer(glm::vec2* playerPos, const glm::vec2& playerCenter);
	Enemy* getEnemy(const glm::vec2& pos, int id);

private:
	EnemyFactory();
	void initEnemy(Enemy& e, const glm::vec2& pos);

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
};

#endif // !_ENEMYMANAGER_INCLUDE