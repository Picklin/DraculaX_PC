#pragma once
#include "Item.h"
class Cat :
    public Item
{
public:
	void grab(GUI& gui) override;
	bool isTrinket() const override;
	int getTrinketID() const override { return GUI::trinketIDs::CAT; };

protected:
	void makeEndSound() const override;
};

