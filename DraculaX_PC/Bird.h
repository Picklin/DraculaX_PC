#pragma once
#ifndef _BIRD_INCLUDE
#define _BIRD_INCLUDE

#include "Item.h"
class Bird : public Item
{
public:
	void grab(GUI& gui) override;
	bool isTrinket() const override;
	int getTrinketID() const override { return GUI::trinketIDs::BIRD; };

protected:
	void makeEndSound() const override;
};

#endif // !_BIRD_INCLUDE