#include "ItemManager.h"
#include "TextureManager.h"

ItemManager::ItemManager() : map(nullptr), platforms(nullptr), program(nullptr)
{
    initializeRandom();

    itemsDist = std::discrete_distribution<int>(itemWeights.begin(), itemWeights.end());


    commonItemsDist = std::discrete_distribution<int>(commonItemWeights.begin(), commonItemWeights.end());

    //itemsTex = TextureManager::instance().getTexture("all_items");
}

void ItemManager::initializeRandom()
{
    randomEngine.seed((unsigned int)std::chrono::high_resolution_clock::now().time_since_epoch().count());
}

void ItemManager::initItem(Item* item, const glm::vec2& pos, const glm::vec2& topLeft, const glm::vec2& bottomRight)
{
    item->init(tileMapDispl, *program, topLeft, bottomRight, *itemsTex);
    item->setTileMap(*map);
    item->setPlatformMap(platforms);
    item->setPosition(pos);
}

ItemManager& ItemManager::instance()
{
    static ItemManager im;
    return im;
}

void ItemManager::init(glm::ivec2& tileMapDispl, ShaderProgram& program, TileMap* map, TileMap* platforms)
{
    this->tileMapDispl = tileMapDispl;
    this->program = &program;
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