#ifndef _TEXTURED_QUAD_INCLUDE
#define _TEXTURED_QUAD_INCLUDE


#include <glm/glm.hpp>
#include "Texture.h"
#include "ShaderProgram.h"
#include "PaletteKeyframes.h"


class TexturedQuad
{

private:
	TexturedQuad(const glm::vec2& topLeft, const glm::vec2& bottomRight, Texture& tex, ShaderProgram& program);
	bool paletteAnimationEnded();

public:
	// Textured quads can only be created inside an OpenGL context
	static TexturedQuad* createTexturedQuad(const glm::vec2& topLeft, const glm::vec2& bottomRight, Texture& tex, ShaderProgram& program);
	void incPosition(const glm::vec2 & inc);
	void setPosition(const glm::vec2& pos);
	void setColor(const glm::vec3& color);
	void setAlpha(float alpha);
	//color palette animation methods
	void setColorPalette(Texture* palette) { this->palette = palette; }
	void setNumberPaletteAnimations(int nPaletteAnims);
	void setPaletteSpeed(int paletteAnimId, int rowsPerSec);
	void addPaletteKeyframe(int paletteAnimId, float paletteRow);
	void setPaletteTransition(int paletteAnimFrom, int paletteAnimTo);
	void setPaletteRow(float paletteRow) { currentPaletteRow = paletteRow; }
	void changePaletteAnimation(int paletteAnimId);
	//
	void render() const;
	void free();

	const glm::vec2& getPosition() { return position; }

private:
	vector<PaletteKeyframes> palettesAnims;
	glm::vec4 color;
	glm::vec2 position;
	glm::vec2 texCoordDispl;
	GLuint vao;
	GLuint vbo;
	GLint posLocation, texCoordLocation;
	Texture* tex;
	Texture* palette = nullptr;
	ShaderProgram* shaderProgram;
	int currentPaletteAnim, currentPaletteKeyframe;
	float currentPaletteRow;
	float timePaletteAnimation;
	bool paletteAnimationDoneOnce;
};


#endif // _TEXTURED_QUAD_INCLUDE

