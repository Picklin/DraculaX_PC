#include "ItemManager.h"
#include "TextureManager.h"

ItemManager::ItemManager() : map(nullptr), platforms(nullptr), shader(nullptr)
{
    initializeRandom();
    trinkets = {
        [this](const glm::vec2& pos) { return getDagger(pos); },
		[this](const glm::vec2& pos) { return getAxe(pos); },
		[this](const glm::vec2& pos) { return getHolyWater(pos); },
		[this](const glm::vec2& pos) { return getWatch(pos); },
		[this](const glm::vec2& pos) { return getBible(pos); },
        [this](const glm::vec2& pos) {return getCross(pos); },
		[this](const glm::vec2& pos) { return getBird(pos); },
		[this](const glm::vec2& pos) { return getCat(pos); },
		[this](const glm::vec2& pos) { return getTurtle(pos); },
        [this](const glm::vec2& pos) { return getEgg(pos); },
		[this](const glm::vec2& pos) {return getBook(pos); },
		[this](const glm::vec2& pos) {return getDragon(pos); }
	};

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

Item* ItemManager::getDagger(const glm::vec2& position)
{
	Dagger* dagger = new Dagger();
	initItem(dagger, position, glm::vec2(0.0f, 0.75f), glm::vec2(0.0625f, 0.8125f));
	return dagger;
}

Item* ItemManager::getAxe(const glm::vec2& position)
{
    Axe* axe = new Axe();
    initItem(axe, position, glm::vec2(0.0625f, 0.75f), glm::vec2(0.125f, 0.8125f));
	return axe;
}

Item* ItemManager::getHolyWater(const glm::vec2& position)
{
	HolyWater* holyWater = new HolyWater();
	initItem(holyWater, position, glm::vec2(0.125f, 0.75f), glm::vec2(0.1875f, 0.8125f));
	return holyWater;
}

Item* ItemManager::getWatch(const glm::vec2& position)
{
	Watch* watch = new Watch();
	initItem(watch, position, glm::vec2(0.1875f, 0.75f), glm::vec2(0.25f, 0.8125f));
	return watch;
}

Item* ItemManager::getBible(const glm::vec2& position)
{
    Bible* bible = new Bible();
    initItem(bible, position, glm::vec2(0.25f, 0.75f), glm::vec2(0.3125f, 0.8125f));
	return bible;
}

Item* ItemManager::getCross(const glm::vec2& position)
{
    Cross* cross = new Cross();
    initItem(cross, position, glm::vec2(0.3125f, 0.75f), glm::vec2(0.375f, 0.8125f));
	return cross;
}

Item* ItemManager::getBird(const glm::vec2& position)
{
    Bird* bird = new Bird();
    initItem(bird, position, glm::vec2(0.375f, 0.75f), glm::vec2(0.4375f, 0.8125f));
    return bird;
}

Item* ItemManager::getCat(const glm::vec2& position)
{
	Cat* cat = new Cat();
	initItem(cat, position, glm::vec2(0.4375f, 0.75f), glm::vec2(0.5f, 0.8125f));
	return cat;
}

Item* ItemManager::getTurtle(const glm::vec2& position)
{
	Turtle* turtle = new Turtle();
	initItem(turtle, position, glm::vec2(0.5f, 0.75f), glm::vec2(0.5625f, 0.8125f));
	return turtle;
}

Item* ItemManager::getEgg(const glm::vec2& position)
{
	Egg* egg = new Egg();
	initItem(egg, position, glm::vec2(0.5625f, 0.75f), glm::vec2(0.625f, 0.8125f));
	return egg;
}

Item* ItemManager::getBook(const glm::vec2& position)
{
    Book* book = new Book();
    initItem(book, position, glm::vec2(0.625f, 0.75f), glm::vec2(0.6875f, 0.8125f));
	return book;
}

Item* ItemManager::getDragon(const glm::vec2& position)
{
    Dragon* dragon = new Dragon();
    initItem(dragon, position, glm::vec2(0.6875f, 0.75f), glm::vec2(0.8125f, 0.875f));
	return dragon;
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

Item* ItemManager::getTrinket(const glm::vec2& position, int numTrinket)
{
	return trinkets[numTrinket](position);
}
