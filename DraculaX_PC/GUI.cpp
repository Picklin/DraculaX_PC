#include "GUI.h"
#include "TextureManager.h"
#include "Game.h"

#define X_OFFSET 8
#define Y_OFFSET 16

namespace
{
	const int heartCosts[14] = {
		1,1,1,5,5,1,1,1,1,1,5,5,0,0
	};
}

void GUI::init(ShaderProgram& shaderProgram, Player* player, bool secondPlayer)
{
	Maria = false;
	// isMaria = player->getName() == "Maria";
	this->player = player;
	this->shader = &shaderProgram;
	tex.loadFromFile("images/gui&items/gui&items.png", TEXTURE_PIXEL_FORMAT_RGBA);
	tex.setMagFilter(GL_NEAREST);
	TextureManager::instance().addTexture("gui&items", &tex);
	healthFrame = TexturedQuad::createTexturedQuad(glm::vec2(0.125f * Maria, 0.375f * secondPlayer), glm::vec2(0.125f + 0.125f * Maria, 0.375f + 0.375f * secondPlayer), tex, shaderProgram);
	hpBar = TexturedQuad::createTexturedQuad(glm::vec2(0.53125f + 0.03125f * Maria, 0.375f), glm::vec2(0.5625f + 0.03125f * Maria, 0.75f), tex, shaderProgram);
	score = TexturedQuad::createTexturedQuad(glm::vec2(0.75f, 0.f), glm::vec2(0.84375f, 0.03125f), tex, shaderProgram);
	credit_rest = TexturedQuad::createTexturedQuad(glm::vec2(0.75f, 0.03125f), glm::vec2(1.f, 0.0625f), tex, shaderProgram);
	boards[0] = TexturedQuad::createTexturedQuad(glm::vec2(0.f, 0.8125f), glm::vec2(0.25f, 0.875f), tex, shaderProgram);
	boards[1] = TexturedQuad::createTexturedQuad(glm::vec2(0.f, 0.8125f), glm::vec2(0.25f, 0.875f), tex, shaderProgram);
	dragon = TexturedQuad::createTexturedQuad(glm::vec2(0.6875f, 0.75f), glm::vec2(0.8125f, 0.875f), tex, shaderProgram);
	boardFrame = Sprite::createSprite(glm::ivec2(64, 16), glm::vec2(0.25f + 0.25f * Maria, 0.f), glm::vec2(0.5f + 0.25f * Maria, 0.06125f), &tex, &shaderProgram);
	boardFrame->setNumberAnimations(2);
	boardFrame->setAnimationSpeed(0, 0);
	boardFrame->addKeyframe(0, glm::vec2(0.f, 0.f));
	boardFrame->setAnimationSpeed(1, 16);
	boardFrame->animatorY(1, 3, 0.06125f, 0.06125f, 0.f);
	boardFrame->setTransition(1, 0);
	boardFrame->changeAnimation(0);
	int secondPlayerOffset = 208 * secondPlayer;
	int yOffset = Y_OFFSET * (!secondPlayer - secondPlayer) * !Game::instance().isTwoPlayerMode();
	int xOffset = X_OFFSET * !Game::instance().isTwoPlayerMode();
	for (int i = 0; i < 7; ++i)
	{
		scoreNumbers[i] = Sprite::createSprite(glm::ivec2(8, 8), glm::vec2(0.f, 0.f), glm::vec2(0.03125f, 0.03125f), &tex, &shaderProgram);
		setNumberAnims(scoreNumbers[i]);
		scoreNumbers[i]->setPosition(glm::vec2(xOffset + 38 + (6 - i) * 8, yOffset + 8 + secondPlayerOffset));
	}
	for (int i = 0; i < 4; ++i)
	{
		creditNumbers[i] = Sprite::createSprite(glm::ivec2(8, 8), glm::vec2(0.f, 0.f), glm::vec2(0.03125f, 0.03125f), &tex, &shaderProgram);
		setNumberAnims(creditNumbers[i]);
		creditNumbers[i]->setPosition(glm::vec2(xOffset + 38 + i * 8, yOffset + 8 + secondPlayerOffset));
	}
	for (int i = 0; i < 2; ++i)
	{
		lifesNumbers[i] = Sprite::createSprite(glm::ivec2(8, 8), glm::vec2(0.f, 0.f), glm::vec2(0.03125f, 0.03125f), &tex, &shaderProgram);
		setNumberAnims(lifesNumbers[i]);
		lifesNumbers[i]->setPosition(glm::vec2(xOffset + 78 + (1 - i) * 8, yOffset + 8 + secondPlayerOffset));
	}
	for (int i = 0; i < 2; ++i)
	{
		heartsNumbers[i] = Sprite::createSprite(glm::ivec2(8, 8), glm::vec2(0.f, 0.f), glm::vec2(0.03125f, 0.03125f), &tex, &shaderProgram);
		setNumberAnims(heartsNumbers[i]);
		heartsNumbers[i]->setPosition(glm::vec2(xOffset + 14 + (1 - i) * 8, yOffset + 6 + secondPlayerOffset));
	}
	item = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.f,0.f), glm::vec2(0.0625f, 0.0625f), &tex, &shaderProgram);
	setItem(item);
	healthFrame->setPosition(glm::vec2(xOffset, yOffset + 128 * secondPlayer));
	hpBar->setPosition(glm::vec2(xOffset, yOffset + 128 * secondPlayer));
	score->setPosition(glm::vec2(xOffset + 35, yOffset + 3 + secondPlayerOffset));
	credit_rest->setPosition(glm::vec2(xOffset + 35, yOffset + 3 + secondPlayerOffset));
	boards[0]->setPosition(glm::vec2(xOffset + 32, yOffset + secondPlayerOffset));
	boards[1]->setPosition(glm::vec2(xOffset + 32, yOffset + secondPlayerOffset));
	boardFrame->setPosition(glm::vec2(xOffset + 32, yOffset + secondPlayerOffset));
	boards[0]->setColor(glm::vec3(0.f, 0.f, 104 / 255.f));
	boards[1]->setColor(glm::vec3(0.f, 0.f, 104 / 255.f));
	item->setPosition(glm::vec2(xOffset + 13, yOffset + 12 + 177 * secondPlayer));
	dragon->setPosition(glm::vec2(xOffset + 5, yOffset + 2 + 177 * secondPlayer));
}

