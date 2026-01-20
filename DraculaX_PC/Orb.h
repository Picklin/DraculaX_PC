#pragma once
#ifndef _ORB_INCLUDE
#define _ORB_INCLUDE

#include "Item.h"

class Orb : public Item
{
public:
	void init(const glm::ivec2& tileMapDispl, ShaderProgram& shader, Texture& tex);
	void update(int deltaTime) override;
	void grab(GUI& gui) override;
	bool isGrabable() const override;

protected:
	void makeEndSound() const override;

private:
	bool grabbed = false;
};

#endif // !_ORB_INCLUDE