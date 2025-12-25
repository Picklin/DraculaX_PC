#include "Level1Sc1.h"
#include "Game.h"

void Level1Sc1::init(Player& player, GUI& gui, ShaderProgram& spriteShader, ShaderProgram& basicShader) 
{
	Scene::init(player, gui, spriteShader, basicShader);
	const int tileSize = 8;
	backgroundTexs.resize(3);
	backgroundSprites.reserve(3);
	backgroundTexs[0].loadFromFile("images/levels/lvl1/animbg1.png", TEXTURE_PIXEL_FORMAT_RGBA);
	backgroundTexs[1].loadFromFile("images/levels/lvl1/animbg2.png", TEXTURE_PIXEL_FORMAT_RGBA);
	backgroundTexs[2].loadFromFile("images/levels/lvl1/animbg3.png", TEXTURE_PIXEL_FORMAT_RGBA);
	backgroundTexs[0].setMagFilter(GL_NEAREST);
	backgroundTexs[1].setMagFilter(GL_NEAREST);
	backgroundTexs[2].setMagFilter(GL_NEAREST);
	backgroundSprites.emplace_back(Sprite::createSprite(glm::ivec2(1024, 64), glm::vec2(32.f, 0.25f), &backgroundTexs[0], &basicShader));
	backgroundSprites.emplace_back(Sprite::createSprite(glm::ivec2(1024, 40), glm::vec2(64.f, 0.125f), &backgroundTexs[1], &basicShader));
	backgroundSprites.emplace_back(Sprite::createSprite(glm::ivec2(1024, 8), glm::vec2(64.f, 0.125f), &backgroundTexs[2], &basicShader));
	backgroundSprites[0]->setPosition(MAP_OFFSET);
	backgroundSprites[1]->setPosition(glm::vec2(SCREEN_X, SCREEN_Y + 18 * tileSize));
	backgroundSprites[2]->setPosition(glm::vec2(SCREEN_X, SCREEN_Y + 23 * tileSize));
	backgroundSprites[0]->setNumberAnimations(1);
	backgroundSprites[0]->setAnimationSpeed(0, 10);
	backgroundSprites[0]->animatorY(0, 4, 0.f, 0.25f, 0.f);
	backgroundSprites[0]->addKeyframe(0, glm::vec2(0.f, 0.75f));
	backgroundSprites[0]->changeAnimation(0);
	backgroundSprites[1]->setNumberAnimations(1);
	backgroundSprites[1]->setAnimationSpeed(0, 20);
	backgroundSprites[1]->animatorY(0, 8, 0.f, 0.125f, 0.f);
	backgroundSprites[1]->animatorY(0, 7, 0.75f, -0.125f, 0.f);
	backgroundSprites[1]->changeAnimation(0);
	backgroundSprites[2]->setNumberAnimations(1);
	backgroundSprites[2]->setAnimationSpeed(0, 20);
	backgroundSprites[2]->animatorY(0, 8, 0.f, 0.125f, 0.f);
	backgroundSprites[2]->animatorY(0, 7, 0.75f, -0.125f, 0.f);
	backgroundSprites[2]->changeAnimation(0);

	layers.reserve(3);
	layers.emplace_back(TileMap::createTileMap("levels/level1sc1/casitas.txt", MAP_OFFSET, basicShader));
	layers.emplace_back(TileMap::createTileMap("levels/level1sc1/wall.txt", MAP_OFFSET, basicShader));
	layers.emplace_back(TileMap::createTileMap("levels/level1sc1/casas.txt", MAP_OFFSET, basicShader));

	projections.resize(3);

	text.init(basicShader, "images/fonts/font_intro.png", glm::ivec2(6, 12), 64);
	text2.init(basicShader, "images/fonts/Letters&Nums.png", glm::ivec2(8, 8), 39);
	text3.init(basicShader, "images/fonts/BigLetters.png", glm::ivec2(16, 16), 26);
}
void Level1Sc1::update(int deltaTime) 
{
	Scene::update(deltaTime);
	for (auto animatedLayer : backgroundSprites) animatedLayer->update(deltaTime);
}
void Level1Sc1::render() 
{
	basicShader->use();
	basicShader->setUniformMatrix4f("projection", projections[0]);
	backgroundSprites[0]->render();	//fuego del fondo
	glm::mat4 modelview = glm::mat4(1.0f);
	basicShader->setUniformMatrix4f("modelview", modelview);
	layers[0]->render();			//casas del fondo
	basicShader->setUniformMatrix4f("projection", projections[1]);
	layers[1]->render();			//muro de "barro"
	backgroundSprites[1]->render();
	basicShader->setUniformMatrix4f("projection", projections[2]);
	backgroundSprites[2]->render();	
	modelview = glm::mat4(1.0f);
	basicShader->setUniformMatrix4f("modelview", modelview);
	layers[2]->render();			//casas
	map->render();
	platforms->render();
	stairs->render();
	spriteShader->use();
	spriteShader->setUniformMatrix4f("projection", projections[2]);
	player->render();
	//text3.render("STAGE CLEAR", player->getPosition() + glm::vec2(60, 0));
	//text2.render("9", player->getPosition() + glm::vec2(60, 0));
	//text.render("In der guten alten Zeit,\nlebten die Menschen\nnoch ruhig und in Frieden...", player->getPosition()+glm::vec2(60,0));
	basicShader->use();
	for (auto item : items) item->render();
	gui->render();
}

