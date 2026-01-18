#include "Scene.h"
#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Game.h"
#include "TextureManager.h"

Scene::~Scene()
{
	if (map != NULL)
	{
		map->free();
		delete map;
	}

	if (platforms != NULL)
	{
		platforms->free();
		delete platforms;
	}

	for (auto enemy : enemies)
		delete enemy;
	enemies.clear();

	for (auto item : items)
		delete item;
	items.clear();

	EffectsManager::instance().free();

	for (auto sp : spawnPoints)
		delete sp;
	spawnPoints.clear();

	for (auto& texLayer : backgroundTexs)
	{
		texLayer.free();
	}
	for (auto backgroundSprite : backgroundSprites)
	{
		backgroundSprite->free();
		delete backgroundSprite;
	}
	for (auto layer : layers)
	{
		layer->free();
		delete layer;
	}
}


void Scene::init(Player& player, GUI& gui, ShaderProgram& spriteShader, ShaderProgram& basicShader)
{
	this->spriteShader = &spriteShader;
	this->basicShader = &basicShader;
	this->gui = &gui;
	map = setTileMap();
	platforms = setPlatformMap();
	stairs = setStairsMap();
	initManagers();
	initActors(&player);
	initItems();
	projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);
	screenWidth = SCREEN_WIDTH;
	blackScreen = TexturedQuad::createTexturedQuad(glm::vec2(0.f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT), *TextureManager::instance().getTexture("pixel"), basicShader);
	blackScreen->setColor(glm::vec3(0.f));
	fadeDuration = 0.5f;
	blackScreenDuration = 0.25f;
}

void Scene::update(int deltaTime)
{
	timeElapsed += deltaTime / 1000.f;
	updateActors(deltaTime);
	updateSpawnPoints(deltaTime);
	updateTriggerAreas();
	updateEffects(deltaTime);
	updateCamera();
}

void Scene::render()
{
	basicShader->use();
	basicShader->setUniformMatrix4f("projection", projection);
	glm::mat4 modelview = glm::mat4(1.0f);
	basicShader->setUniformMatrix4f("modelview", modelview);
	map->render();
	spriteShader->use();
	spriteShader->setUniformMatrix4f("projection", projection);
	player->render();
	basicShader->use();
	glm::mat4 guiProjection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f); // Proyección fija para la interfaz
	basicShader->setUniformMatrix4f("projection", guiProjection);
	gui->render();
}

void Scene::setScreenWidth(int width)
{
	screenWidth = width;
}

void Scene::setViewportOffset(int offset)
{
	viewportOffset = offset;
}

void Scene::updateActors(int deltaTime)
{
	if (!player->isEnded())
	{
		if (player->usingSubweapon())
		{
			Axe* axe = new Axe();
			int dir = player->getLookingDirection();
			axe->init(MAP_OFFSET, *spriteShader, dir);
			glm::vec2 pos = player->getPosition() + player->myCenter();
			axe->setPosition(glm::vec2(pos.x - 32 * (dir == -1), pos.y));
			subweapons.push_back(axe);
		}
		//cout << axes.size() << endl;
		//cout << items.size() << endl;
		for (unsigned int i = 0; i < items.size(); i++)
		{
			if (!items[i]->isEnded() && items[i]->isGrabable() && collision(items[i]->getHitbox(), player->getHitbox()))
			{
				Trinket* trinketItem = dynamic_cast<Trinket*>(items[i]);
				if (trinketItem && gui->compatibleTrinket(trinketItem->getTrinketID()))
				{
					int currentTrinket = gui->getCurrentTrinketID();
					if (currentTrinket != GUI::trinketIDs::NONE)
					{
						Item* trinket = ItemManager::instance().getTrinket(player->getPosition() + player->myCenter() - glm::vec2(8.f), currentTrinket);
						trinket->eject(-1 * player->getLookingDirection());
						trinket->setUngrabable();
						items.push_back(trinket);
					}
					trinketItem->grab(*gui);
				}
				else if (!trinketItem)
				{
					items[i]->grab(*gui);
				}
			}
			items[i]->update(deltaTime);
			if (items[i]->getsRemoved())
			{
				delete items[i];
				items.erase(items.begin() + i);
			}
		}
		for (unsigned int i = 0; i < candles.size(); i++)
		{
			if (player->isAttacking() && !candles[i]->isDestroyed() && collision(candles[i]->getHitbox(), player->getWhipHitbox()))
			{
				destroyCandle(*candles[i]);
			}
			candles[i]->update(deltaTime);
			if (candles[i]->getsRemoved())
			{
				delete candles[i];
				candles.erase(candles.begin() + i);
			}
		}
		for (unsigned int i = 0; i < subweapons.size(); i++)
		{
			subweapons[i]->update(deltaTime);
			for (auto candle : candles)
			{
				//Hitbox candleHb = candle->getHitbox();
				//cout << "Candle Hitbox: Min(" << candleHb.min.x << ", " << candleHb.min.y << ") Max(" << candleHb.max.x << ", " << candleHb.max.y << ")\n";
				if (!candle->isDestroyed() && collision(candle->getHitbox(), subweapons[i]->getHitbox()))
				{
					destroyCandle(*candle);
				}
			}
			if (subweapons[i]->getsRemoved())
			{
				delete subweapons[i];
				subweapons.erase(subweapons.begin() + i);
			}
		}
		
	}
	player->update(deltaTime);

}

