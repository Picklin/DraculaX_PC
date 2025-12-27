#include <vector>
#include <iostream>
#include <map>
#include <sstream>
#include "Text.h"
#include "Game.h"

struct TextVertex {
    glm::vec2 pos;
    glm::vec2 texCoords;
};

const map<char, int> specialCharMap = {
    {'¿', 26},
    {'?', 27},
    {'¡', 28},
    {'!', 29},
    {',', 30},
    {'.', 31},
    {'á', 58},
    {'é', 59},
    {'í', 60},
    {'ó', 61},
    {'ú', 62},
    {'ñ', 63},
    {'Ñ', 26},
};

void Text::init(ShaderProgram& shader, const string& file, const glm::ivec2 & size, int maxChars)
{
	this->shader = &shader;
    this->file = file;
	this->CHAR_WIDTH = size.x;
	this->CHAR_HEIGHT = size.y;
	this->MAX_CHARS = maxChars;
    fontTexture.loadFromFile(file, TEXTURE_PIXEL_FORMAT_RGBA);
	fontTexture.setMagFilter(GL_NEAREST);

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
	this->shader->bindVertexAttribute("position", 2, sizeof(TextVertex), (void*)0);
    this->shader->bindVertexAttribute("texCoord", 2, sizeof(TextVertex), (void*)(sizeof(glm::vec2)));
    glBindVertexArray(0);

	currentAlpha = 1.0f;
	currentColor = glm::vec4(1.f, 1.f, 1.f, 1.f);
}

void Text::render(const std::string& text, glm::vec2 position)
{
    if (text.empty()) return;

    fontTexture.use();
	shader->use();
	shader->setUniform2f("texCoordDispl", 0.f, 0.f);
    shader->setUniform4f("color", currentColor.r, currentColor.g, currentColor.b, currentColor.a);

    std::vector<TextVertex> vertices;
    vertices.reserve(text.size() * 6);

    //float currentX = position.x;
    //float currentY = position.y;

    int texWidth = fontTexture.width();
    int texHeight = fontTexture.height();

    float stepU = (float)CHAR_WIDTH / texWidth;
    float stepV = (float)CHAR_HEIGHT / texHeight;

    int columnsInTexture = fontTexture.width() / CHAR_WIDTH;

	std::stringstream ss(text);
    std::string line;
	int currentY = (int)position.y;
    while (getline(ss, line, '\n'))
    {
		int lineWidth = line.length() * CHAR_WIDTH;
		int startX = (int)position.x - lineWidth / 2;
		int currentX = startX;
        for (char c : line)
        {
		    int charIndex = -1;
            if (c == ' ') {
                currentX += CHAR_WIDTH;
                continue;
            }
            else if (c == '\n') {
                currentX = (int)position.x;
                currentY += CHAR_HEIGHT;
                continue;
            }
            else if (c < 65 || c > 126) {
                if ((c >= 48 && c <= 57) && file == "images/fonts/Letters&Nums.png")
                {
					charIndex = (int(c) - 48 + 30);
                }
                else
                {
                    auto it = specialCharMap.find(c);
                    if (it != specialCharMap.end()) {
                        charIndex = it->second;
                    }
                }
		    }
            else charIndex = (int(c) - 64 - 1);
        
            if (charIndex < 0 || charIndex >= MAX_CHARS) {
			    charIndex = 27;
            }

            // --- CÁLCULO DE UVs ---
            int col = charIndex % columnsInTexture;
            int row = charIndex / columnsInTexture;

            float uMin = col * stepU;
            float vMin = row * stepV;
            float uMax = uMin + stepU;
            float vMax = vMin + stepV;

            // --- DEFINICIÓN DEL QUAD (2 Triángulos) ---
            // Vértice Superior Izquierda
            glm::vec2 posTL(currentX, currentY);
            glm::vec2 uvTL(uMin, vMin);

            // Vértice Inferior Izquierda
            glm::vec2 posBL(currentX, currentY + CHAR_HEIGHT);
            glm::vec2 uvBL(uMin, vMax);

            // Vértice Superior Derecha
            glm::vec2 posTR(currentX + CHAR_WIDTH, currentY);
            glm::vec2 uvTR(uMax, vMin);

            // Vértice Inferior Derecha
            glm::vec2 posBR(currentX + CHAR_WIDTH, currentY + CHAR_HEIGHT);
            glm::vec2 uvBR(uMax, vMax);

            // Triángulo 1
            vertices.push_back({ posTL, uvTL });
            vertices.push_back({ posBL, uvBL });
            vertices.push_back({ posTR, uvTR });

            // Triángulo 2
            vertices.push_back({ posBL, uvBL });
            vertices.push_back({ posBR, uvBR });
            vertices.push_back({ posTR, uvTR });

            // Avanzar el cursor
            currentX += CHAR_WIDTH;
        }
		currentY += CHAR_HEIGHT;
    }

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(TextVertex), vertices.data(), GL_DYNAMIC_DRAW);

    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    glBindVertexArray(0);
}