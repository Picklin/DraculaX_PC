#pragma once
#ifndef _TEXT_INCLUDE
#define _TEXT_INCLUDE

#include <string>
#include "Texture.h"
#include "ShaderProgram.h"

class Text
{
public:
    static Text CreateDialogueText(ShaderProgram& shader);
    static Text CreateStageClearText(ShaderProgram& shader);
    static Text CreateLettersAndNumsText(ShaderProgram& shader);
    void render(const wstring& text, glm::vec2 position);
    void setAlpha(float alpha) {
	    currentAlpha = alpha;
    }
    void setColor(const glm::vec4& color) {
        currentColor = color;  
	}

private:
    void init(ShaderProgram& shader, const string & fontName, const glm::ivec2 & size, int maxChars);

private:
    Texture* fontTex;
	glm::vec4 currentColor;
    string fontName;
    ShaderProgram* shader;
    GLuint vao, vbo;
    int CHAR_WIDTH, CHAR_HEIGHT, MAX_CHARS;
	float currentAlpha;
};

#endif // !_TEXT_INCLUDE
