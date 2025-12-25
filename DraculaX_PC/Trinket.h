#pragma once
#ifndef _TRINKET_INCLUDE
#endif // !_TRINKET_INCLUDE

#include "Item.h"

class Trinket : public Item
{
public:
	void grab(GUI& gui) override;
	void setTrinketID(int id) { trinketID = id; }
	int getTrinketID() const;

protected:
	void makeEndSound() const override;

private:
	int trinketID;
};

#define _TRINKET_INCLUDE