#pragma once
#include "Item.h"
class Dragon :
    public Item
{
public:
	void grab(GUI& gui) override;
	bool isTrinket() const override;
	int getTrinketID() const override { return GUI::trinketIDs::DRAGON; };

protected:
	void makeEndSound() const override;
};

