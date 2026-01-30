#pragma once
#ifndef _OPTIONS1_INCLUDE
#define _OPTIONS1_INCLUDE

#include "Screen.h"

class Options : public Screen
{
public:
	void update(int deltaTime) override;

protected:
	void init(ShaderProgram& program) override;

};

#endif // !_OPTIONS1_INCLUDE