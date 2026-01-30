#pragma once
#ifndef _SCREEN_INCLUDE
#define _SCREEN_INCLUDE

#include <vector>
#include "Sprite.h"
#include "TexturedQuad.h"

class Screen
{
public:
	static Screen* createScreen(ShaderProgram& program, int screenId);
	virtual void update(int deltaTime) = 0;
	virtual void render();

protected:
	virtual void init(ShaderProgram& program) = 0;

public:
	enum screenType
	{
		OPTIONS, TITLE, COUNT
	};
protected:
	ShaderProgram* texProgram;
	vector<Texture> textures;
	vector<Sprite*> sprites;
	vector<TexturedQuad*> quads;
	glm::mat4 projection;
};

#endif // !_SCREEN_INCLUDE