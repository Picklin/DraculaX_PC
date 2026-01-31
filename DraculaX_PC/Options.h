#pragma once
#ifndef _OPTIONS1_INCLUDE
#define _OPTIONS1_INCLUDE

#include "Screen.h"
#include "TileMap.h"

class Options : public Screen
{
public:
	void update(int deltaTime) override;
	void render() override;

protected:
	void init(ShaderProgram& program) override;
	void apply();

private:
	Text* text;
	TileMap* ui;
	float bgYoffset = 0.f;
	int numLang = 2;
	int currentLang = 0;
	int currentFlamePos = 0; 
	int currentOptions[4] =
	{
		0,0,0,0
	};
	bool upPressed = false;
	bool downPressed = false;
	bool rightPressed = false;
	bool leftPressed = false;
	bool aPressed = false;
};

#endif // !_OPTIONS1_INCLUDE