#pragma once
#ifndef _FUEGUITO_INCLUDE
#define _FUEGUITO_INCLUDE

#include "Effect.h"

class Fueguito : public Effect
{
protected:
	string getName() const;
	const string getSpritesheet() const;
	const glm::vec2 getSizeInSpritesheet() const;
	void setAnimations();
	void childUpdate(int deltaTime);
};

#endif // !_FUEGUITO_INCLUDE