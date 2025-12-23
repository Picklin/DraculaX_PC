#pragma once
#include "Item.h"
class Book :
    public Item
{
public:
	void grab(GUI& gui) override;
	bool isTrinket() const override;
	int getTrinketID() const override { return GUI::trinketIDs::BOOK; };

protected:
	void makeEndSound() const override;
};

