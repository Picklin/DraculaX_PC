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
        shader->setUniform4f("color", 1, 1, 1, alpha);
    }

private:
    Texture fontTexture;
    string file;
    ShaderProgram* shader;
    GLuint vao, vbo;
    int CHAR_WIDTH, CHAR_HEIGHT, MAX_CHARS;

};

#endif // !_TEXT_INCLUDE
