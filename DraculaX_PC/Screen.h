#pragma once
#ifndef _SCREEN_INCLUDE
#define _SCREEN_INCLUDE

#include <vector>
#include "Sprite.h"
#include "TexturedQuad.h"
#include "Text.h"

class Screen
{
public:
	static Screen* createScreen(ShaderProgram& program, int screenId);
	virtual void update(int deltaTime) = 0;
	virtual void render();

protected:
	virtual void init(ShaderProgram& program);

public:
	enum screenType
	{
		OPTIONS, TITLE, COUNT
	};
protected:
	ShaderProgram* texProgram;
	vector<Sprite*> sprites;
	vector<TexturedQuad*> quads;
};

#endif // !_SCREEN_INCLUDE