#pragma once
#ifndef _HOLYWATER_INCLUDE
#define _HOLYWATER_INCLUDE

#include "Item.h"

class HolyWater : public Item
{
public:
	void grab(GUI& gui) override;
	bool isTrinket() const override;
	int getTrinketID() const override;

protected:
	void makeEndSound() const override;
};

#endif // !_HOLYWATER_INCLUDE