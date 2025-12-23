#pragma once
#ifndef _ITEMMANAGER_INCLUDE
#define _ITEMMANAGER_INCLUDE

#include "TileMap.h"
#include "ShaderProgram.h"
#include "Dagger.h"
#include "Axe.h"
#include "HolyWater.h"
#include "Watch.h"
#include "Bible.h"
#include "Cross.h"
#include "Bird.h"
#include "Cat.h"
#include "Turtle.h"
#include "Egg.h"
#include "Book.h"
#include "Dragon.h"
#include <functional>
#include <random>
#include <chrono>


class ItemManager
{
public:
	static ItemManager& instance();
	void init(glm::ivec2& tileMapDispl, ShaderProgram& shader, TileMap* map, TileMap* platforms);
	Item* getDagger(const glm::vec2& position);
	Item* getAxe(const glm::vec2& position);
	Item* getHolyWater(const glm::vec2& position);
	Item* getWatch(const glm::vec2& position);
	Item* getBible(const glm::vec2& position);
	Item* getCross(const glm::vec2& position);
	Item* getBird(const glm::vec2& position);
	Item* getCat(const glm::vec2& position);
	Item* getTurtle(const glm::vec2& position);
	Item* getEgg(const glm::vec2& position);
	Item* getBook(const glm::vec2& position);
	Item* getDragon(const glm::vec2& position);
	Item* getRandomItem(const glm::vec2& position);
	Item* getCommonRandomItem(const glm::vec2& position);
	Item* getTrinket(const glm::vec2& position, int numTrinket);

private:
	ItemManager();
	void initializeRandom();
	void initItem(Item* i, const glm::vec2& pos, const glm::vec2& topLeft, const glm::vec2& bottomRight);

private:
	glm::ivec2 tileMapDispl;
	TileMap* map;
	TileMap* platforms;
	ShaderProgram* shader;
	using ItemCreator = std::function<Item* (const glm::vec2&)>;
	vector<ItemCreator> randomItem;
	vector<ItemCreator> commonRandomItem;
	vector<ItemCreator> trinkets;
	vector<float> itemWeights;
	vector<float> commonItemWeights;

	std::mt19937 randomEngine;
	std::discrete_distribution<int> itemsDist;
	std::discrete_distribution<int> commonItemsDist;

	Texture* itemsTex;
};

#endif // !_ITEMMANAGER_INCLUDE