void GUI::update(int deltaTime)
{
	if (dynamicHp < hp)
	{
		dynamicHp++;
		if (dynamicHp > hp) dynamicHp = hp;
	}
	else if (dynamicHp > hp)
	{
		dynamicHp--;
		if (dynamicHp < hp) dynamicHp = hp;
	}
	boardFrame->update(deltaTime);
}

void GUI::render()
{
	shader->use();
	glm::mat4 guiProjection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);
	shader->setUniformMatrix4f("projection", guiProjection);
	healthFrame->render();
	float hpPercent = float(dynamicHp) / MAX_HP;
	float hpPercentAdj = hpPercent * 0.375f + 0.25f;
	shader->setUniform1f("hpPercent", hpPercentAdj);
	hpBar->render();
	shader->setUniform1f("hpPercent", 1.f);
	if (paused)
	{
		boards[1]->render();
		credit_rest->render();
		for (Sprite* num : creditNumbers) num->render();
		for (Sprite* num : lifesNumbers) num->render();
	}
	else
	{
		boards[0]->render();
		score->render();
		for (Sprite* num : scoreNumbers) num->render();
	}
	for (Sprite* num : heartsNumbers) num->render();
	boardFrame->render();
	if (renderDragon) dragon->render();
	else item->render();
}

void GUI::toggleBoardFrame()
{
	boardFrame->changeAnimation(1);
	paused = !paused;
}

void GUI::changeItem(int itemId)
{
	if (itemId == DRAGON)
	{
		renderDragon = true;
		checkHeartsForTrinket();
	}
	else
	{
		renderDragon = false;
		item->changeAnimation(itemId);
		if (itemId == KEY) player->setKey();
		else
		{
			player->unsetKey();
			checkHeartsForTrinket();
		}
	}
}

void GUI::takeDmg(int dmg)
{
	hp -= dmg;
	if (hp < 0) hp = 0;
}

void GUI::heal(int heal)
{
	hp += heal;
	if (hp > MAX_HP) hp = MAX_HP;
}

