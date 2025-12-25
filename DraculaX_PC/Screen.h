#pragma once
#ifndef _SCREEN_INCLUDE
#define _SCREEN_INCLUDE

#include <vector>
#include "Sprite.h"
#include "TexturedQuad.h"

class Screen
{
public:
	virtual void init(ShaderProgram& program) = 0;
	virtual void update(int deltaTime) = 0;
	virtual void render();

protected:
	ShaderProgram* texProgram;
	vector<Texture> textures;
	vector<Sprite*> sprites;
	vector<TexturedQuad*> quads;
	glm::mat4 projection;
};

#endif // !_SCREEN_INCLUDE