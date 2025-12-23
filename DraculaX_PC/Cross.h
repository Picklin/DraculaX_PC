#pragma once
#include "Item.h"
class Cross :
    public Item
{
public:
	void grab(GUI& gui) override;
	bool isTrinket() const override;
	int getTrinketID() const override { return GUI::trinketIDs::CROSS; };

protected:
	void makeEndSound() const override;
};

