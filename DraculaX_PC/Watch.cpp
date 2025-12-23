#include "Watch.h"

void Watch::grab(GUI& gui)
{
	gui.changeItem(GUI::trinketIDs::STOPWATCH);
	end();
}

bool Watch::isTrinket() const
{
	return true;
}

int Watch::getTrinketID() const
{
	return GUI::trinketIDs::STOPWATCH;
}

void Watch::makeEndSound() const
{
	SoundEngine::instance().grabTrinket();
}
