#include "Trinket.h"

void Trinket::grab(GUI& gui)
{
	gui.changeItem(trinketID);
	end();
}

int Trinket::getTrinketID() const
{
	return trinketID;
}

void Trinket::makeEndSound() const
{
	SoundEngine::instance().grabTrinket();
}
