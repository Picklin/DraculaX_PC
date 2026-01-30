#include "Options.h"
#include "TitleScreen.h"
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
	//texProgram->setUniformMatrix4f("projection", projection); //solo cuando cambie la proyección
	for (auto sp : sprites)
	{
		sp->render();
	}
	for (auto quad : quads)
	{
		quad->render();
	}
}
