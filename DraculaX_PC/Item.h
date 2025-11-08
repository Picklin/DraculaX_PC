#pragma once
#ifndef _ITEM_INCLUDE
#define _ITEM_INCLUDE

#include "Player.h"

class Item
{
public:
	void init(const glm::ivec2& tileMapDispl, ShaderProgram& program, const glm::vec2& topLeft, const glm::vec2& bottomRight, Texture& itemsTex);
	void setTileMap(TileMap& map);
	void update(int deltaTime);
	void render();
	void setUngrabable();
	void setPlatformMap(TileMap* platforms) { this->platforms = platforms; }
	void setPosition(const glm::vec2& pos);
	void eject(int xDir);
	void end();
	virtual void makeEndSound() const = 0;
	virtual void grab(Player* player) = 0;
	const vector<Hitbox> getHitbox() const;
	virtual bool isTrinket() const { return false; }
	bool isGrabable() const;
	bool isEnded() const;
	bool getsRemoved() const;

protected:
	const glm::vec2 myCenter() const;

public:
	enum Trinkets {
		DRUG, SHURIKEN, SPREAD, WATCH
	};

protected:
	glm::vec2 position;

private:
	glm::ivec2 tileMapDispl;
	glm::ivec2 quadSize;
	TileMap* platforms;
	TileMap* tileMap;
	Sprite* sprite;
	const int lifeSpan = 5000;	//milisegundos
	const int ungrabableTime = 1000;
	int timeElapsed = ungrabableTime;

	//para cuando se lanze el que ya tenía equipado el jugador al coger un trinket nuevo
	float velocityX = 0.f;
	float velocityY = 0.f;
	int xDir;
	const float minSpeedX = 3.f;
	const float friction = 0.15f;
	const float minSpeedY = 3.f;
	const float gravity = 0.3f;
	float endTimer = 1;
	bool beingEjected = false;
	bool ended = false;
};

#endif // !_ITEM_INCLUDE