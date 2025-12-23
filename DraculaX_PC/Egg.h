#pragma once
#include "Item.h"
class Egg :
    public Item
{
public:
	void grab(GUI& gui) override;
	bool isTrinket() const override;
	int getTrinketID() const override { return GUI::trinketIDs::EGG; };

protected:
	void makeEndSound() const override;
};

