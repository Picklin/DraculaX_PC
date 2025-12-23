#pragma once
#include "Item.h"
class Turtle :
    public Item
{
public:
	void grab(GUI& gui) override;
	bool isTrinket() const override;
	int getTrinketID() const override { return GUI::trinketIDs::TURTLE; };

protected:
	void makeEndSound() const override;
};

