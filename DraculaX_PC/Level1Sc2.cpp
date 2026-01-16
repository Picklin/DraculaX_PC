#include "Level1Sc2.h"
#include "Game.h"

void Level1Sc2::init(Player& player, GUI& gui, ShaderProgram& spriteShader, ShaderProgram& basicShader)
{
	Scene::init(player, gui, spriteShader, basicShader);
	backgroundTexs.resize(1);
	backgroundTexs[0].loadFromFile("images/levels/lvl1/image.png", TEXTURE_PIXEL_FORMAT_RGBA);
	backgroundTexs[0].setMagFilter(GL_NEAREST);
	backgroundSprites.reserve(1);
	backgroundSprites.emplace_back(Sprite::createSprite(glm::ivec2(1024, 672), glm::vec2(1.f, 1.f), &backgroundTexs[0], &basicShader));
	backgroundSprites[0]->setPosition(glm::vec2(CAMERA_X, CAMERA_Y));
}

void Level1Sc2::update(int deltaTime)
{
	Scene::update(deltaTime);
}

void Level1Sc2::render()
{
	basicShader->use();
	basicShader->setUniformMatrix4f("projection", projection);
	backgroundSprites[0]->render();
	glm::mat4 modelview = glm::mat4(1.0f);
	basicShader->setUniformMatrix4f("modelview", modelview);
	map->render();
	platforms->render();
	stairs->render();
	for (auto candle : candles) candle->render();
	spriteShader->use();
	spriteShader->setUniformMatrix4f("projection", projection);
	for (auto axe : subweapons) axe->render();
	player->render();
	//text3.render("STAGE CLEAR", player->getPosition() + glm::vec2(60, 0));
	//text2.render("9", player->getPosition() + glm::vec2(60, 0));
	//text.render("La fe es lo que mueve a las personas\na unirse y seguir adelante.", player->getPosition()+glm::vec2(66,0));
	//text.render("Solo pueden ser gobernados por quienes\nson valiosos, respetados y apreciados.", player->getPosition()+glm::vec2(66,0));
	basicShader->use();
	for (auto item : items) item->render();
	gui->render();
}

TileMap* Level1Sc2::setTileMap()
{
	return TileMap::createTerrainMap("levels/level1sc2/terrain.txt", MAP_OFFSET, *basicShader);
}

TileMap* Level1Sc2::setPlatformMap()
{
	return TileMap::createTileMap("levels/level1sc2/platforms.txt", MAP_OFFSET, *basicShader);
}

TileMap* Level1Sc2::setStairsMap()
{
	return TileMap::createTileMap("levels/level1sc2/stairs.txt", MAP_OFFSET, *basicShader);
}

void Level1Sc2::initItems()
{
}

void Level1Sc2::initActors(Player* player)
{
	this->player = player;
	player->setTileMap(map);
	player->setPlatforms(platforms);
	player->setStairsMap(stairs);
	player->setPosition(glm::vec2(3 * map->getTileSize(), 20 * map->getTileSize()));
}

void Level1Sc2::updateCamera()
{
	glm::vec2 playerPos = player->getPosition();
	//cout << playerPos.y << endl;
	if (playerPos.y > (SCREEN_HEIGHT - CAMERA_Y * 4) && playerPos.y <= (SCREEN_HEIGHT * 2 - CAMERA_Y))
	{
		cameraPos.y = SCREEN_HEIGHT + CAMERA_Y;
		if (playerPos.x > SCREEN_WIDTH * 3) cameraPos.x = SCREEN_WIDTH * 3;
	}
	else if (playerPos.y > (SCREEN_HEIGHT * 2 - CAMERA_Y))
	{
		cameraPos.y = SCREEN_HEIGHT * 2 - CAMERA_Y;
	}
	else
	{
		cameraPos.y = CAMERA_Y;
		cameraPos.x = playerPos.x - SCREEN_WIDTH / 2;
		if (cameraPos.x < 0) cameraPos.x = 0;
		else if (cameraPos.x > (map->getMapSize().x * 16 - 16 - SCREEN_WIDTH)) cameraPos.x = (float)(map->getMapSize().x * 16 - 16 - SCREEN_WIDTH);
	}
	float minX = cameraPos.x + CAMERA_X;
	float minY = cameraPos.y;
	float maxX = cameraPos.x + SCREEN_WIDTH + CAMERA_X;
	float maxY = cameraPos.y + SCREEN_HEIGHT;
	projection = glm::ortho(minX, maxX, maxY, minY);
}

const pair<int, int> Level1Sc2::setNewLevelAndScene() const
{
	return pair<int, int>(-1,-1);
}
