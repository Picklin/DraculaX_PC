#pragma once
#ifndef _CANDLE_ICLUDE
#define _CANDLE_ICLUDE
#include "Item.h"

class Candle
{
private:
	Candle(Item* item) : drop(item) {}

public:
	~Candle();
	static Candle* createCandle(ShaderProgram& shaderProgram, const glm::vec2& position, Item* item);
	static Candle* createTorchCandle(ShaderProgram& shaderProgram, const glm::vec2& position, Item* item);
	//void init(ShaderProgram& shaderProgram, const glm::vec2& position);
	void update(int deltaTime);
	void render();
	void destroy();
	bool isDestroyed() const;
	bool getsRemoved() const;
	Item* getDrop() const { return drop; }

private:
	Item* drop;
	Texture* fireTex = nullptr;
	Texture* baseTex = nullptr;
	Sprite* fire = nullptr;
	Sprite* base = nullptr;
	int endTimer = 0;
	bool destroyed = false;
};

#endif // !_CANDLE_ICLUDE