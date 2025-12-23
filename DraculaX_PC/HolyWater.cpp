#include "HolyWater.h"

void HolyWater::grab(GUI& gui)
{
	gui.changeItem(GUI::trinketIDs::HOLY_WATER);
	end();
}

bool HolyWater::isTrinket() const
{
	return true;
}

int HolyWater::getTrinketID() const
{
	return GUI::trinketIDs::HOLY_WATER;
}

void HolyWater::makeEndSound() const
{
	SoundEngine::instance().grabTrinket();
}
