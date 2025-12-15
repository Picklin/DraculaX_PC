#pragma once
#ifndef _TEXT_INCLUDE
#define _TEXT_INCLUDE

#include <string>
#include "Texture.h"
#include "ShaderProgram.h"

class Text
{
private:
    Text() : shader(nullptr), vao(0), vbo(0) {}

public:
    static Text& instance();
    void init(ShaderProgram* shader);
    void render(const std::string& text, glm::vec2 position);
    void setAlpha(float alpha) {
        shader->setUniform4f("color", 1, 1, 1, alpha);
    }

private:
    Texture fontTexture;
    ShaderProgram* shader;
    GLuint vao, vbo;
};

#endif // !_TEXT_INCLUDE
