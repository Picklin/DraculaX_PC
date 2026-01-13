#include <map>
#include "ItemManager.h"
#include "TextureManager.h"

namespace
{
    const int foodEquivalents[12]
    {
        GUI::foodIds::ROAST,
        GUI::foodIds::ROAST,
        GUI::foodIds::ROAST,
        GUI::foodIds::ROAST,
        GUI::foodIds::HALF_ROAST,
        GUI::foodIds::ROAST,
        GUI::foodIds::ROAST,
        GUI::foodIds::ROAST,
        GUI::foodIds::ROAST,
        GUI::foodIds::ROAST,
        GUI::foodIds::ROAST,
        GUI::foodIds::BIG_ROAST
    };
    const pair<glm::vec2, glm::vec2> RichterFoodCoords[3]
    {
        pair<glm::vec2, glm::vec2>(glm::vec2(0.f, 0.875f), glm::vec2(0.0625f, 0.9375f)),
        pair<glm::vec2, glm::vec2>(glm::vec2(0.f, 0.9375f), glm::vec2(0.0625f, 1.f)),
        pair<glm::vec2, glm::vec2>(glm::vec2(0.0625f, 0.875f), glm::vec2(0.1875f, 1.f))
    };
}


ItemManager::ItemManager() : map(nullptr), platforms(nullptr), shader(nullptr)
{
    initializeRandom();
    itemsDist = std::discrete_distribution<int>(itemWeights.begin(), itemWeights.end());
    commonItemsDist = std::discrete_distribution<int>(commonItemWeights.begin(), commonItemWeights.end());

    heartsMoneyBags =
    {
        [this](const glm::vec2& pos) { return this->getSmallHeart(pos); },
        [this](const glm::vec2& pos) { return this->getHeart(pos); },
		[this](const glm::vec2& pos) { return this->getOneHundredMoneyBag(pos); },
		[this](const glm::vec2& pos) { return this->getFourHundredMoneyBag(pos); },
		[this](const glm::vec2& pos) { return this->getSevenHundredMoneyBag(pos); },
		[this](const glm::vec2& pos) { return this->getThousandMoneyBag(pos); },
        //[this](const glm::vec2& pos) { return this->getBigHeart(pos); },
	};

    itemsTex = TextureManager::instance().getTexture("gui&items");
}

void ItemManager::initializeRandom()
{
    randomEngine.seed((unsigned int)std::chrono::high_resolution_clock::now().time_since_epoch().count());
}

void ItemManager::initItem(Item* item, const glm::vec2& pos, const glm::vec2& topLeft, const glm::vec2& bottomRight)
{
    item->init(tileMapDispl, *shader, topLeft, bottomRight, *itemsTex);
    item->setTileMap(*map);
    item->setPlatformMap(platforms);
    item->setPosition(pos);
}

ItemManager& ItemManager::instance()
{
    static ItemManager im;
    return im;
}

void ItemManager::init(glm::ivec2& tileMapDispl, ShaderProgram& shader, TileMap* map, TileMap* platforms, GUI& gui)
{
    this->tileMapDispl = tileMapDispl;
    this->shader = &shader;
    this->map = map;
    this->platforms = platforms;
}



Item* ItemManager::getRandomItem(const glm::vec2& position)
{
    int randomIndex = itemsDist(randomEngine);
    return randomItem[randomIndex](position);
}

Item* ItemManager::getCommonRandomItem(const glm::vec2& position)
{
    int randomIndex = commonItemsDist(randomEngine);
    return commonRandomItem[randomIndex](position);
}

Item* ItemManager::getHeartsOrMoneyBag(const glm::vec2& position, int id)
{
	return heartsMoneyBags[id](position);
}

