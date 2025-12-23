#include "Book.h"

void Book::grab(GUI& gui)
{
	gui.changeItem(GUI::trinketIDs::BOOK);
	end();
}

bool Book::isTrinket() const
{
	return true;
}

void Book::makeEndSound() const
{
	SoundEngine::instance().grabTrinket();
}
