#include "Level1Sc2.h"
#include "Game.h"
#include "TextureManager.h"

namespace
{
	const glm::vec4 fire1Colors[7] = {
		glm::vec4(182 / 255.f, 0.f, 0.f, 1.f),
		glm::vec4(218 / 255.f, 0.f, 0.f, 1.f),
		glm::vec4(255 / 255.f, 0.f, 0.f, 1.f),
		glm::vec4(255 / 255.f, 36 / 255.f, 0.f, 1.f),
		glm::vec4(255 / 255.f, 36 / 255.f, 0.f, 1.f),
		glm::vec4(255 / 255.f, 0.f, 0.f, 1.f),
		glm::vec4(218 / 255.f, 0.f, 0.f, 1.f),
	};
}

void Level1Sc2::init(Player& player, GUI& gui, ShaderProgram& spriteShader, ShaderProgram& basicShader)
{
	Scene::init(player, gui, spriteShader, basicShader);
	backgroundTexs.resize(6);
	backgroundTexs[0].loadFromFile("images/levels/lvl1/staticbg1.png", TEXTURE_PIXEL_FORMAT_RGBA);
	backgroundTexs[0].setMagFilter(GL_NEAREST);
	backgroundTexs[1].loadFromFile("images/levels/lvl1/staticbg2.png", TEXTURE_PIXEL_FORMAT_RGBA);
	backgroundTexs[1].setMagFilter(GL_NEAREST);
	backgroundTexs[2].loadFromFile("images/levels/lvl1/fire_bottom.png", TEXTURE_PIXEL_FORMAT_RGBA);
	backgroundTexs[2].setMagFilter(GL_NEAREST);
	backgroundTexs[3].loadFromFile("images/levels/lvl1/fire.png", TEXTURE_PIXEL_FORMAT_RGBA);
	backgroundTexs[3].setMagFilter(GL_NEAREST);
	backgroundTexs[4].loadFromFile("images/levels/lvl1/fire1.png", TEXTURE_PIXEL_FORMAT_RGBA);
	backgroundTexs[4].setMagFilter(GL_NEAREST);
	backgroundTexs[5].loadFromFile("images/levels/lvl1/fire2.png", TEXTURE_PIXEL_FORMAT_RGBA);
	backgroundTexs[5].setMagFilter(GL_NEAREST);
	bg1 = TexturedQuad::createTexturedQuad(glm::vec2(0.f), glm::vec2(1.f), backgroundTexs[0], basicShader);
	bg2 = TexturedQuad::createTexturedQuad(glm::vec2(0.f), glm::vec2(1.f), backgroundTexs[1], basicShader);
	bg3 = TexturedQuad::createTexturedQuad(glm::vec2(0.f), glm::vec2(64.f, 1.f), backgroundTexs[2], basicShader);
	bg4 = TexturedQuad::createTexturedQuad(glm::vec2(0.f), glm::vec2(1024.f, 448.f), *TextureManager::instance().getTexture("pixel"), basicShader);
	fireEffect = TexturedQuad::createTexturedQuad(glm::vec2(0.f, 0.5f), glm::vec2(32.f, 1.f), backgroundTexs[4], basicShader);
	backgroundSprites.reserve(3);
	backgroundSprites.emplace_back(Sprite::createSprite(glm::ivec2(768, 40), glm::vec2(48.f, .0625f), &backgroundTexs[3], &basicShader));
	backgroundSprites[0]->setNumberAnimations(1);
	backgroundSprites[0]->setAnimationSpeed(0, 16);
	backgroundSprites[0]->animatorY(0, 16, 0.f, 0.0625f, 0.f);
	backgroundSprites[0]->changeAnimation(0);
	backgroundSprites.emplace_back(Sprite::createSprite(glm::ivec2(1024, 32), glm::vec2(32.f, 0.5f), &backgroundTexs[4], &basicShader));
	backgroundSprites[1]->setNumberAnimations(1);
	backgroundSprites[1]->setAnimationSpeed(0, 2);
	backgroundSprites[1]->animatorY(0, 2, 0.f, 0.5f, 0.f);
	backgroundSprites[1]->changeAnimation(0);
	backgroundSprites.emplace_back(Sprite::createSprite(glm::ivec2(1024, 40), glm::vec2(32.f, 0.25f), &backgroundTexs[5], &basicShader));
	backgroundSprites[2]->setNumberAnimations(1);
	backgroundSprites[2]->setAnimationSpeed(0, 20);
	for (int i = 0; i < 8; i++)
		backgroundSprites[2]->animatorY(0, 2, 0.f, 0.25f, 0.f);
	for (int i = 0; i < 8; i++)
		backgroundSprites[2]->animatorY(0, 2, 0.5f, 0.25f, 0.f);
	backgroundSprites[2]->changeAnimation(0);
	bg1->setPosition(glm::vec2(CAMERA_X, CAMERA_Y));
	bg2->setPosition(glm::vec2(CAMERA_X + SCREEN_WIDTH * 1.5f, CAMERA_Y + SCREEN_HEIGHT));
	bg3->setPosition(glm::vec2(CAMERA_X + SCREEN_WIDTH * 4 + 64, CAMERA_Y + SCREEN_HEIGHT + 184+8));
	bg4->setPosition(glm::vec2(CAMERA_X, CAMERA_Y));
	backgroundSprites[0]->setPosition(glm::vec2(CAMERA_X + SCREEN_WIDTH * 2.f, CAMERA_Y + SCREEN_HEIGHT + 144+8));
	fireEffect->setColor(glm::vec4(109 / 255.f, 0.f, 0.f, 1.f));
	backgroundSprites[1]->setColor(fire1Colors[fireColorIndex]);

	projections.resize(3);

	//SoundEngine::instance().playStageSong(Game::STAGE1);
}

