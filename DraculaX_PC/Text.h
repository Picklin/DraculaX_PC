#pragma once
#ifndef _TEXT_INCLUDE
#define _TEXT_INCLUDE

#include <string>
#include "Texture.h"
#include "ShaderProgram.h"

class Text
{
public:
    void init(ShaderProgram* shader);
    void render(const std::string& text, glm::vec2 position);

private:
    Texture fontTexture;
    ShaderProgram* shader;
    GLuint vao, vbo;
	GLint posLocation, texCoordLocation;
};

#endif // !_TEXT_INCLUDE