void Scene::updateCamera()
{
	glm::vec2 playerPos = player->getPosition();
	cameraPos = playerPos - glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT) / 2.f;
	float minX = cameraPos.x + 50.f;
	float minY = cameraPos.y;
	float maxX = cameraPos.x + SCREEN_WIDTH + 50.f;
	float maxY = cameraPos.y + SCREEN_HEIGHT;
	projection = glm::ortho(minX, maxX, maxY, minY);
}

void Scene::renderTransition()
{
	//basicShader->use();
	if (fadeOut)
	{
		float alpha = 1.f - (timeElapsed / fadeDuration);
		blackScreen->setAlpha(alpha);
		blackScreen->render();
	}
	else if (fadeIn)
	{
		float alpha = fadeTime / fadeDuration;
		blackScreen->setAlpha(alpha);
		blackScreen->render();
	}
}

bool Scene::collision(const Hitbox& hitbox1, const Hitbox& hitbox2)
{
	return ((hitbox1.min.x < hitbox2.max.x) && (hitbox2.min.x < hitbox1.max.x) && (hitbox1.min.y < hitbox2.max.y) && (hitbox2.min.y < hitbox1.max.y));	
}

bool Scene::oobEnemy(const glm::vec2& pos) const
{
	glm::ivec2 mapSize = map->getMapSize() * map->getTileSize();
	return pos.x < -64 || pos.y < -64 || pos.x > mapSize.x || pos.y > mapSize.y;
}

void Scene::updateSpawnPoints(int deltaTime)
{
	glm::vec2 playerPos(player->getPosition() + player->myCenter());
	for (auto& sp : infSpawnPoints)
	{
		if (sp.spawnTime <= 0 && isInArea(sp.area, playerPos))
		{
			enemies.push_back(EnemyManager::instance().getEnemy(sp.spawnPos, sp.enemyId));
			sp.spawnTime = sp.millisecsBetweenSpawns;
		}
		sp.spawnTime -= deltaTime;
	}
	for (unsigned int i = 0; i < spawnPoints.size(); i++)
	{
		if (isInArea(spawnPoints[i]->area, playerPos))
		{
			enemies.push_back(EnemyManager::instance().getEnemy(spawnPoints[i]->spawnPos, spawnPoints[i]->enemyId));
			delete spawnPoints[i];
			spawnPoints.erase(spawnPoints.begin() + i);
		}
	}

}

void Scene::updateTriggerAreas()
{
	glm::vec2 playerPos(player->getPosition() + player->myCenter());
	for (unsigned int i = 0; i < triggerAreas.size(); i++)
	{
		if (isInArea(triggerAreas[i]->area, playerPos))
		{
			doAction(triggerAreas[i]->eventId);
			delete triggerAreas[i];
			triggerAreas.erase(triggerAreas.begin() + i);
		}
	}
}

bool Scene::isInArea(const Hitbox& area, const glm::vec2& pos)
{
	return pos.x > area.min.x && pos.x < area.max.x && pos.y > area.min.y && pos.y < area.max.y;
}

void Scene::updateEffects(int deltaTime)
{
	EffectsManager::instance().update(deltaTime);
	fadeOut = timeElapsed <= fadeDuration;
	if (fadeIn)
	{
		fadeTime += deltaTime / 1000.f;
		if (fadeTime >= fadeDuration + blackScreenDuration)
		{
			if (player->isEnded())
			{
				Game::instance().restartScene();
			}
			else
			{
				auto& lvlnsce = setNewLevelAndScene();
				Game::instance().changeScene(lvlnsce.first, lvlnsce.second);
			}
		}
	}
}

void Scene::destroyCandle(Candle& candle)
{
	int dropId = candle.getDropId();
	if (dropId >= GUI::BIRD) items.push_back(ItemManager::instance().getTrinket(candle.getDropPosition(), dropId - !gui->isMaria() * 6));
	else items.push_back(ItemManager::instance().getHeartsOrMoneyBag(candle.getDropPosition(), candle.getDropId()));
	candle.destroy();
}

void Scene::initManagers()
{
	ItemManager::instance().init(MAP_OFFSET, *basicShader, map, platforms, *gui);
	EffectsManager::instance().init(MAP_OFFSET, *spriteShader);
	EnemyManager::instance().init(MAP_OFFSET, *spriteShader, map, platforms);
}