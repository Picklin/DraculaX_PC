#pragma once
#ifndef _GUI_INCLUDE
#define _GUI_INCLUDE

#include "Player.h"
#include "TexturedQuad.h"

#define MAX_HP 92

class GUI
{
public:
	void init(ShaderProgram& shaderProgram, Player* player, bool secondPlayer);
	void update(int deltaTime);
	void render();
	void toggleBoardFrame();
	void changeItem(int itemId);
	void takeDmg(int dmg);
	void heal(int heal);
	void gainHearts(int hearts);
	void gainScore(int score);
	void gainCredits(int credits);
	void oneUp();
	void reset();
	void respawn();
	bool compatibleItem(int itemId) const;
	int getCurrentTrinketID() const;

private:
	void setNumberAnims(Sprite* num);
	void setItem(Sprite* item);
	void refreshNumber(Sprite* num[], int size, int& stat);

public:
	enum trinketIDs
	{
		DAGGER, AXE, HOLY_WATER, STOPWATCH, BIBLE, CROSS, BIRD, CAT, TURTLE, EGG, BOOK, DRAGON, KEY, NONE,
	};
private:
	Texture tex;
	Player* player;
	TexturedQuad* healthFrame, * hpBar, * score, * credit_rest, * boards[2], * dragon;
	Sprite* boardFrame, * scoreNumbers[7], * creditNumbers[4], * lifesNumbers[2], * heartsNumbers[2], *item;
	ShaderProgram* shader;
	int hp;
	int dynamicHp;
	int currentHearts;
	int currentScore;
	int currentCredits;
	int currentLifes;
	bool paused = false;
	bool renderDragon = false;
	bool isMaria = false;
};

 #endif	// !_GUI_INCLUDE