#include "Bible.h"

void Bible::grab(GUI& gui)
{
	gui.changeItem(GUI::trinketIDs::BIBLE);
	end();
}

bool Bible::isTrinket() const
{
	return true;
}

void Bible::makeEndSound() const
{
	SoundEngine::instance().grabTrinket();
}
