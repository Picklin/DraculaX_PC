#pragma once
#ifndef _ITEM_INCLUDE
#define _ITEM_INCLUDE

#include "GUI.h"
#include "SoundEngine.h"

class Item
{
public:
	~Item();
	void init(const glm::ivec2& tileMapDispl, ShaderProgram& shader, const glm::vec2& topLeft, const glm::vec2& bottomRight, Texture& itemsTex);
	void setTileMap(TileMap& map);
	virtual void update(int deltaTime);
	virtual void render();
	void setUngrabable();
	void setPlatformMap(TileMap* platforms) { this->platforms = platforms; }
	virtual void setPosition(const glm::vec2& pos);
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
	virtual int setEndTimer() { return  1; }
	const glm::vec2 myCenter() const;

protected:
	glm::vec2 position;
	glm::ivec2 quadSize;
	glm::ivec2 tileMapDispl;
	TileMap* platforms;
	TileMap* tileMap;
	int endTimer = 1;
	bool ended = false;

private:
	Sprite* sprite;
	const int lifeSpan = 5000;	//milisegundos
	static int ungrabableTime;
	float velocityX = 0.f;
	float velocityY = 0.f;
	int xDir;
	int timeElapsed;
	bool beingEjected = false;
};

#endif // !_ITEM_INCLUDE