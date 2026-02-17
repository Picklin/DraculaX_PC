#include "Options.h"
#include "TitleScreen.h"
#include "Game.h"

Screen* Screen::createScreen(ShaderProgram& program, int screenId)
{
	Screen* currentMenu;
	switch (screenId)
	{
	case OPTIONS:
		currentMenu = new Options();
		break;
	case TITLE:
		currentMenu = new TitleScreen();
		break;
	default:
		currentMenu = nullptr;
	}
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