#include "Level1Sc2.h"
#include "Game.h"

void Level1Sc2::init(Player& player, GUI& gui, ShaderProgram& spriteShader, ShaderProgram& basicShader)
{
	Scene::init(player, gui, spriteShader, basicShader);
	backgroundTexs.resize(4);
	backgroundTexs[0].loadFromFile("images/levels/lvl1/staticbg1.png", TEXTURE_PIXEL_FORMAT_RGBA);
	backgroundTexs[0].setMagFilter(GL_NEAREST);
	backgroundTexs[1].loadFromFile("images/levels/lvl1/staticbg2.png", TEXTURE_PIXEL_FORMAT_RGBA);
	backgroundTexs[1].setMagFilter(GL_NEAREST);
	backgroundTexs[2].loadFromFile("images/levels/lvl1/fire_bottom.png", TEXTURE_PIXEL_FORMAT_RGBA);
	backgroundTexs[2].setMagFilter(GL_NEAREST);
	backgroundTexs[3].loadFromFile("images/levels/lvl1/fire.png", TEXTURE_PIXEL_FORMAT_RGBA);
	backgroundTexs[3].setMagFilter(GL_NEAREST);
	bg1 = TexturedQuad::createTexturedQuad(glm::vec2(0.f), glm::vec2(1.f), backgroundTexs[0], basicShader);
	bg2 = TexturedQuad::createTexturedQuad(glm::vec2(0.f), glm::vec2(1.f), backgroundTexs[1], basicShader);
	bg3 = TexturedQuad::createTexturedQuad(glm::vec2(0.f), glm::vec2(40.f, 1.f), backgroundTexs[2], basicShader);
	backgroundSprites.reserve(1);
	backgroundSprites.emplace_back(Sprite::createSprite(glm::ivec2(640, 40), glm::vec2(40.f, .0625f), &backgroundTexs[3], &basicShader));
	backgroundSprites[0]->setNumberAnimations(1);
	backgroundSprites[0]->setAnimationSpeed(0, 16);
	backgroundSprites[0]->animatorY(0, 16, 0.f, 0.0625f, 0.f);
	backgroundSprites[0]->changeAnimation(0);
	bg1->setPosition(glm::vec2(CAMERA_X, CAMERA_Y+8));
	bg2->setPosition(glm::vec2(CAMERA_X + SCREEN_WIDTH * 1.5f, CAMERA_Y + SCREEN_HEIGHT+8));
	bg3->setPosition(glm::vec2(CAMERA_X + SCREEN_WIDTH * 1.5f, CAMERA_Y + SCREEN_HEIGHT + 184+8));
	backgroundSprites[0]->setPosition(glm::vec2(CAMERA_X + SCREEN_WIDTH * 1.5f, CAMERA_Y + SCREEN_HEIGHT + 144+8));
	projections.resize(3);
}

void Level1Sc2::update(int deltaTime)
{
	Scene::update(deltaTime);
	backgroundSprites[0]->update(deltaTime);
}

void Level1Sc2::render()
{
	basicShader->use();
	basicShader->setUniformMatrix4f("projection", projections[0]);
	bg2->render();
	bg3->render();
	backgroundSprites[0]->render();
	basicShader->setUniformMatrix4f("projection", projections[2]);
	bg1->render();
	glm::mat4 modelview = glm::mat4(1.0f);
	basicShader->setUniformMatrix4f("modelview", modelview);
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
	player->setPosition(glm::vec2(66 * map->getTileSize(), 20 * map->getTileSize()));
}

void Level1Sc2::updateCamera()
{
	glm::vec2 playerPos = player->getPosition();
	//cout << playerPos.y << endl;
	const float multipliers[3] = { 0.375f, 0.5f, 1.f };
	for (int i = 0; i < 3; i++)
	{
		if (playerPos.y > (SCREEN_HEIGHT - CAMERA_Y * 4) && playerPos.y <= (SCREEN_HEIGHT * 2 - CAMERA_Y))
		{
			cameraPos.y = SCREEN_HEIGHT + CAMERA_Y;
			if (playerPos.x > SCREEN_WIDTH * 3 && playerPos.x < SCREEN_WIDTH * 4) cameraPos.x = SCREEN_WIDTH * 3;
			else if (playerPos.x > SCREEN_WIDTH * 4)
			{
				cameraPos.x = playerPos.x - SCREEN_WIDTH / 2;
				if (cameraPos.x < SCREEN_WIDTH * 4) cameraPos.x = SCREEN_WIDTH * 4;
				else if (cameraPos.x > (map->getMapSize().x * 16 - 16 - SCREEN_WIDTH)) cameraPos.x = (float)(map->getMapSize().x * 16 - 16 - SCREEN_WIDTH);
			}
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
		cameraPos.x *= multipliers[i];
		float minX = cameraPos.x + CAMERA_X;
		float minY = cameraPos.y;
		float maxX = cameraPos.x + SCREEN_WIDTH + CAMERA_X;
		float maxY = cameraPos.y + SCREEN_HEIGHT;
		projections[i] = glm::ortho(minX, maxX, maxY, minY);
	}
}

const pair<int, int> Level1Sc2::setNewLevelAndScene() const
{
	return pair<int, int>(-1,-1);
}
