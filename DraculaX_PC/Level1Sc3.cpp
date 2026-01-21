#include "Level1Sc3.h"
#include "Game.h"

void Level1Sc3::init(Player& player, GUI& gui, ShaderProgram& spriteShader, ShaderProgram& basicShader)
{
	Scene::init(player, gui, spriteShader, basicShader);
	backgroundTexs.resize(4);
	backgroundTexs[0].loadFromFile("images/levels/lvl1/cielo.png", TEXTURE_PIXEL_FORMAT_RGBA);
	backgroundTexs[0].setMagFilter(GL_NEAREST);
	backgroundTexs[1].loadFromFile("images/levels/lvl1/castlevania.png", TEXTURE_PIXEL_FORMAT_RGBA);
	backgroundTexs[1].setMagFilter(GL_NEAREST);
	backgroundTexs[2].loadFromFile("images/levels/lvl1/muralla2.png", TEXTURE_PIXEL_FORMAT_RGBA);
	backgroundTexs[2].setMagFilter(GL_NEAREST);
	backgroundTexs[3].loadFromFile("images/levels/lvl1/muralla.png", TEXTURE_PIXEL_FORMAT_RGBA);
	backgroundTexs[3].setMagFilter(GL_NEAREST);
	cielo = TexturedQuad::createTexturedQuad(glm::vec2(0.f, 0.f), glm::vec2(8.f, 1.f), backgroundTexs[0], basicShader);
	castlevania = TexturedQuad::createTexturedQuad(glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f), backgroundTexs[1], basicShader);
	muralla2 = TexturedQuad::createTexturedQuad(glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f), backgroundTexs[2], basicShader);
	muralla = TexturedQuad::createTexturedQuad(glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f), backgroundTexs[3], basicShader);
	cielo->setPosition(glm::vec2(CAMERA_X, CAMERA_Y));
	castlevania->setPosition(glm::vec2(CAMERA_X, CAMERA_Y+16));
	muralla2->setPosition(glm::vec2(CAMERA_X, CAMERA_Y+64));
	muralla->setPosition(glm::vec2(CAMERA_X, CAMERA_Y+128));

	projections.resize(4);

	SoundEngine::instance().playNonStageSong(SoundEngine::FORMER_ROOM, true);
	bossAppearanceTimer = 2.f;
	orbSpawnTimer = 2.f;
}

void Level1Sc3::update(int deltaTime)
{
	if (!bossDefeated && bossAppeares && bossAppearanceTimer > 0.f)
	{
		bossAppearanceTimer -= deltaTime / 1000.f;
	}
	else if (!bossDefeated && bossAppearanceTimer <= 0.f && wyvern == nullptr)
	{
		wyvern = new Wyvern();
		wyvern->init(MAP_OFFSET, *spriteShader, glm::vec2(42 * 16, -48));
		SoundEngine::instance().playSFX(SoundEngine::WYVERN_APPEAR);
	}
	else if (wyvern != nullptr)
	{
		wyvern->update(deltaTime);
		Hitbox wyvBox = wyvern->getHitbox(); 
		if (!wyvern->isWounded())
		{
			if (player->isAttacking() && collision(wyvBox, player->getWhipHitbox())) wyvern->takeDmg(7);
			else
			{
				for (auto subwpn : subweapons)
				{
					if (collision(wyvBox, subwpn->getHitbox())) wyvern->takeDmg(subwpn->getDamage());
				}
			}
		}
		else if (!player->wounded() && collision(wyvBox, player->getHitbox())) gui->takeDmg(15);
		else if (!bossDefeated && wyvern->isEnded())
		{
			EffectsManager::instance().createExplosions(wyvern->getPosition(), glm::vec2(64.f, 64.f), 200, 32, 64, glm::vec4(1.f));
			SoundEngine::instance().playSFX(SoundEngine::EXPLOSION_HUGE_LONG);
			bossDefeated = true;
		}
		else if (wyvern->isRemoved())
		{
			delete wyvern;
			wyvern = nullptr;
		}
	}
	else if (bossDefeated)
	{
		orbSpawnTimer -= deltaTime / 1000.f;
		if (!orbSpawned && orbSpawnTimer <= 0.f)
		{
			items.push_back(ItemManager::instance().getOrb(glm::vec2(41 * 16 - 7, 32 + 8)));
			orbSpawned = true;
			SoundEngine::instance().playLoopedSFX(SoundEngine::ORB);
			SoundEngine::instance().fadeOutMusic(3000);
		}
		else if (!stageCleared && gui->stageCleared())
		{
			stageCleared = true;
			stageClearText.init(*basicShader, "images/fonts/BigLetters.png", glm::ivec2(16, 16), 26);
			stageClearText.setColor(glm::vec4(108 / 255.f, 252 / 255.f, 0.f, 1.f));
			SoundEngine::instance().stopLoopedSFX(SoundEngine::ORB);
			SoundEngine::instance().playNonStageSong(SoundEngine::STAGE_CLEAR, false);
		}
	}
	Scene::update(deltaTime);	
}