TileMap* Level1Sc1::setTileMap() 
{
	return TileMap::createTerrainMap("levels/level1sc1/terrain.txt", MAP_OFFSET, *basicShader);
}
TileMap* Level1Sc1::setPlatformMap() 
{
	return TileMap::createTileMap("levels/level1sc1/platforms.txt", MAP_OFFSET, *basicShader);
}

TileMap* Level1Sc1::setStairsMap()
{
	return TileMap::createTileMap("levels/level1sc1/stairs.txt", MAP_OFFSET, *basicShader);
}

void Level1Sc1::initItems() 
{
	items.push_back(ItemManager::instance().getHolyWater(glm::vec2(48 * map->getTileSize(), 6 * map->getTileSize())));
	items.push_back(ItemManager::instance().getWatch(glm::vec2(52 * map->getTileSize(), 6 * map->getTileSize())));
	items.push_back(ItemManager::instance().getBible(glm::vec2(56 * map->getTileSize(), 6 * map->getTileSize())));
	items.push_back(ItemManager::instance().getCross(glm::vec2(60 * map->getTileSize(), 6 * map->getTileSize())));
	items.push_back(ItemManager::instance().getCat(glm::vec2(64 * map->getTileSize(), 6 * map->getTileSize())));
	items.push_back(ItemManager::instance().getTurtle(glm::vec2(68 * map->getTileSize(), 6 * map->getTileSize())));
	items.push_back(ItemManager::instance().getEgg(glm::vec2(72 * map->getTileSize(), 6 * map->getTileSize())));
	items.push_back(ItemManager::instance().getBook(glm::vec2(76 * map->getTileSize(), 6 * map->getTileSize())));
	items.push_back(ItemManager::instance().getDragon(glm::vec2(80 * map->getTileSize(), 6 * map->getTileSize())));
}
void Level1Sc1::initActors(Player* player) 
{
	this->player = player;
	player->setTileMap(map);
	player->setPlatforms(platforms);
	player->setStairsMap(stairs);
	player->setPosition(glm::vec2(38 * map->getTileSize(), 6 * map->getTileSize()));
}
void Level1Sc1::updateCamera() 
{
	glm::vec2 playerPos = player->getPosition();
	glm::vec2 playerCenter = player->myCenter();
	const float multipliers[3] = { 0.37f, 0.5f, 1.f };
	for (int i = 0; i < 3; i++)
	{
		cameraPos.x = (playerPos + playerCenter).x - SCREEN_WIDTH / 2.f;
		if (cameraPos.x < 0) cameraPos.x = 0;
		else if (cameraPos.x > 1024 - SCREEN_WIDTH) cameraPos.x = 1024 - SCREEN_WIDTH;
		cameraPos.x *= multipliers[i];
		float minX = cameraPos.x + SCREEN_X;
		float minY = SCREEN_Y;
		float maxX = cameraPos.x + SCREEN_WIDTH + SCREEN_X;
		float maxY = SCREEN_HEIGHT + SCREEN_Y;
		projections[i] = glm::ortho(minX, maxX, maxY, minY);
	}
}
const pair<int, int> Level1Sc1::setNewLevelAndScene() const 
{
	return pair<int, int>(-1, -1);
}