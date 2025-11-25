#pragma once
#ifndef _STAIR_INCLUDE
#define _STAIR_INCLUDE

#include <glm/glm.hpp>
#include "Hitbox.h"

struct Stair
{
	Hitbox areaDetect;
	int posX;
	bool up;
};

#endif // !_STAIR_INCLUDE