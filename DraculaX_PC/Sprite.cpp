#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Sprite.h"


Sprite* Sprite::createSprite(const glm::ivec2& quadSize, const glm::vec2& sizeInSpritesheet, Texture* spritesheet, ShaderProgram* shader)
{
	Sprite* quad = new Sprite(quadSize, sizeInSpritesheet, spritesheet, shader);

	return quad;
}

Sprite* Sprite::createSprite(const glm::ivec2& quadSize, const glm::vec2& topLeft, const glm::vec2& bottomRight, Texture* spritesheet, ShaderProgram* shader)
{
	Sprite* quad = new Sprite(quadSize, topLeft, bottomRight, spritesheet, shader);

	return quad;
}

Sprite::Sprite(const glm::vec2& quadSize, const glm::vec2& sizeInSpritesheet, Texture* spritesheet, ShaderProgram* shader)
{
	float vertices[24] = { 0.f, 0.f, 0.f, 0.f,
												quadSize.x, 0.f, sizeInSpritesheet.x, 0.f,
												quadSize.x, quadSize.y, sizeInSpritesheet.x, sizeInSpritesheet.y,
												0.f, 0.f, 0.f, 0.f,
												quadSize.x, quadSize.y, sizeInSpritesheet.x, sizeInSpritesheet.y,
												0.f, quadSize.y, 0.f, sizeInSpritesheet.y };

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), vertices, GL_STATIC_DRAW);
	posLocation = shader->bindVertexAttribute("position", 2, 4 * sizeof(float), 0);
	texCoordLocation = shader->bindVertexAttribute("texCoord", 2, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	texture = spritesheet;
	shaderProgram = shader;
	this->quadSize = quadSize;
	center = glm::vec2(quadSize.x / 2, quadSize.y / 2);
	currentAnimation = -1;
	currentKeyframe = -1;
	timeAnimation = 0.f;
	position = glm::vec2(0.f);
}

Sprite::Sprite(const glm::vec2& quadSize, const glm::vec2& topLeft, const glm::vec2& bottomRight, Texture* spritesheet, ShaderProgram* shader)
{
	float vertices[24] = { 0.f,0.f,topLeft.x,topLeft.y,
														quadSize.x, 0.f, bottomRight.x, topLeft.y,
														quadSize.x, quadSize.y, bottomRight.x, bottomRight.y,
														0.f,0.f, topLeft.x, topLeft.y,
														quadSize.x, quadSize.y, bottomRight.x, bottomRight.y,
														0.f, quadSize.y, topLeft.x, bottomRight.y };
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), vertices, GL_STATIC_DRAW);
	posLocation = shader->bindVertexAttribute("position", 2, 4 * sizeof(float), 0);
	texCoordLocation = shader->bindVertexAttribute("texCoord", 2, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	texture = spritesheet;
	shaderProgram = shader;
	this->quadSize = quadSize;
	center = glm::vec2(quadSize.x / 2, quadSize.y / 2);
	currentAnimation = -1;
	currentKeyframe = -1;
	timeAnimation = 0.f;
	position = glm::vec2(0.f);
}

void Sprite::update(int deltaTime)
{
	if (currentAnimation >= 0)
	{
		timeAnimation += deltaTime;
		while (timeAnimation > animations[currentAnimation].millisecsPerKeyframe)
		{
			timeAnimation -= animations[currentAnimation].millisecsPerKeyframe;
			animationDoneOnce = (currentKeyframe + 1) == animations[currentAnimation].keyframeDispl.size();
			currentKeyframe = (currentKeyframe + 1) % animations[currentAnimation].keyframeDispl.size();
		}
		if (animations[currentAnimation].nextAnim != -1 && animationEnded())
		{
			changeAnimation(animations[currentAnimation].nextAnim);
		}
		texCoordDispl = animations[currentAnimation].keyframeDispl[currentKeyframe];
	}
}

void Sprite::render()
{
	glm::mat4 modelview = glm::mat4(1.f);
	modelview = glm::translate(modelview, glm::vec3(position.x, position.y, 0.f));
	modelview = glm::translate(modelview, glm::vec3(center.x, center.y, 0.f));
	modelview = glm::rotate(modelview, glm::radians(angleDegrees), glm::vec3(0.f, 0.f, 1.f));
	modelview = glm::translate(modelview, glm::vec3(-center.x, -center.y, 0.f));
	modelview = glm::scale(modelview, glm::vec3(scale, 1.f));
	shaderProgram->setUniformMatrix4f("modelview", modelview);
	shaderProgram->setUniform2f("texCoordDispl", texCoordDispl.x, texCoordDispl.y);
	shaderProgram->setUniform4f("color", color.x, color.y, color.z, color.a);
	shaderProgram->setUniform1i("invert", invert);
	//glEnable(GL_TEXTURE_2D);
	texture->use();
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	//glDisable(GL_TEXTURE_2D);
	invert = false;
	shaderProgram->setUniform1i("invert", invert);
}

