#include "Scene.h"
#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Game.h"
#include "TextureManager.h"

const string Scene::stageClearStr[2] = { "STAGE CLEAR", "NIVEL\nSUPERADO" };
const string Scene::stageTitles[56] = {
	"Prologue", "Prólogo", "Prologue", "Prólogo",
	"Dinner of Flames", "Cena en Llamas", "Birthplace of Tragedy", "El Origen de la Tragedia",
	"God, Grant Me Strength", "Dios, Otórgame Fuerzas", "Oh Lord, Please Give Me Strength", "Oh Señor, Por Favor Dame Fuerzas",
	"Breaking Through the Front Door", "Abriéndose Paso por la Puerta Principal", "I Hate Taking the Long Way Around", "Odio Tomar el Camino Más Largo",
	"Crucifix on the Breast", "Crucifijo Junto al Pecho", "Crucifix Next to the Heart", "Crucifijo Junto al Corazón",
	"An Evil Prayer Summons Darkness", "Una Oración Malvada Invoca la Oscuridad", "The Vengeful King of Bloodshed", "El Rey Vengativo de la Matanza",
	"Release From the Thirst for Blood", "Saciado de la Sed de Sangre", "Let Thy Soul Be At Peace...", "Deja Que Tu Alma Esté En Paz...",
	"Atop Countless Terrors", "Por Encima de Incontables Horrores", "Atop the Corpses of Thy Brethren", "Sobre los Cadáveres de Tus Hermanos",
	"Fortress of the Water Demon", "La Fortaleza del Demonio Acuático", "The Final Divergence", "La Divergencia Final",
	"The Devil Flies By Night", "El Diablo Vuela de Noche", "Toward the Tower of the Final Showdown", "Hacia la Torre del Enfrentamiento Final",
	"Wandering", "Vagando", "Shudder", "Estremecimiento",
	"A Nightmare Reborn", "El Renacer de una Pesadilla", "Undying Melody", "Melodía Inmortal",
	"Hear Now the Requiem of Blood", "Escuchad Ahora el Réquiem de Sangre", "Believe in the Dawn", "Ten Fe en el Amanecer",
	"Bloodlines", "Linajes", "The Brink of Death", "Al Borde de la Muerte"
};
const glm::vec2 Scene::stageNumCoords[14] = {
	glm::vec2(0.0f, 0.6f),
	glm::vec2(0.1f, 0.6f),
	glm::vec2(0.2f, 0.6f),
	glm::vec2(0.3f, 0.6f),
	glm::vec2(0.4f, 0.6f),
	glm::vec2(0.5f, 0.6f),
	glm::vec2(0.6f, 0.6f),
	glm::vec2(0.7f, 0.6f),
	glm::vec2(0.8f, 0.6f),
	glm::vec2(0.9f, 0.6f),
	glm::vec2(0.0f, 0.7f),
	glm::vec2(0.1f, 0.7f),
	glm::vec2(0.2f, 0.7f),
	glm::vec2(0.3f, 0.7f),
};

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
	screenWidth = SCREEN_WIDTH;
	blackScreen = TexturedQuad::createTexturedQuad(glm::vec2(0.f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT), *TextureManager::instance().getTexture("pixel"), basicShader);
	blackScreen->setColor(glm::vec3(0.f));
	fadeDuration = 0.5f;
	blackScreenDuration = 0.25f;
	textLanguage = Game::instance().getCurrentTxtLang();
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

void Scene::initStageTitle()
{
	currentStage = Game::instance().getCurrentLevel();
	Texture* stageTexs = TextureManager::instance().getTexture("lvltitle");
	if (currentStage < 12) stage = TexturedQuad::createTexturedQuad(glm::vec2(0.f + textLanguage * 0.5f, 0.f), glm::vec2(0.5f + textLanguage * 0.5f, 0.2f), *stageTexs, *basicShader);
	else stage = TexturedQuad::createTexturedQuad(glm::vec2(0.f, 0.2f), glm::vec2(0.5f, 0.4f), *stageTexs, *basicShader);
	lvlnum = TexturedQuad::createTexturedQuad(stageNumCoords[currentStage], stageNumCoords[currentStage] + 0.1f, *stageTexs, *basicShader);
	triangle = TexturedQuad::createTexturedQuad(glm::vec2(0.f + gui->isMaria() * 0.5f, 0.4f), glm::vec2(0.5f + gui->isMaria() * 0.5f, 0.6f), *stageTexs, *basicShader);
	stage->setPosition(glm::vec2(SCREEN_WIDTH / 2, 0));
	lvlnum->setPosition(glm::vec2(SCREEN_WIDTH / 2 + 59, 0));
	triangle->setPosition(glm::vec2(SCREEN_WIDTH / 2, 0));
	titleSpeed = 4.f;
	stageTitle.init(*basicShader, "images/fonts/font_intro.png", glm::ivec2(6, 12), 64);
	stageTitleStr = stageTitles[currentStage * 4 + textLanguage + gui->isMaria() * 2];
}

