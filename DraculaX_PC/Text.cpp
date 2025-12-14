#include <vector>
#include "Text.h"

#define CHAR_WIDTH 6
#define CHAR_HEIGHT 12
#define MAX_CHARS 64

struct TextVertex {
    glm::vec2 pos;
    glm::vec2 texCoords;
};

void Text::init(ShaderProgram* shader)
{
	this->shader = shader;
    fontTexture.loadFromFile("images/font_intro.png", TEXTURE_PIXEL_FORMAT_RGBA);
	fontTexture.setMagFilter(GL_NEAREST);

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
	posLocation = shader->bindVertexAttribute("position", 2, sizeof(TextVertex), (void*)0);
    texCoordLocation = shader->bindVertexAttribute("texCoord", 2, sizeof(TextVertex), (void*)(sizeof(glm::vec2)));
    glBindVertexArray(0);
}

void Text::render(const std::string& text, glm::vec2 position)
{
    if (text.empty()) return;

    fontTexture.use();
	shader->setUniform2f("texCoordDispl", 0.f, 0.f);

    std::vector<TextVertex> vertices;
    vertices.reserve(text.size() * 6);

    float currentX = position.x;
    float currentY = position.y;

    int texWidth = fontTexture.width();
    int texHeight = fontTexture.height();

    float stepU = (float)CHAR_WIDTH / texWidth;
    float stepV = (float)CHAR_HEIGHT / texHeight;

    int columnsInTexture = fontTexture.width() / CHAR_WIDTH;

    for (char c : text)
    {
        int charIndex = (int(c) - 64 - 1);
        if (c == ' ') {
            currentX += CHAR_WIDTH;
            continue;
        }
        else if (c == '\n') {
            currentX = position.x;
            currentY += CHAR_HEIGHT;
            continue;
        }
        
		//en vez de hacer todo este if else, se puede usar un carácter 
        //que haciendo la operación de charIndex, de el charIndex equivalente
		//por ejemplo, para el '¿' se escribe en el texto el simbolo '[' que al restarle 65 da 26
        //else if (c == '¿') charIndex = 26;
        //else if (c == '?') charIndex = 27;
        //else if (c == '¡') charIndex = 28;
        //else if (c == '!') charIndex = 29;
        //else if (c == ',') charIndex = 30;
        //else if (c == '.') charIndex = 31;
        //else if (c == 'á') charIndex = 58;
        //else if (c == 'é') charIndex = 59;
        //else if (c == 'í') charIndex = 60;
        //else if (c == 'ó') charIndex = 61;
		//aqui ya toca hacer asi porque ya nos salimos del ASCII
        else if (c == 'ú') charIndex = 62;
		else if (c == 'ñ') charIndex = 63;
        
        if (charIndex < 0 || charIndex >= MAX_CHARS) {
			charIndex = 27; //equivale a poner '?' cuando el caracter no esta en el mapa
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

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(TextVertex), vertices.data(), GL_DYNAMIC_DRAW);

    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    glBindVertexArray(0);
}