#pragma once
#ifndef _ITEMMANAGER_INCLUDE
#define _ITEMMANAGER_INCLUDE

#include "Trinket.h"
#include "Orb.h"
#include <functional>
#include <random>
#include <chrono>


class ItemManager
{
public:
	static ItemManager& instance();
	void init(const glm::ivec2& tileMapDispl, ShaderProgram& shader, TileMap* map, TileMap* platforms, GUI& gui);
	
	//Item* getRandomItem(const glm::vec2& position);
	//Item* getCommonRandomItem(const glm::vec2& position);
	Item* getHeartsOrMoneyBag(const glm::vec2& position, int id);
	Item* getTrinket(const glm::vec2& position, int trinketID);
	Item* getFood(const glm::vec2& position, int foodID, const GUI& gui);
	Item* getSmallHeart(const glm::vec2& position);
	Item* getHeart(const glm::vec2& position);
	Item* getBigHeart(const glm::vec2& position);
	Item* getOneHundredMoneyBag(const glm::vec2& position);
	Item* getFourHundredMoneyBag(const glm::vec2& position);
	Item* getSevenHundredMoneyBag(const glm::vec2& position);
	Item* getThousandMoneyBag(const glm::vec2& position);
	Orb* getOrb(const glm::vec2& position);

private:
	ItemManager();
	//void initializeRandom();
	void initItem(Item* i, const glm::vec2& pos, const glm::vec2& topLeft, const glm::vec2& bottomRight);

public:
	enum DropIds
	{
		HEART_SMALL, HEART, ONE_HUNDRED, FOUR_HUNDRED, SEVEN_HUNDRED, ONE_THOUSAND, 
	};
	enum trinketIDs
	{
		DAGGER, AXE, HOLY_WATER, STOPWATCH, BIBLE, CROSS, BIRD, CAT, TURTLE, EGG, BOOK, DRAGON, KEY, NONE,
	};
	enum foodIds
	{
		HALF_ROAST, ROAST, BIG_ROAST, PARFAIT, LOLLIPOP, CAKE, FLAN, CHEESE, CUCURUCHO, SUSHI, BURGER, STAR, ICECREAM, RAMEN, BIRTHDAY_CAKE
	};

private:
	using ItemCreator = std::function<Item* (const glm::vec2&)>;
	vector<ItemCreator> heartsMoneyBags;
	glm::ivec2 tileMapDispl;
	TileMap* map;
	TileMap* platforms;
	ShaderProgram* shader;
	Texture* itemsTex;
	Texture* orbTex;
	//vector<ItemCreator> randomItem;
	//vector<ItemCreator> commonRandomItem;
	//vector<float> itemWeights;
	//vector<float> commonItemWeights;
	const int foodEquivalents[12]
	{
		ROAST,
		ROAST,
		ROAST,
		ROAST,
		HALF_ROAST,
		ROAST,
		ROAST,
		ROAST,
		ROAST,
		ROAST,
		ROAST,
		BIG_ROAST
	};

	//std::mt19937 randomEngine;
	//std::discrete_distribution<int> itemsDist;
	//std::discrete_distribution<int> commonItemsDist;

};

#endif // !_ITEMMANAGER_INCLUDE