void Scene::updateActors(int deltaTime)
{
	if (!player->isEnded())
	{
		player->update(deltaTime);
		if (player->usingSubweapon())
		{
			Axe* axe = new Axe();
			int dir = player->getLookingDirection();
			axe->init(MAP_OFFSET, *spriteShader, dir);
			glm::vec2 pos = player->getPosition() + player->myCenter();
			axe->setPosition(glm::vec2(pos.x - 32 * (dir == -1), pos.y));
			subweapons.push_back(axe);
			gui->gainHearts(-1);
			SoundEngine::instance().playSFX(SoundEngine::AXE);
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
		for (unsigned int i = 0; i < enemies.size(); i++)
		{
			if (oobEnemy(enemies[i]->getPosition() + enemies[i]->myCenter()))
			{
				delete enemies[i];
				enemies.erase(enemies.begin() + i);
			}
			else
			{
				enemies[i]->update(deltaTime);
				if (!player->wounded() && !enemies[i]->isEnded() && collision(enemies[i]->getHitbox(), player->getHitbox()))
				{
					if (enemies[i]->isAttacking())
					{
						gui->takeDmg(enemies[i]->getDamageFromAttack());
					}
					else
					{
						gui->takeDmg(enemies[i]->getContactDamage());
					}
				}
				if (!enemies[i]->isEnded())
				{
					if (player->isAttacking() && !enemies[i]->wounded() && collision(enemies[i]->getHitbox(), player->getWhipHitbox()))
					{
						enemies[i]->takeDmg(7);
					}
					for (auto subwpn : subweapons)
					{
						if (!enemies[i]->wounded() && collision(enemies[i]->getHitbox(), subwpn->getHitbox())) enemies[i]->takeDmg(subwpn->getDamage());
					}
				}
				if (enemies[i]->getsRemoved())
				{
					gui->gainScore(enemies[i]->getScore());
					delete enemies[i];
					enemies.erase(enemies.begin() + i);
				}
			}
		}
		for (unsigned int i = 0; i < projectiles.size(); i++)
		{
			projectiles[i]->update(deltaTime);

			if (!player->wounded() && collision(projectiles[i]->getHitbox(), player->getHitbox()))
			{
					gui->takeDmg(projectiles[i]->getDamage());
					projectiles[i]->end();
			}
			if (oobProjectile(projectiles[i]->getPosition()) || projectiles[i]->getsRemoved())
			{
				delete projectiles[i];
				projectiles.erase(projectiles.begin() + i);
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

void Scene::renderTitle()
{
	if (!start)
	{
		basicShader->use();
		triangle->render();
		stage->render();
		lvlnum->render();
		stageTitle.render(stageTitleStr, glm::vec2(triangle->getPosition().y + 56, SCREEN_WIDTH / 2 + 12));
	}
}

void Scene::calcIncrement(float& valToInc, float targetVal, float factor)
{
	if (abs(valToInc - targetVal) > 0.2f) valToInc = valToInc + (targetVal - valToInc) * factor;
	else valToInc = targetVal;
}

void Scene::calcEaseIn(float& valToInc, float startVal, float targetVal, float factor)
{
	factor = factor * factor;
	valToInc = startVal + (targetVal - startVal) * factor;
}

void Scene::updateStageTitle(int deltaTime)
{
	glm::vec2 titlePos(triangle->getPosition());
	if (!titleAppeared && titleSpeed > 0)
	{
		titlePos.y += titleSpeed;
		stage->setPosition(titlePos);
		lvlnum->setPosition(glm::vec2(titlePos.x + 59, titlePos.y));
		triangle->setPosition(titlePos);
		calcIncrement(titleSpeed, 0.f, 0.035f);
	}
	else if (!start && timeElapsed >= 2)
	{
		titleAppeared = true;
		titlePos.y += titleSpeed;
		stage->setPosition(titlePos);
		lvlnum->setPosition(glm::vec2(titlePos.x + 59, titlePos.y));
		triangle->setPosition(titlePos);
		calcEaseIn(titleSpeed, 0.f, 4.f, timeElapsed - 2);
		if (timeElapsed > 4)
		{
			start = true;
			player->unlockInput();
		}
	}
}

bool Scene::collision(const Hitbox& hitbox1, const Hitbox& hitbox2)
{
	return ((hitbox1.min.x < hitbox2.max.x) && (hitbox2.min.x < hitbox1.max.x) && (hitbox1.min.y < hitbox2.max.y) && (hitbox2.min.y < hitbox1.max.y));	
}

bool Scene::oobProjectile(const glm::vec2& pos) const
{
	return pos.x < minX - CAMERA_X || pos.y < minY - CAMERA_Y || pos.x > maxX || pos.y > maxY;
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
	glm::vec2 pos(candle.getDropPosition());
	if (dropId >= GUI::BIRD) items.push_back(ItemManager::instance().getTrinket(pos, dropId - !gui->isMaria() * 6));
	else items.push_back(ItemManager::instance().getHeartsOrMoneyBag(pos, dropId));
	candle.destroy();
	SoundEngine::instance().playSFX(candle.getSFXId());
	EffectsManager::instance().createFuegote(pos+glm::vec2(8), glm::vec4(1.f), false);
}

void Scene::initManagers()
{
	ItemManager::instance().init(MAP_OFFSET, *basicShader, map, platforms, *gui);
	EffectsManager::instance().init(MAP_OFFSET, *basicShader);
	EnemyManager::instance().init(MAP_OFFSET, *spriteShader, map, platforms);
	ProjectileManager::instance().init(MAP_OFFSET, *spriteShader, map, &projectiles);
}