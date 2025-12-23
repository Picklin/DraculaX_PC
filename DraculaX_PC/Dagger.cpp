#include "Dagger.h"

void Dagger::grab(GUI& gui)
{
    gui.changeItem(GUI::trinketIDs::DAGGER);
	end();
}

bool Dagger::isTrinket() const
{
    return true;
}

void Dagger::makeEndSound() const
{
	SoundEngine::instance().grabTrinket();
}
