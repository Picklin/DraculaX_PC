#pragma once
#ifndef _WATCHINCLUDE
#define _WATCHINCLUDE

#include "Item.h"

class Watch : public Item
{
public:
	void grab(GUI& gui) override;
	bool isTrinket() const override;
	int getTrinketID() const override;

protected:
	void makeEndSound() const override;
};

#endif // !_WATCHINCLUDE