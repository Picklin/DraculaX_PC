#include "Level1Sc1.h"
#include "Game.h"

void Level1Sc1::init(Player& player, GUI& gui, ShaderProgram& spriteShader, ShaderProgram& basicShader) 
{
	Scene::init(player, gui, spriteShader, basicShader);
	blackScreenDuration = 1.f;
	const int tileSize = 8;
	backgroundTexs.resize(4);
	backgroundSprites.reserve(3);
	backgroundTexs[0].loadFromFile("images/levels/lvl1/animbg1.png", TEXTURE_PIXEL_FORMAT_RGBA);
	backgroundTexs[1].loadFromFile("images/levels/lvl1/animbg2.png", TEXTURE_PIXEL_FORMAT_RGBA);
	backgroundTexs[2].loadFromFile("images/levels/lvl1/animbg3.png", TEXTURE_PIXEL_FORMAT_RGBA);
	backgroundTexs[3].loadFromFile("images/levels/lvl1/trunk.png", TEXTURE_PIXEL_FORMAT_RGBA);
	backgroundTexs[0].setMagFilter(GL_NEAREST);
	backgroundTexs[1].setMagFilter(GL_NEAREST);
	backgroundTexs[2].setMagFilter(GL_NEAREST);
	backgroundTexs[3].setMagFilter(GL_NEAREST);
	backgroundSprites.emplace_back(Sprite::createSprite(glm::ivec2(1024, 64), glm::vec2(32.f, 0.25f), &backgroundTexs[0], &basicShader));
	backgroundSprites.emplace_back(Sprite::createSprite(glm::ivec2(1024, 40), glm::vec2(64.f, 0.125f), &backgroundTexs[1], &basicShader));
	backgroundSprites.emplace_back(Sprite::createSprite(glm::ivec2(1024, 8), glm::vec2(64.f, 0.125f), &backgroundTexs[2], &basicShader));
	trunk = TexturedQuad::createTexturedQuad(glm::vec2(0.f), glm::vec2(1.f, 1.f), backgroundTexs[3], basicShader);
	trunk->setPosition(glm::vec2(SCREEN_X + 94 * tileSize, SCREEN_Y + 10 * tileSize));
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
	text2.init(basicShader, "images/fonts/Letters&Nums.png", glm::ivec2(8, 8), 40);
	text2.setColor(glm::vec4(144 / 255.f, 144 / 255.f, 252 / 255.f, 1.f));
	text3.init(basicShader, "images/fonts/BigLetters.png", glm::ivec2(16, 16), 26);
	text3.setColor(glm::vec4(108 / 255.f, 252 / 255.f, 0.f, 1.f));

	SoundEngine::instance().playStageSong(Game::STAGE1);
	//SoundEngine::instance().playNonStageSong(SoundEngine::FORMER_ROOM, false);
	//SoundEngine::instance().playOverture();
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
	for (auto candle : candles) candle->render();
	spriteShader->use();
	spriteShader->setUniformMatrix4f("projection", projections[2]);
	for (auto axe : subweapons) axe->render();
	player->render();
	//text3.render("STAGE CLEAR", player->getPosition() + glm::vec2(60, 0));
	//text2.render("9", player->getPosition() + glm::vec2(60, 0));
	//text.render("La fe es lo que mueve a las personas\na unirse y seguir adelante.", player->getPosition()+glm::vec2(66,0));
	//text.render("Solo pueden ser gobernados por quienes\nson valiosos, respetados y apreciados.", player->getPosition()+glm::vec2(66,0));
	basicShader->use();
	trunk->render();
	for (auto item : items) item->render();
	EffectsManager::instance().render();
	gui->render();
	renderTransition();
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
	//items.push_back(ItemManager::instance().getTrinket(glm::vec2(48 * map->getTileSize(), 6 * map->getTileSize()), GUI::trinketIDs::KEY));
	//items.push_back(ItemManager::instance().getFood(glm::vec2(52 * map->getTileSize(), 6 * map->getTileSize()), GUI::foodIds::BIRTHDAY_CAKE, *gui));
	//items.push_back(ItemManager::instance().getHeart(glm::vec2(20 * map->getTileSize(), 0 * map->getTileSize())));
	//items.push_back(ItemManager::instance().getHeart(glm::vec2(20 * map->getTileSize(), 0 * map->getTileSize())));
	//items.push_back(ItemManager::instance().getSmallHeart(glm::vec2(16 * map->getTileSize(), 0 * map->getTileSize())));
	//items.push_back(ItemManager::instance().getBigHeart(glm::vec2(12 * map->getTileSize(), 0 * map->getTileSize())));
	Candle* candle1 = Candle::createTorchCandle(*basicShader, glm::vec2(20 * map->getTileSize(), 15 * map->getTileSize()), ItemManager::HEART_SMALL);
	candles.push_back(candle1);
	candle1 = Candle::createTorchCandle(*basicShader, glm::vec2(48 * map->getTileSize(), 6 * map->getTileSize()), ItemManager::ONE_HUNDRED);
	candles.push_back(candle1);
	candle1 = Candle::createTorchCandle(*basicShader, glm::vec2(44 * map->getTileSize(), 17 * map->getTileSize()), ItemManager::HEART_SMALL);
	candles.push_back(candle1);
	candle1 = Candle::createTorchCandle(*basicShader, glm::vec2(52 * map->getTileSize(), 17 * map->getTileSize()), ItemManager::HEART);
	candles.push_back(candle1);
	candle1 = Candle::createTorchCandle(*basicShader, glm::vec2(76 * map->getTileSize(), 17 * map->getTileSize()), GUI::CAT);
	candles.push_back(candle1);
	candle1 = Candle::createTorchCandle(*basicShader, glm::vec2(85 * map->getTileSize(), 17 * map->getTileSize()), ItemManager::HEART_SMALL);
	candles.push_back(candle1);
}

void Level1Sc1::initActors(Player* player) 
{
	this->player = player;
	player->setTileMap(map);
	player->setPlatforms(platforms);
	player->setStairsMap(stairs);
	int tileSize = map->getTileSize();
	player->setPosition(glm::vec2(4 * tileSize, 17 * tileSize));

	triggerAreas.push_back(new TriggerArea(glm::vec2(131 * tileSize, 0), glm::vec2(135 * tileSize, 30 * tileSize), Scene::NEXT_SCENE));
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
	//cout << playerPos.x + playerCenter.x << endl;
}

void Level1Sc1::doAction(int eventId)
{
	if (eventId == Scene::AUTO_ADVANCE)
	{

	}
	else fadeIn = eventId == Scene::NEXT_SCENE;
}

const pair<int, int> Level1Sc1::setNewLevelAndScene() const 
{
	return pair<int, int>(0, 1);
}