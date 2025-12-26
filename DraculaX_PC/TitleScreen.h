#pragma once
#ifndef _TITLESCREEN_INCLUDE
#define _TITLESCREEN_INCLUDE

#include "Screen.h"
#include "Text.h"

class TitleScreen : public Screen
{
public:
	void init(ShaderProgram& program) override;
	void update(int deltaTime) override;
	void render() override;

private:
	void reset();

private:
	Text pushRunButton;
	float timeElapsed;
	float boltTimer;
	int boltDuration;
	float timeBeforeStart;
	bool startPressed;
	bool renderText;
	bool renderBigBolt;
};

#endif // !_TITLESCREEN_INCLUDE