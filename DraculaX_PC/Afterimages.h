#pragma once
#include "Afterimage.h"
#include "Sprite.h"

struct Afterimages
{
	std::vector<Afterimage> lastImages;
	const int MAX_IMAGES = 4;
	const float timeImageInterval = 0.05f;
	float timeSinceLastImage = 0.f;
	Sprite* sprite;
	glm::vec4 color = glm::vec4(1.f);

	void updateAfterimage(int deltaTime, const glm::vec2& position, int anim, int frame)
	{
		timeSinceLastImage += deltaTime / 1000.f;
		if (timeSinceLastImage >= timeImageInterval)
		{
			if ((int)lastImages.size() == MAX_IMAGES) lastImages.erase(lastImages.begin());
			Afterimage image = { position, anim, frame };
			lastImages.emplace_back(image);
			timeSinceLastImage = 0;
		}
	}
	void render()
	{
		int size = (int) lastImages.size();
		for (int i = 0; i < size; i++)
		{
			float alpha = 1.f/size + ((float)i / size);
			sprite->setAlpha(alpha);
			sprite->render(lastImages[i].position, lastImages[i].anim, lastImages[i].keyframe);
		}
	}
	void setColor(const glm::vec4& color)
	{
		this->color = color;
	}
};
