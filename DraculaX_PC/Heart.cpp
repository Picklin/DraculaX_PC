#include "Heart.h"

namespace
{
	const float fallSpeed[] = { 1.f, 3.f };
	const float amplitude = 16.f;
	const float frequency = 5.f;
	const float redTones[] = {
		72 / 255.f,
		108 / 255.f,
		144 / 255.f,
		180 / 255.f,
		216 / 255.f, 
		252 / 255.f,
	};
}

Heart::Heart(TexturedQuad* tone) : tone(tone), heartAmmount(1)
{
}

Heart::Heart(TexturedQuad* tone, int heartAmmount) : tone(tone), heartAmmount(heartAmmount)
{
}

void Heart::update(int deltaTime)
{
	if (ended)
	{
		endTimer -= deltaTime;
	}
	else
	{
		int colorIndex = int(sin(timeElapsed * 10.f) * 2 + 2);
		tone->setColor(glm::vec4(redTones[colorIndex], 0.f, 0.f, 1.f));
		position.y += fallSpeed[heartAmmount > 1];
		if (!tileMap->collisionMoveDown(getHitbox(), &position.y, quadSize.y - 1) && platforms != nullptr
			&& !platforms->collisionMoveDown(getHitbox(), &position.y, quadSize.y - 1) && heartAmmount == 1)
		{
			float offsetX = sin(timeElapsed * frequency) * amplitude;
			position.x = xAnchor + offsetX;
		}
		Item::setPosition(position);
		timeElapsed += deltaTime / 1000.f;
	}
}

void Heart::render()
{
	tone->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
	tone->render();
	Item::render();
}

void Heart::setPosition(const glm::vec2& pos)
{
	this->position = pos;
	xAnchor = position.x;
}

void Heart::grab(GUI& gui)
{
	gui.gainHearts(heartAmmount);
	end();
}

void Heart::makeEndSound() const
{
	SoundEngine::instance().playSFX(SoundEngine::PICKUP_HEART_SMALL * (heartAmmount == 1) + SoundEngine::PICKUP_HEART * (heartAmmount > 1));
}
