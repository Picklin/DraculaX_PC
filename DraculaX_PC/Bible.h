#pragma once
#ifndef _BIBLE_INCLUDE
#define _BIBLE_INCLUDE

#include "Item.h"

class Bible : public Item
{
public:
	void grab(GUI& gui) override;
	bool isTrinket() const override;
	int getTrinketID() const override { return GUI::trinketIDs::BIBLE; };

protected:
	void makeEndSound() const override;
};

#endif // !_BIBLE_INCLUDE
