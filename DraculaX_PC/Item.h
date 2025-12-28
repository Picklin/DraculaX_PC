#pragma once
#ifndef _ITEM_INCLUDE
#define _ITEM_INCLUDE

#include "GUI.h"
#include "SoundEngine.h"

class Item
{
public:
	void init(const glm::ivec2& tileMapDispl, ShaderProgram& shader, const glm::vec2& topLeft, const glm::vec2& bottomRight, Texture& itemsTex);
	void setTileMap(TileMap& map);
	void update(int deltaTime);
	void render();
	void setUngrabable();
	void setPlatformMap(TileMap* platforms) { this->platforms = platforms; }
	void setPosition(const glm::vec2& pos);
	void eject(int xDir);
	void end();
	virtual void grab(GUI& gui) = 0;
	const Hitbox getHitbox() const;
	bool isGrabable() const;
	bool isEnded() const;
	bool getsRemoved() const;

	static void setUngrabableTime(int timeMilisecs);

protected:
	virtual void makeEndSound() const = 0;
	const glm::vec2 myCenter() const;

protected:
	glm::vec2 position;

private:
	glm::ivec2 tileMapDispl;
	glm::ivec2 quadSize;
	TileMap* platforms;
	TileMap* tileMap;
	Sprite* sprite;
	const int lifeSpan = 5000;	//milisegundos
	static int ungrabableTime;
	int timeElapsed;
	float velocityX = 0.f;
	float velocityY = 0.f;
	int xDir;
	float endTimer = 1;
	bool beingEjected = false;
	bool ended = false;
};

#endif // !_ITEM_INCLUDE