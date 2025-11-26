#pragma once
#ifndef _STAIR_INCLUDE
#define _STAIR_INCLUDE

#include <glm/glm.hpp>
#include "Hitbox.h"

struct Stair
{
	Hitbox areaDetect;
	int posX;			//donde el jugador se alinea al empezar a subir o bajar
	int ydistance;
	bool up;
	bool right;
};

#endif // !_STAIR_INCLUDE