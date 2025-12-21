#pragma once
#ifndef _GUI_INCLUDE
#define _GUI_INCLUDE

#include "Player.h"
#include "TexturedQuad.h"

class GUI
{
public:
	void init(ShaderProgram& shaderProgram, Player* player, bool secondPlayer);
	void update(int deltaTime);
	void render();
	void toggleBoardFrame();
	void takeDmg(int dmg);
	void heal(int heal);
	void gainHearts(int hearts);
	void gainScore(int score);
	void gainCredits(int credits);
	void oneUp();
	void reset();
	void respawn();

private:
	void setNumberAnims(Sprite* num);
	void setItem(Sprite* item);
	void refreshNumber(Sprite* num[], int size, int& stat);

private:
	Texture tex;
	Player* player;
	TexturedQuad* healthFrame, * score, * credit_rest, * boards[2], * dragon;
	Sprite* boardFrame, * scoreNumbers[7], * creditNumbers[4], * lifesNumbers[2], * heartsNumbers[2], *item;
	ShaderProgram* shader;
	int hp;
	int currentHearts;
	int currentScore;
	int currentCredits;
	int currentLifes;
	bool secondPlayer = false;
	bool isMaria = false;
	bool paused = false;
};

#endif !_GUI_INCLUDE