void Level1Sc2::update(int deltaTime)
{
	Scene::update(deltaTime);
	for (auto animbg : backgroundSprites) animbg->update(deltaTime);
	glm::vec4 bgColor = glm::vec4((72 + (37 * (int(timeElapsed * 100) % 2 == 0))) / 255.f, 0.f, 0.f, 1.f);
	bg4->setColor(bgColor);
	fireColorTimer += deltaTime;
	if (fireColorTimer >= 80)
	{
		fireColorTimer = 0;
		fireColorIndex++;
		if (fireColorIndex >= 7) fireColorIndex = 0;
		backgroundSprites[1]->setColor(fire1Colors[fireColorIndex]);
	}
}

void Level1Sc2::render()
{
	basicShader->use();
	basicShader->setUniformMatrix4f("projection", projections[0]);
	bg2->render();
	basicShader->setUniformMatrix4f("projection", projections[1]);
	backgroundSprites[0]->render();
	basicShader->setUniformMatrix4f("projection", projections[2]);
	bg4->render();
	for (int i = 0; i < 2; i++)
	{
		fireEffect->setPosition(glm::vec2(CAMERA_X, CAMERA_Y + 112 + i * 128));
		fireEffect->render();
		backgroundSprites[1]->setPosition(glm::vec2(CAMERA_X, CAMERA_Y + 112 + i * 128));
		backgroundSprites[2]->setPosition(glm::vec2(CAMERA_X, CAMERA_Y + 112 + i * 128));
		backgroundSprites[1]->render();
		backgroundSprites[2]->render();
	}
	bg1->render();
	bg3->render();
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
	player->setPosition(glm::vec2(3 * map->getTileSize(), 20 * map->getTileSize()));
}

void Level1Sc2::updateCamera()
{
	glm::vec2 playerPos = player->getPosition() + player->myCenter();
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
			else if (cameraPos.x > 768) cameraPos.x = 768.f;
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
