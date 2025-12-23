#pragma once
#ifndef _AXE_INCLUDE
#define _AXE_INCLUDE

#include "Item.h"

class Axe : public Item
{
public:
	void grab(GUI& gui) override;
	bool isTrinket() const override;
	int getTrinketID() const override { return GUI::trinketIDs::AXE; };

protected:
	void makeEndSound() const override;
};

#endif // !_AXE_INCLUDE