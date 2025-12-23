#include "Bird.h"

void Bird::grab(GUI& gui)
{
	gui.changeItem(GUI::trinketIDs::BIRD);
	end();
}

bool Bird::isTrinket() const
{
	return true;
}

void Bird::makeEndSound() const
{
	SoundEngine::instance().grabTrinket();
}
