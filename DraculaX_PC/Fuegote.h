#pragma once
#ifndef _FUEGOTE_INCLUDE
#define _FUEGOTE_INCLUDE

#include "Effect.h"

class Fuegote : public Effect
{
protected:
	string getName() const;
	const string getSpritesheet() const;
	void setAnimations();
	void childUpdate(int deltaTime);
};

#endif // !_FUEGOTE_INCLUDE