void GUI::gainHearts(int hearts)
{
	currentHearts += hearts;
	if (currentHearts > 99) currentHearts = 99;
	else if (currentHearts < 0) currentHearts = 0;
	checkHeartsForTrinket();
	refreshNumber(heartsNumbers, 2, currentHearts);
}

void GUI::gainScore(int score)
{
	currentScore += score;
	if (currentScore > 9999999) currentScore = 9999999;
	else if (currentScore < 0) currentScore = 0;
	refreshNumber(scoreNumbers, 7, currentScore);
}

void GUI::gainCredits(int credits)
{
	currentCredits += credits;
	if (currentCredits > 9999) currentCredits = 9999;
	else if (currentCredits < 0) currentCredits = 0;
	refreshNumber(creditNumbers, 4, currentCredits);
}

void GUI::oneUp()
{
	currentLifes++;
	if (currentLifes > 99) currentLifes = 99;
	else if (currentLifes < 0) currentLifes = 0;
	refreshNumber(lifesNumbers, 2, currentLifes);
}

void GUI::respawn()
{
	hp = MAX_HP;
	currentHearts = 10;
	currentLifes--;
	refreshNumber(heartsNumbers, 2, currentHearts);
	refreshNumber(lifesNumbers, 2, currentLifes);
	changeItem(NONE);
}

bool GUI::isMaria() const
{
	return Maria;
}

bool GUI::compatibleTrinket(int trinketId) const
{
	return (trinketId <= CROSS && !Maria) || (trinketId > CROSS && Maria) || trinketId == KEY;
}

bool GUI::compatibleFood(int foodId) const
{
	return (foodId <= BIG_ROAST && !Maria) || (foodId > BIG_ROAST && Maria);
}

int GUI::getCurrentTrinketID() const
{
	return item->animation() * !renderDragon + DRAGON * renderDragon;
}

void GUI::reset()
{
	hp = MAX_HP;
	currentHearts = 1;
	currentScore = 0;
	currentCredits = 0;
	currentLifes = 3;
	refreshNumber(heartsNumbers, 2, currentHearts);
	refreshNumber(lifesNumbers, 2, currentLifes);
	changeItem(NONE);
}

void GUI::setNumberAnims(Sprite* num)
{
	num->setNumberAnimations(10);
	for (int i = 0; i < 10; ++i)
	{
		num->addKeyframe(i, glm::vec2(0.25f + i * 0.03125f, 0.3125));
	}
	num->changeAnimation(0);
}

void GUI::setItem(Sprite* item)
{
	
	item->setNumberAnimations(14);
	item->addKeyframe(DAGGER, glm::vec2(0.f, 0.75f));
	item->addKeyframe(AXE, glm::vec2(0.0625f, 0.75f));
	item->addKeyframe(HOLY_WATER, glm::vec2(0.125f, 0.75f));
	item->addKeyframe(STOPWATCH, glm::vec2(0.1875f, 0.75f));
	item->addKeyframe(BIBLE, glm::vec2(0.25f, 0.75f));
	item->addKeyframe(CROSS, glm::vec2(0.3125f, 0.75f));
	item->addKeyframe(BIRD, glm::vec2(0.375f, 0.75f));
	item->addKeyframe(CAT, glm::vec2(0.4375f, 0.75f));
	item->addKeyframe(TURTLE, glm::vec2(0.5f, 0.75f));
	item->addKeyframe(EGG, glm::vec2(0.5625f, 0.75f));
	item->addKeyframe(BOOK, glm::vec2(0.625f, 0.75f));
	item->addKeyframe(KEY, glm::vec2(0.8125f, 0.75f));
	item->addKeyframe(NONE, glm::vec2(0.25f, 0.8125f));

	item->changeAnimation(NONE);
}

void GUI::refreshNumber(Sprite* num[], int size, int& stat)
{
	int temp = stat;
	for (int i = 0; i < size; i++)
	{
		num[i]->changeAnimation(temp % 10);
		temp /= 10;
	}
}

void GUI::checkHeartsForTrinket() const
{
	int currentTrinketID = getCurrentTrinketID();
	if (currentTrinketID != NONE && currentTrinketID != KEY && currentHearts >= heartCosts[currentTrinketID]) player->setTrinket();
	else player->unsetTrinket();
}
