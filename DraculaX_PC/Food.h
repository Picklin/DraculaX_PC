#pragma once
#ifndef _FOOD_INCLUDE
#define _FOOD_INCLUDE

#include "Item.h"

class Food : public Item
{
public:
	void grab(GUI& gui) override;
	void setFoodID(int foodID);

protected:
	void makeEndSound() const override;

private:
	int foodID;
};

#endif // !_FOOD_INCLUDE