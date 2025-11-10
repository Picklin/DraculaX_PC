#pragma once
#ifndef _EFFECT_INCLUDE
#define _EFFECT_INCLUDE

#include "Sprite.h"

struct TexInfo
{
	Texture* tex;
	glm::vec2 topLeft;
	glm::vec2 bottomRight;
};

class Effect
{
public:
	~Effect();
	virtual void init(const glm::ivec2& tileMapDispl, ShaderProgram& shaderProgram);
	void init(const glm::ivec2& tileMapDispl, ShaderProgram& shaderProgram, const TexInfo& ti);
	void update(int deltaTime);
	void render();
	void setPosition(const glm::vec2& pos);
	bool isEnded() const { return ended; }
	void setColor(const glm::vec4& color) { sprite->setColor(color); }
	void setFlicker();
	void free();

protected:
	virtual string getName() const = 0;
	const virtual string getSpritesheet() const = 0;
	const virtual glm::vec2 getSizeInSpritesheet() const { return glm::vec2(0.25f, 0.25f); }
	virtual void setAnimations() = 0;
	const virtual glm::vec2 myCenter() const { return quadSize / 2; }
	virtual void childUpdate(int deltaTime) = 0;

protected:
	Texture* spritesheet;
	Sprite* sprite;
	glm::ivec2 quadSize;
	glm::ivec2 tileMapDispl;
	glm::vec2 position;
	bool ended = false;
	bool flicker = false;
	bool renderAltern = true;
	ShaderProgram* shader;
};

#endif // !_EFFECT_INCLUDE