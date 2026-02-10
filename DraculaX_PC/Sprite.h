#ifndef _SPRITE_INCLUDE
#define _SPRITE_INCLUDE


#include <vector>
#include "Texture.h"
#include "ShaderProgram.h"
#include "AnimKeyframes.h"
#include "PaletteKeyframes.h"


// This class is derived from code seen earlier in TexturedQuad but it is also
// able to manage animations stored as a spritesheet. 


class Sprite
{

private:
	Sprite(const glm::vec2& quadSize, const glm::vec2& sizeInSpritesheet, Texture* spritesheet, ShaderProgram* shader);
	Sprite(const glm::vec2& quadSize, const glm::vec2& topLeft, const glm::vec2& bottomRight, Texture* spritesheet, ShaderProgram* shader);
	bool paletteAnimationEnded();

public:
	// Textured quads can only be created inside an OpenGL context
	static Sprite* createSprite(const glm::ivec2& quadSize, const glm::vec2& sizeInSpritesheet, Texture* spritesheet, ShaderProgram* shader);
	static Sprite* createSprite(const glm::ivec2& quadSize, const glm::vec2& topLeft, const glm::vec2& bottomRight, Texture* spritesheet, ShaderProgram* shader);

	void update(int deltaTime);
	void render();
	void render(glm::vec2& position, int anim, int frame);
	void free();

	void setPosition(const glm::vec2& pos);
	void incPosition(const glm::vec2& inc);
	void setNumberAnimations(int nAnimations);
	void setAnimationSpeed(int animId, int keyframesPerSec);
	void addKeyframe(int animId, const glm::vec2& frame);
	void setTransition(int animFrom, int animTo);
	void setKeyframe(int frame) { currentKeyframe = frame; }
	void changeAnimation(int animId);
	void setColorPalette(Texture* palette) { this->palette = palette; }
	void setNumberPaletteAnimations(int nPaletteAnims);
	void setPaletteSpeed(int paletteAnimId, int rowsPerSec);
	void addPaletteKeyframe(int paletteAnimId, float paletteRow);
	void setPaletteTransition(int paletteAnimFrom, int paletteAnimTo);
	void setPaletteRow(float paletteRow) { currentPaletteRow = paletteRow; }
	void changePaletteAnimation(int paletteAnimId);
	void setAngleDegrees(float angleDegrees);
	void setCenter(const glm::vec2& center);
	void setColor(const glm::vec3 & color);
	void setAlpha(float alpha) { this->color.a = alpha; }
	void setScale(glm::vec2 scale);
	void invertColor() { invert = true; }
	void addAnimations(const vector<AnimKeyframes>& anims);
	int animation() const;
	int paletteAnimation() const;

	bool animationEnded();
	int getCurrentKeyframe() const;
	int getNumAnimations() const;
	int getNumFrames(int animId) const;
	float getAngleDegrees() const;
	const vector<AnimKeyframes>& getAnimations() const;
	const glm::vec2& getPosition() const;

	void animatorX(int animId, int numFrames, float beginOffset, float inc, float yOffset);
	void animatorY(int animId, int numFrames, float beginOffset, float inc, float xOffset);

private:
	vector<AnimKeyframes> animations;
	vector<PaletteKeyframes> palettesAnims;
	glm::vec4 color = glm::vec4(1.f);
	glm::vec2 quadSize;
	glm::vec2 center;
	glm::vec2 scale = glm::vec2(1.f);
	glm::vec2 position;
	glm::vec2 texCoordDispl;
	Texture* texture;
	Texture* palette = nullptr;
	ShaderProgram* shaderProgram;
	GLuint vao;
	GLuint vbo;
	GLint posLocation, texCoordLocation;
	int currentAnimation, currentKeyframe;
	int currentPaletteAnim, currentPaletteKeyframe;
	float timeAnimation;
	float timePaletteAnimation;
	float currentPaletteRow;
	float angleDegrees = 0.f;
	bool animationDoneOnce = false;
	bool paletteAnimationDoneOnce = false;
	bool invert = false;
};


#endif // _SPRITE_INCLUDE

