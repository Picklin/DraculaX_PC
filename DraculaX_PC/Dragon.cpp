#include "Dragon.h"

void Dragon::grab(GUI& gui)
{
	gui.changeItem(GUI::trinketIDs::DRAGON);
	end();
}

bool Dragon::isTrinket() const
{
	return true;
}

void Dragon::makeEndSound() const
{
	SoundEngine::instance().grabTrinket();
}