Item* ItemManager::getTrinket(const glm::vec2& position, int trinketID)
{
	Trinket* trinket = new Trinket();
    initItem(trinket, position, glm::vec2(0.0625f * trinketID + (trinketID == GUI::trinketIDs::KEY) * 0.0625f, 0.75f),
        glm::vec2(0.0625f + 0.0625f * trinketID + 0.0625f * (trinketID == GUI::trinketIDs::DRAGON) + (trinketID == GUI::trinketIDs::KEY) * 0.0625f,
            0.8125f + (trinketID == GUI::trinketIDs::DRAGON) * 0.0625f));
	trinket->setTrinketID(trinketID);
	return trinket;
}

Item* ItemManager::getFood(const glm::vec2& position, int foodID, const GUI& gui)
{
    bool isRichter = !gui.isMaria();
    if (isRichter && foodID >= GUI::foodIds::PARFAIT)
    {
        foodID = foodEquivalents[foodID - 3];
    }
    Food* food = new Food();
    if (foodID < GUI::foodIds::PARFAIT)
    {
        initItem(food, position, RichterFoodCoords[foodID].first, RichterFoodCoords[foodID].second);
    }
    else
    {
        initItem(food, position, glm::vec2(0.0625f * foodID, 0.875f + 0.0625f * (foodID != GUI::foodIds::BIRTHDAY_CAKE)),
            glm::vec2(0.0625f + 0.0625f * foodID + 0.0625f * (foodID == GUI::foodIds::BIRTHDAY_CAKE), 1.f));
    }
    food->setFoodID(foodID);
    return food;
}

Item* ItemManager::getSmallHeart(const glm::vec2& position)
{
	TexturedQuad* heartTone = TexturedQuad::createTexturedQuad(glm::vec2(0.875f, 0.1875f), glm::vec2(0.9375f, 0.25f), *itemsTex, *shader);
    Heart* heart = new Heart(heartTone);
    initItem(heart, position, glm::vec2(0.75f, 0.1875f), glm::vec2(0.8125f, 0.25f));
	return heart;
}

Item* ItemManager::getHeart(const glm::vec2& position)
{
    TexturedQuad* heartTone = TexturedQuad::createTexturedQuad(glm::vec2(0.9375f, 0.1875f), glm::vec2(1.f, 0.25f), *itemsTex, *shader);
    Heart* heart = new Heart(heartTone, 5);
    initItem(heart, position, glm::vec2(0.8125f, 0.1875f), glm::vec2(0.875f, 0.25f));
    return heart;
}

Item* ItemManager::getBigHeart(const glm::vec2& position)
{
    TexturedQuad* heartTone = TexturedQuad::createTexturedQuad(glm::vec2(0.875f, 0.25f), glm::vec2(1.f, 0.375f), *itemsTex, *shader);
    Heart* heart = new Heart(heartTone, 50);
    initItem(heart, position, glm::vec2(0.75f, 0.25f), glm::vec2(0.875f, 0.375f));
    return heart;
}

Item* ItemManager::getOneHundredMoneyBag(const glm::vec2& position)
{
    Bag* bag = new Bag(100);
	initItem(bag, position, glm::vec2(0.75f, 0.125f), glm::vec2(0.8125f, 0.1875f));
	return bag;
}

Item* ItemManager::getFourHundredMoneyBag(const glm::vec2& position)
{
	Bag* bag = new Bag(400);
	initItem(bag, position, glm::vec2(0.8125f, 0.125f), glm::vec2(0.875f, 0.1875f));
    return bag;
}

Item* ItemManager::getSevenHundredMoneyBag(const glm::vec2& position)
{
	Bag* bag = new Bag(700);
	initItem(bag, position, glm::vec2(0.875f, 0.125f), glm::vec2(0.9375f, 0.1875f));
    return bag;
}

Item* ItemManager::getThousandMoneyBag(const glm::vec2& position)
{
	Bag* bag = new Bag(1000);
    bag->initOneThousandBag(tileMapDispl, *shader, *itemsTex);
    bag->setTileMap(*map);
    bag->setPlatformMap(platforms);
    bag->setPosition(position);
	return bag;
}
