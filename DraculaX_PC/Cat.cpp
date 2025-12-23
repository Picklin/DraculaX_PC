#include "Cat.h"

void Cat::grab(GUI& gui)
{
	gui.changeItem(GUI::trinketIDs::CAT);
	end();
}

bool Cat::isTrinket() const
{
	return true;
}

void Cat::makeEndSound() const
{
	SoundEngine::instance().grabTrinket();
}
