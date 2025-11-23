#include "Level1Sc1.h"

void Level1Sc1::init(Player& player, ShaderProgram& spriteShader, ShaderProgram& basicShader) 
{
	Scene::init(player, spriteShader, basicShader);
}
void Level1Sc1::update(int deltaTime) 
{
	Scene::update(deltaTime);
}
void Level1Sc1::render() 
{
	Scene::render();
}

TileMap* Level1Sc1::setTileMap() 
{
	return TileMap::createTileMap("levels/level02.txt", MAP_OFFSET, *spriteShader);
}
TileMap* Level1Sc1::setPlatformMap() 
{
	return nullptr;
}
void Level1Sc1::initItems() 
{

}
void Level1Sc1::initActors(Player* player) 
{
	this->player = player;
	player->setTileMap(map);
	player->setPosition(glm::vec2(8 * map->getTileSize(), 16 * map->getTileSize()));
}
void Level1Sc1::updateCamera() 
{
	Scene::updateCamera();
}
const pair<int, int> Level1Sc1::setNewLevelAndScene() const 
{
	return pair<int, int>(-1, -1);
}