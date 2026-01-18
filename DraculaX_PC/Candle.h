#pragma once
#ifndef _CANDLE_ICLUDE
#define _CANDLE_ICLUDE
#include "Sprite.h"
#include "Hitbox.h"

class Candle
{
private:
	Candle(int itemId) : dropId(itemId) {}

public:
	~Candle();
	static Candle* createCandle(ShaderProgram& shaderProgram, const glm::vec2& position, int itemId);
	static Candle* createTorchCandle(ShaderProgram& shaderProgram, const glm::vec2& position, int itemId);
	static Candle* createStreetlightCandle(ShaderProgram& shaderProgram, const glm::vec2& position, int itemId);
	static Candle* createPilarCandle(ShaderProgram& shaderProgram, const glm::vec2& position, int itemId);
	//void init(ShaderProgram& shaderProgram, const glm::vec2& position);
	void update(int deltaTime);
	void render();
	void destroy();
	bool isDestroyed() const;
	bool getsRemoved() const;
	int getDropId() const;
	int getSFXId() const;
	Hitbox getHitbox() const;
	glm::vec2 getDropPosition() const;

private:
	void setHitbox(Candle* candle, const glm::vec2& position);

private:
	Hitbox hitbox;
	Texture* fireTex = nullptr;
	Texture* baseTex = nullptr;
	Sprite* fire = nullptr;
	Sprite* base = nullptr;
	int endTimer = 0;
	int dropId;
	bool destroyed = false;
	bool streetlight = false;
};

#endif // !_CANDLE_ICLUDE