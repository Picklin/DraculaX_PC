#include "Egg.h"

void Egg::grab(GUI& gui)
{
	gui.changeItem(GUI::trinketIDs::EGG);
	end();
}

bool Egg::isTrinket() const
{
	return true;
}

void Egg::makeEndSound() const
{
	SoundEngine::instance().grabTrinket();
}
