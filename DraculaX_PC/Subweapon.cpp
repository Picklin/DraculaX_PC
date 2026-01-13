#include "Subweapon.h"

Subweapon::~Subweapon()
{
	if (sprite != NULL)
	{
		sprite->free();
		delete sprite;
	}
}

void Subweapon::setPosition(const glm::vec2& pos)
{
	position = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x) + position.x, float(tileMapDispl.y) + position.y));
}