#pragma once
#ifndef _TEXT_INCLUDE
#define _TEXT_INCLUDE

#include <string>
#include "Texture.h"
#include "ShaderProgram.h"

class Text
{
public:
    void init(ShaderProgram& shader, const string& file, const glm::ivec2 & size, int maxChars);
    void render(const string& text, glm::vec2 position);
    void setAlpha(float alpha) {
	    currentAlpha = alpha;
    }
    void setColor(const glm::vec4& color) {
        currentColor = color;  
	}

private:
    Texture fontTexture;
	glm::vec4 currentColor;
    string file;
    ShaderProgram* shader;
    GLuint vao, vbo;
    int CHAR_WIDTH, CHAR_HEIGHT, MAX_CHARS;
	float currentAlpha;
};

#endif // !_TEXT_INCLUDE
