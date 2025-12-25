#include <GL/glew.h>
#include <GL/gl.h>
#include "TexturedQuad.h"
#include <glm/gtc/matrix_transform.hpp>


TexturedQuad* TexturedQuad::createTexturedQuad(const glm::vec2& topLeft, const glm::vec2& bottomRight, Texture& tex, ShaderProgram& program)
{

	TexturedQuad* quad = new TexturedQuad(topLeft, bottomRight, tex, program);
	return quad;
}


TexturedQuad::TexturedQuad(const glm::vec2& topLeft, const glm::vec2& bottomRight, Texture& tex, ShaderProgram& program)
{
	glm::vec2 quadSize(tex.width() * (bottomRight.x - topLeft.x), tex.height() * (bottomRight.y - topLeft.y));
	float vertices[24] = { 0.f, 0.f, topLeft.x, topLeft.y,
		quadSize.x, 0.f, bottomRight.x, topLeft.y,
		quadSize.x, quadSize.y, bottomRight.x, bottomRight.y,
		0.f, 0.f, topLeft.x, topLeft.y,
		quadSize.x, quadSize.y, bottomRight.x, bottomRight.y,
		0.f, quadSize.y, topLeft.x, bottomRight.y };

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), vertices, GL_STATIC_DRAW);
	posLocation = program.bindVertexAttribute("position", 2, 4 * sizeof(float), 0);
	texCoordLocation = program.bindVertexAttribute("texCoord", 2, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	this->tex = &tex;
	this->shaderProgram = &program;
	this->position = glm::vec2(0.f);
	this->color = glm::vec4(1.f);
}

void TexturedQuad::setPosition(const glm::vec2& pos)
{
	this->position = pos;
}

void TexturedQuad::setColor(const glm::vec4& color)
{
	this->color = color;
}

void TexturedQuad::setAlpha(float aplha)
{
	this->color.a = aplha;
}

void TexturedQuad::render() const
{
	glm::mat4 modelview = glm::mat4(1.f);
	modelview = glm::translate(modelview, glm::vec3(position.x, position.y, 0.f));
	shaderProgram->setUniformMatrix4f("modelview", modelview);
	shaderProgram->setUniform4f("color", color.x, color.y, color.z, color.a);
	tex->use();
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void TexturedQuad::free()
{
	glDeleteBuffers(1, &vbo);
}

