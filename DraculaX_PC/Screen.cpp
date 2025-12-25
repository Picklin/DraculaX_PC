#include "Screen.h"

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
