#include "Scene.h"
#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Game.h"

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
}

void Scene::update(int deltaTime)
{
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
		for (unsigned int i = 0; i < items.size(); i++)
		{
			if (!items[i]->isEnded() && items[i]->isGrabable() && collision(items[i]->getHitbox(), player->getHitbox()[0]))	//habrá que cambiar la hitbox a que no devuelva un vector
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
}

void Scene::initManagers()
{
	ItemManager::instance().init(MAP_OFFSET, *basicShader, map, platforms, *gui);
	EffectsManager::instance().init(MAP_OFFSET, *spriteShader);
	EnemyManager::instance().init(MAP_OFFSET, *spriteShader, map, platforms);
}