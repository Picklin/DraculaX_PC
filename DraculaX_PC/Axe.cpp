#include "Axe.h"

void Axe::grab(GUI& gui)
{
	gui.changeItem(GUI::trinketIDs::AXE);
	end();
}

bool Axe::isTrinket() const
{
	return true;
}

void Axe::makeEndSound() const
{
	SoundEngine::instance().grabTrinket();
}
