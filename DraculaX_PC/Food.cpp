#include "Food.h"

const int healAmounts[15]
{
	20,30,92,30,30,30,30,20,30,30,30,30,30,30,92
};

void Food::grab(GUI& gui)
{
	gui.heal(healAmounts[foodID]);
	end();
}

void Food::setFoodID(int foodID)
{
	this->foodID = foodID;
}

void Food::makeEndSound() const
{
	SoundEngine::instance().playSFX(SoundEngine::HEAL);
}
