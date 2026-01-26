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
	const wstring msg[2]
	{
		L" PUSH  RUN  BUTTON!",
		L"¡PULSA EL BOTON RUN!"
	};
	int lang;
	float boltTimer;
	int boltDuration;
	float timeBeforeStart;
	bool startPressed;
	bool renderText;
	bool renderBigBolt;
};

#endif // !_TITLESCREEN_INCLUDE