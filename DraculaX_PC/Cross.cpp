#include "Cross.h"

void Cross::grab(GUI& gui)
{
	gui.changeItem(GUI::trinketIDs::CROSS);
	end();
}

bool Cross::isTrinket() const
{
	return true;
}

void Cross::makeEndSound() const
{
	SoundEngine::instance().grabTrinket();
}