void Sprite::render(glm::vec2& position, int anim, int frame)
{
	texCoordDispl = animations[anim].keyframeDispl[frame];
	glm::mat4 modelview = glm::mat4(1.f);
	modelview = glm::translate(modelview, glm::vec3(position.x, position.y, 0.f));
	shaderProgram->setUniformMatrix4f("modelview", modelview);
	shaderProgram->setUniform2f("texCoordDispl", texCoordDispl.x, texCoordDispl.y);
	shaderProgram->setUniform4f("color", color.x, color.y, color.z, color.a);
	shaderProgram->setUniform1i("invert", invert);
	glEnable(GL_TEXTURE_2D);
	texture->use();
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisable(GL_TEXTURE_2D);
	invert = false;
}

void Sprite::free()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
}

void Sprite::setNumberAnimations(int nAnimations)
{
	animations.clear();
	animations.resize(nAnimations);
}

void Sprite::setAnimationSpeed(int animId, int keyframesPerSec)
{
	if (animId < int(animations.size()))
		animations[animId].millisecsPerKeyframe = 1000.f / keyframesPerSec;
}

void Sprite::addKeyframe(int animId, const glm::vec2& displacement)
{
	if (animId < int(animations.size()))
		animations[animId].keyframeDispl.emplace_back(displacement);
}

void Sprite::addAnimations(const vector<AnimKeyframes>& anims)
{
	this->animations = anims;
}

void Sprite::changeAnimation(int animId)
{
	if (animId < int(animations.size()))
	{
		currentAnimation = animId;
		currentKeyframe = 0;
		timeAnimation = 0.f;
		texCoordDispl = animations[animId].keyframeDispl[0];
		animationDoneOnce = false;
	}
}

void Sprite::changeAnimation(int animId, int frame)
{
	if (animId < int(animations.size()))
	{
		currentAnimation = animId;
		currentKeyframe = frame;
		timeAnimation = 0.f;
		texCoordDispl = animations[animId].keyframeDispl[0];
		animationDoneOnce = false;
	}
}

void Sprite::setTransition(int animFrom, int animTo)
{
	if (animFrom < int(animations.size()) && animTo < int(animations.size()))
	{
		animations[animFrom].nextAnim = animTo;
	}
}

void Sprite::setAngleDegrees(float angleDegrees)
{
	this->angleDegrees = angleDegrees;
}

void Sprite::setCenter(const glm::vec2& center)
{
	this->center = center;
}

void Sprite::setColor(const glm::vec4& color)
{
	this->color = color;
}

void Sprite::setScale(glm::vec2 scale)
{
	this->scale = scale;
}

int Sprite::animation() const
{
	return currentAnimation;
}

void Sprite::setPosition(const glm::vec2& pos)
{
	position = pos;
}

const bool Sprite::animationEnded()
{
	bool ret = animationDoneOnce && currentKeyframe == 0;
	//animationDoneOnce = false;
	return ret;
}

const int Sprite::getCurrentKeyframe() const
{
	return currentKeyframe;
}

const int Sprite::getNumAnimations() const
{
	return animations.size();
}

const int Sprite::getNumFrames(int animId) const
{
	return animations[animId].keyframeDispl.size();
}

float Sprite::getAngleDegrees() const
{
	return angleDegrees;
}

const vector<AnimKeyframes>& Sprite::getAnimations() const
{
	return animations;
}

void Sprite::animatorX(int animId, int numFrames, float beginOffset, float inc, float yOffset)
{
	for (int i = 0; i < numFrames; i++)
	{
		addKeyframe(animId, glm::vec2(beginOffset + inc * i, yOffset));
	}
}

void Sprite::animatorY(int animId, int numFrames, float beginOffset, float inc, float xOffset)
{
	for (int i = 0; i < numFrames; i++)
	{
		addKeyframe(animId, glm::vec2(xOffset, beginOffset + inc * i));
	}
}



