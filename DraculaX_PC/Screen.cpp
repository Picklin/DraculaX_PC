#include "Options.h"
#include "TitleScreen.h"
#include "Game.h"
#include <functional>

using ScreenCreator = function<Screen* ()>;
static const ScreenCreator screenCreator[Screen::COUNT] =
{
	[]() {return new Options(); },
	[]() {return new TitleScreen(); },
};

Screen* Screen::createScreen(ShaderProgram& program, int screenId)
{
	Screen* currentMenu = screenCreator[screenId]();
	currentMenu->init(program);
	return currentMenu;
}

void Screen::render()
{
	for (auto sp : sprites)
	{
		sp->render();
	}
	for (auto quad : quads)
	{
		quad->render();
	}
}

void Screen::init(ShaderProgram& program)
{
	program.setUniformMatrix4f("projection", glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f));
	this->texProgram = &program;
}