#ifndef _TEXTURED_QUAD_INCLUDE
#define _TEXTURED_QUAD_INCLUDE


#include <glm/glm.hpp>
#include "Texture.h"
#include "ShaderProgram.h"


class TexturedQuad
{

private:
	TexturedQuad(const glm::vec2& topLeft, const glm::vec2& bottomRight, Texture& tex, ShaderProgram& program);

public:
	// Textured quads can only be created inside an OpenGL context
	static TexturedQuad* createTexturedQuad(const glm::vec2& topLeft, const glm::vec2& bottomRight, Texture& tex, ShaderProgram& program);
	void setPosition(const glm::vec2& pos);
	void setColor(const glm::vec3& color);
	void setAlpha(float alpha);
	void render() const;
	void free();
	const glm::vec2& getPosition() { return position; }

private:
	glm::vec4 color;
	glm::vec2 position;
	GLuint vao;
	GLuint vbo;
	GLint posLocation, texCoordLocation;
	Texture* tex;
	ShaderProgram* shaderProgram;
};


#endif // _TEXTURED_QUAD_INCLUDE

