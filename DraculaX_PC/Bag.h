#pragma once
#ifndef _BAG_INCLUDE
#define _BAG_INCLUDE

#include "Item.h"
class Bag : public Item
{
public:
	Bag(int scoreAmount);
	void initOneThousandBag(const glm::ivec2& tileMapDispl, ShaderProgram& shader, Texture& itemsTex);
	void update(int deltaTime) override;
	void grab(GUI& gui) override;

protected:
	void makeEndSound() const override;
	int setEndTimer() override;

private:
	int scoreAmount;
};

#endif // !_BAG_INCLUDE