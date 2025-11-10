#include "SceneTesting.h"
#include "Game.h"


void SceneTesting::initItems()
{
}

void SceneTesting::initActors(Player* player)
{
	this->player = player;
	player->setTileMap(map);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
}

void SceneTesting::update(int deltaTime)
{
	Scene::update(deltaTime);
}

TileMap* SceneTesting::setTileMap()
{
	return TileMap::createTileMap("levels/level02.txt", MAP_OFFSET, *texProgram);;
}