#include "ItemManager.h"
#include "TextureManager.h"

ItemManager::ItemManager() : map(nullptr), platforms(nullptr), shader(nullptr)
{
    initializeRandom();

    itemsDist = std::discrete_distribution<int>(itemWeights.begin(), itemWeights.end());

    commonItemsDist = std::discrete_distribution<int>(commonItemWeights.begin(), commonItemWeights.end());

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

void ItemManager::init(glm::ivec2& tileMapDispl, ShaderProgram& shader, TileMap* map, TileMap* platforms)
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

Item* ItemManager::getTrinket(const glm::vec2& position, int trinketID)
{
	Trinket* trinket = new Trinket();
    initItem(trinket, position, glm::vec2(0.0625f * trinketID, 0.75f), 
        glm::vec2(0.0625f + 0.0625f * trinketID + 0.0625f * (trinketID == GUI::trinketIDs::DRAGON), 0.8125f + (trinketID == GUI::trinketIDs::DRAGON) * 0.0625f));
	trinket->setTrinketID(trinketID);
	return trinket;
}
