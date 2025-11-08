#pragma once
#ifndef _HITBOX_INCLUDE
#define _HITBOX_INCLUDE

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Hitbox {
	glm::vec2 min;
	glm::vec2 max;
	Hitbox operator+(const glm::vec2& pos)
	{
		Hitbox newHb;
		newHb.min = pos + min;
		newHb.max = pos + max;
		return newHb;
	}
};
#endif