#include "Food.h"

const int foodType[15]
{
	20,30,92,30,30,30,30,20,30,30,30,30,30,30,92
};

void Food::grab(GUI& gui)
{
	gui.heal(foodType[foodID]);
	end();
}

void Food::setFoodID(int foodID)
{
	this->foodID = foodID;
}

void Food::makeEndSound() const
{
	SoundEngine::instance().playHeal();
}
