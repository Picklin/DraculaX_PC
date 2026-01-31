#ifndef _SPRITE_INCLUDE
#define _SPRITE_INCLUDE


#include <vector>
#include "Texture.h"
#include "ShaderProgram.h"
#include "AnimKeyframes.h"


// This class is derived from code seen earlier in TexturedQuad but it is also
// able to manage animations stored as a spritesheet. 


class Sprite
{

private:
	Sprite(const glm::vec2& quadSize, const glm::vec2& sizeInSpritesheet, Texture* spritesheet, ShaderProgram* shader);
	Sprite(const glm::vec2& quadSize, const glm::vec2& topLeft, const glm::vec2& bottomRight, Texture* spritesheet, ShaderProgram* shader);

public:
	// Textured quads can only be created inside an OpenGL context
	static Sprite* createSprite(const glm::ivec2& quadSize, const glm::vec2& sizeInSpritesheet, Texture* spritesheet, ShaderProgram* shader);
	static Sprite* createSprite(const glm::ivec2& quadSize, const glm::vec2& topLeft, const glm::vec2& bottomRight, Texture* spritesheet, ShaderProgram* shader);

	void update(int deltaTime);
	void render();
	void render(glm::vec2& position, int anim, int frame);
	void free();

	void setPosition(const glm::vec2& pos);
	void setNumberAnimations(int nAnimations);
	void setAnimationSpeed(int animId, int keyframesPerSec);
	void setTransition(int animFrom, int animTo);
	void setAngleDegrees(float angleDegrees);
	void setCenter(const glm::vec2& center);
	void setColor(const glm::vec3 & color);
	void setAlpha(float alpha) { this->color.a = alpha; }
	void setScale(glm::vec2 scale);
	void setKeyframe(int frame) { currentKeyframe = frame; }
	void invertColor() { invert = true; }
	void addKeyframe(int animId, const glm::vec2& frame);
	void addAnimations(const vector<AnimKeyframes>& anims);
	void changeAnimation(int animId);
	void changeAnimation(int animId, int frame);
	int animation() const;

	const bool animationEnded();
	const int getCurrentKeyframe() const;
	const int getNumAnimations() const;
	const int getNumFrames(int animId) const;
	float getAngleDegrees() const;
	float getTimeAnimation() const;
	const vector<AnimKeyframes>& getAnimations() const;

	void animatorX(int animId, int numFrames, float beginOffset, float inc, float yOffset);
	void animatorY(int animId, int numFrames, float beginOffset, float inc, float xOffset);


private:
	Texture* texture;
	ShaderProgram* shaderProgram;
	GLuint vao;
	GLuint vbo;
	GLint posLocation, texCoordLocation;
	glm::vec2 quadSize;
	glm::vec2 center;
	glm::vec2 scale = glm::vec2(1.f);
	glm::vec2 position;
	float angleDegrees = 0.f;
	int currentAnimation, currentKeyframe;
	float timeAnimation;
	glm::vec2 texCoordDispl;
	vector<AnimKeyframes> animations;
	bool animationDoneOnce = false;
	glm::vec4 color = glm::vec4(1.f);
	bool invert = false;
};


#endif // _SPRITE_INCLUDE

