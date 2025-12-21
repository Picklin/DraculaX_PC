#include "GUI.h"
#include "TextureManager.h"

enum RichterItems
{
	DAGGER, AXE, HOLY_WATER, STOPWATCH, BIBLE, CROSS
};

enum MariaItems
{
	BIRD, CAT, TURTLE, EGG, BOOK, DRAGON
};

void GUI::init(ShaderProgram& shaderProgram, Player* player, bool secondPlayer)
{
	isMaria = false;
	// isMaria = player->getName() == "Maria";
	this->player = player;
	this->shader = &shaderProgram;
	this->secondPlayer = secondPlayer;
	tex.loadFromFile("images/gui/gui&items.png", TEXTURE_PIXEL_FORMAT_RGBA);
	tex.setMagFilter(GL_NEAREST);
	TextureManager::instance().addTexture("gui&items", &tex);
	healthFrame = TexturedQuad::createTexturedQuad(glm::ivec2(32, 96), glm::vec2(0.125f * isMaria, 0.375f*secondPlayer), glm::vec2(0.125f + 0.125f * isMaria, 0.375f+0.375f*secondPlayer), tex, shaderProgram);
	score = TexturedQuad::createTexturedQuad(glm::ivec2(24, 8), glm::vec2(0.75f, 0.f), glm::vec2(0.84375f, 0.03125f), tex, shaderProgram);
	credit_rest = TexturedQuad::createTexturedQuad(glm::ivec2(64, 8), glm::vec2(0.75f, 0.03125f), glm::vec2(1.f, 0.0625f), tex, shaderProgram);
	boards[0] = TexturedQuad::createTexturedQuad(glm::ivec2(64, 16), glm::vec2(0.75f, 0.0625f), glm::vec2(1.f, 0.125f), tex, shaderProgram);
	boards[1] = TexturedQuad::createTexturedQuad(glm::ivec2(64, 16), glm::vec2(0.75f, 0.125f), glm::vec2(1.f, 0.1875f), tex, shaderProgram);
	dragon = TexturedQuad::createTexturedQuad(glm::ivec2(32, 32), glm::vec2(0.6875f, 0.75f), glm::vec2(0.8125f, 0.875f), tex, shaderProgram);
	boardFrame = Sprite::createSprite(glm::ivec2(64, 16), glm::vec2(0.25f + 0.25f * isMaria, 0.f), glm::vec2(0.5f + 0.25f * isMaria, 0.06125f), &tex, &shaderProgram);
	boardFrame->setNumberAnimations(2);
	boardFrame->setAnimationSpeed(0, 0);
	boardFrame->addKeyframe(0, glm::vec2(0.f, 0.f));
	boardFrame->setAnimationSpeed(1, 16);
	boardFrame->animatorY(1, 3, 0.06125f, 0.06125f, 0.f);
	boardFrame->setTransition(1, 0);
	boardFrame->changeAnimation(0);
	int secondPlayerOffset = 208 * secondPlayer;
	for (int i = 0; i < 7; ++i)
	{
		scoreNumbers[i] = Sprite::createSprite(glm::ivec2(8, 8), glm::vec2(0.f, 0.f), glm::vec2(0.03125f, 0.03125f), &tex, &shaderProgram);
		setNumberAnims(scoreNumbers[i]);
		scoreNumbers[i]->setPosition(glm::vec2(38 + (6 - i) * 8, 8 + secondPlayerOffset));
	}
	for (int i = 0; i < 4; ++i)
	{
		creditNumbers[i] = Sprite::createSprite(glm::ivec2(8, 8), glm::vec2(0.f, 0.f), glm::vec2(0.03125f, 0.03125f), &tex, &shaderProgram);
		setNumberAnims(creditNumbers[i]);
		creditNumbers[i]->setPosition(glm::vec2(38 + i * 8, 8 + secondPlayerOffset));
	}
	for (int i = 0; i < 2; ++i)
	{
		lifesNumbers[i] = Sprite::createSprite(glm::ivec2(8, 8), glm::vec2(0.f, 0.f), glm::vec2(0.03125f, 0.03125f), &tex, &shaderProgram);
		setNumberAnims(lifesNumbers[i]);
		lifesNumbers[i]->setPosition(glm::vec2(78 + (1 - i) * 8, 8 + secondPlayerOffset));
	}
	for (int i = 0; i < 2; ++i)
	{
		heartsNumbers[i] = Sprite::createSprite(glm::ivec2(8, 8), glm::vec2(0.f, 0.f), glm::vec2(0.03125f, 0.03125f), &tex, &shaderProgram);
		setNumberAnims(heartsNumbers[i]);
		heartsNumbers[i]->setPosition(glm::vec2(14 + (1 - i) * 8, 6 + secondPlayerOffset));
	}
	item = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.f,0.f), glm::vec2(0.0625f, 0.0625f), &tex, &shaderProgram);
	setItem(item);
	healthFrame->setPosition(glm::vec2(0, 128 * secondPlayer));
	score->setPosition(glm::vec2(35, 3 + secondPlayerOffset));
	credit_rest->setPosition(glm::vec2(35, 3 + secondPlayerOffset));
	boards[0]->setPosition(glm::vec2(32, secondPlayerOffset));
	boards[1]->setPosition(glm::vec2(32, secondPlayerOffset));
	boardFrame->setPosition(glm::vec2(32, secondPlayerOffset));
	boards[0]->setColor(glm::vec4(0.f, 0.f, 104 / 255.f, 1.f));
	boards[1]->setColor(glm::vec4(0.f, 0.f, 104 / 255.f, 1.f));
	item->setPosition(glm::vec2(13, 12 + secondPlayerOffset));
	dragon->setPosition(glm::vec2(5, 2 + 176 * secondPlayer));

	hp = 100;
	currentScore = 0;
	currentCredits = 0;
	currentHearts = 10;
	currentLifes = 3;
	refreshNumber(heartsNumbers, 2, currentHearts);
	refreshNumber(lifesNumbers, 2, currentLifes);
}

