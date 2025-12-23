#pragma once
#ifndef _DAGGER_INCLUDE
#define _DAGGER_INCLUDE

#include "Item.h"

class Dagger : public Item
{
public:
	void grab(GUI& gui) override;
	bool isTrinket() const override;
	int getTrinketID() const override { return GUI::trinketIDs::DAGGER; };

protected:
	void makeEndSound() const override;
};

#endif // !_DAGGER_INCLUDE