void Level1Sc3::render()
{
	basicShader->use();
	basicShader->setUniformMatrix4f("projection", projections[0]);
	cielo->render();
	castlevania->render();
	basicShader->setUniformMatrix4f("projection", projections[1]);
	muralla2->render();
	basicShader->setUniformMatrix4f("projection", projections[2]);
	muralla->render();
	glm::mat4 modelview = glm::mat4(1.0f);
	basicShader->setUniformMatrix4f("modelview", modelview);
	basicShader->setUniformMatrix4f("projection", projections[3]);
	map->render();
	for (auto candle : candles) candle->render();
	for (auto item : items) item->render();
	EffectsManager::instance().render();
	if (stageCleared) stageClearText.render(stageClearStr[Game::instance().getCurrentTxtLang()], glm::vec2(43 * 16, 72));
	spriteShader->use();
	spriteShader->setUniformMatrix4f("projection", projections[3]);
	for (auto axe : subweapons) axe->render();
	if (wyvern != nullptr) wyvern->render();
	player->render();
	gui->render();
	renderTransition();
}

TileMap* Level1Sc3::setTileMap()
{
	return TileMap::createTerrainMap("levels/level1sc3/terrain.txt", MAP_OFFSET, *basicShader);
}

TileMap* Level1Sc3::setPlatformMap()
{
	return nullptr;
}

TileMap* Level1Sc3::setStairsMap()
{
	return nullptr;
}

void Level1Sc3::initItems()
{
	Candle* candle1 = Candle::createPilarCandle(*basicShader, glm::vec2(7 * map->getTileSize(), 11 * map->getTileSize()), ItemManager::HEART);
	candles.push_back(candle1);
	candle1 = Candle::createPilarCandle(*basicShader, glm::vec2(12 * map->getTileSize(), 11 * map->getTileSize()), ItemManager::FOUR_HUNDRED);
	candles.push_back(candle1);
	candle1 = Candle::createPilarCandle(*basicShader, glm::vec2(15 * map->getTileSize(), 11 * map->getTileSize()), ItemManager::HEART);
	candles.push_back(candle1);
	candle1 = Candle::createPilarCandle(*basicShader, glm::vec2(24 * map->getTileSize(), 11 * map->getTileSize()), ItemManager::HEART);
	candles.push_back(candle1);
}

void Level1Sc3::initActors(Player* player)
{
	this->player = player;
	player->setTileMap(map);
	player->setPlatforms(platforms);
	player->setStairsMap(stairs);
	player->setPosition(glm::vec2(0 * map->getTileSize(), 9 * map->getTileSize()));

	
}

void Level1Sc3::updateCamera()
{
	if (!bossAppeares && !bossDefeated)
	{
		glm::vec2 playerPos = player->getPosition();
		glm::vec2 playerCenter = player->myCenter();
		const float multipliers[4] = { 0.125f, 0.25f, 0.5f, 1.f };
		for (int i = 0; i < 4; i++)
		{
			cameraPos.x = (playerPos + playerCenter).x - SCREEN_WIDTH / 2.f;
			if (cameraPos.x < 0) cameraPos.x = 0;
			else if (cameraPos.x > 768 - SCREEN_WIDTH)
			{
				cameraPos.x = 768 - SCREEN_WIDTH;
				bossAppeares = true;
				SoundEngine::instance().fadeOutMusic(5000);
			}
			cameraPos.x *= multipliers[i];
			float minX = cameraPos.x + CAMERA_X;
			float minY = CAMERA_Y;
			float maxX = cameraPos.x + SCREEN_WIDTH + CAMERA_X;
			float maxY = SCREEN_HEIGHT + CAMERA_Y;
			projections[i] = glm::ortho(minX, maxX, maxY, minY);
		}
	}
}

const pair<int, int> Level1Sc3::setNewLevelAndScene() const
{
	return pair<int, int>(-1, -1);
}
