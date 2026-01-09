#pragma once
#ifndef _HEART_INCLUDE
#define _HEART_INCLUDE

#include "Item.h"
#include "TexturedQuad.h"

class Heart : public Item
{
public:
	Heart(TexturedQuad* tone, int toneId);
	Heart(TexturedQuad* tone, int toneId, int heartAmmount);
	void update(int deltaTime) override;
	void render() override;
	void setPosition(const glm::vec2& pos) override;
	void grab(GUI& gui) override;

protected:
	void makeEndSound() const override;

private:
	TexturedQuad* tone;
	int heartAmmount;
	float timeElapsed = 0.f;
	float xAnchor = 0.f;
};

#endif // !_HEART_INCLUDE