void GUI::update(int deltaTime)
{
	boardFrame->update(deltaTime);
}

void GUI::render()
{
	healthFrame->render();
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
	//dragon->render();
	item->render();
}

void GUI::toggleBoardFrame()
{
	boardFrame->changeAnimation(1);
	paused = !paused;
}

void GUI::takeDmg(int dmg)
{
	hp -= dmg;
	if (hp < 0) hp = 0;
}

void GUI::heal(int heal)
{
	hp += heal;
	if (hp > 100) hp = 100;
}

void GUI::gainHearts(int hearts)
{
	currentHearts += hearts;
	refreshNumber(heartsNumbers, 2, currentHearts);
}

void GUI::gainScore(int score)
{
	currentScore += score;
	refreshNumber(scoreNumbers, 7, currentScore);
}

void GUI::gainCredits(int credits)
{
	currentCredits += credits;
	refreshNumber(creditNumbers, 4, currentCredits);
}

void GUI::oneUp()
{
	currentLifes++;
	refreshNumber(lifesNumbers, 2, currentLifes);
}

void GUI::respawn()
{
	hp = 100;
	currentHearts = 10;
	currentLifes--;
	refreshNumber(heartsNumbers, 2, currentHearts);
	refreshNumber(lifesNumbers, 2, currentLifes);
}

void GUI::reset()
{
	hp = 100;
	currentHearts = 10;
	currentScore = 0;
	currentCredits = 0;
	currentLifes = 3;
	refreshNumber(heartsNumbers, 2, currentHearts);
	refreshNumber(lifesNumbers, 2, currentLifes);
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
	if (isMaria)
	{
		item->setNumberAnimations(6);
		item->addKeyframe(5, glm::vec2(0.125f, 0.8125f));
		item->addKeyframe(BIRD, glm::vec2(0.375f, 0.75f));
		item->addKeyframe(CAT, glm::vec2(0.4375f, 0.75f));
		item->addKeyframe(TURTLE, glm::vec2(0.5f, 0.75f));
		item->addKeyframe(EGG, glm::vec2(0.5625f, 0.75f));
		item->addKeyframe(BOOK, glm::vec2(0.725f, 0.75f));
	}
	else
	{
		item->setNumberAnimations(7);
		item->addKeyframe(6, glm::vec2(0.125f, 0.8125f));
		item->addKeyframe(DAGGER, glm::vec2(0.f, 0.75f));
		item->addKeyframe(AXE, glm::vec2(0.0625f, 0.75f));
		item->addKeyframe(HOLY_WATER, glm::vec2(0.125f, 0.75f));
		item->addKeyframe(STOPWATCH, glm::vec2(0.1875f, 0.75f));
		item->addKeyframe(BIBLE, glm::vec2(0.25f, 0.75f));
		item->addKeyframe(CROSS, glm::vec2(0.3125f, 0.75f));
	}
	item->changeAnimation(5 + !isMaria);
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
