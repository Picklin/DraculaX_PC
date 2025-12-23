#include "Turtle.h"

void Turtle::grab(GUI& gui)
{
	gui.changeItem(GUI::trinketIDs::TURTLE);
	end();
}

bool Turtle::isTrinket() const
{
	return true;
}

void Turtle::makeEndSound() const
{
	SoundEngine::instance().grabTrinket();
}
