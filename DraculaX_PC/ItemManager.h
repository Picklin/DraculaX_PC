#pragma once
#ifndef _ITEMMANAGER_INCLUDE
#define _ITEMMANAGER_INCLUDE

#include "TileMap.h"
#include "ShaderProgram.h"
#include "Trinket.h"
#include "Food.h"
#include "Heart.h"
#include <functional>
#include <random>
#include <chrono>


class ItemManager
{
public:
	static ItemManager& instance();
	void init(glm::ivec2& tileMapDispl, ShaderProgram& shader, TileMap* map, TileMap* platforms, GUI& gui);
	
	Item* getRandomItem(const glm::vec2& position);
	Item* getCommonRandomItem(const glm::vec2& position);
	Item* getHeartsOrMoneyBag(const glm::vec2& position, int id);
	Item* getTrinket(const glm::vec2& position, int trinketID);
	Item* getFood(const glm::vec2& position, int foodID, const GUI& gui);
	Item* getSmallHeart(const glm::vec2& position);
	Item* getHeart(const glm::vec2& position);
	Item* getBigHeart(const glm::vec2& position);

private:
	ItemManager();
	void initializeRandom();
	void initItem(Item* i, const glm::vec2& pos, const glm::vec2& topLeft, const glm::vec2& bottomRight);

public:
	enum DropIds
	{
		HEART_SMALL, HEART, ONE_HUNDRED, FOUR_HUNDRED, SEVEN_HUNDRED, THOUSAND, 
	};

private:
	glm::ivec2 tileMapDispl;
	TileMap* map;
	TileMap* platforms;
	ShaderProgram* shader;
	using ItemCreator = std::function<Item* (const glm::vec2&)>;
	vector<ItemCreator> randomItem;
	vector<ItemCreator> commonRandomItem;
	vector<ItemCreator> heartsMoneyBags;
	vector<float> itemWeights;
	vector<float> commonItemWeights;

	std::mt19937 randomEngine;
	std::discrete_distribution<int> itemsDist;
	std::discrete_distribution<int> commonItemsDist;

	Texture* itemsTex;
};

#endif // !_ITEMMANAGER_INCLUDE