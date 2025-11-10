#pragma once
#ifndef _ENTITY_INCLUDE
#define _ENTITY_INCLUDE

#include "Sprite.h"
#include "Hitbox.h"
#include "TileMap.h"
#include <iostream>

class Entity
{
public:
	~Entity();

	virtual void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	virtual void render();
	void setTileMap(TileMap* tilemap);
	virtual void setPosition(const glm::vec2& pos);
	void setPivotPoint(const glm::vec2& pivotPoint);
	virtual const glm::vec2 myCenter() const;
	glm::vec2 getPosition() const { return position; }
	glm::vec2* getPointerPos() { return &position; }
	const virtual vector<Hitbox> getHitbox() const;
	bool isEnded() const { return ended; }
	bool getsRemoved() const { return endTimer <= 0; }
	void rotate(float angle);
	virtual void end();
	void free();

protected:
	virtual string getName() const = 0;
	const virtual string getSpritesheet() const = 0;
	const virtual glm::vec2 getSizeInSpritesheet() const = 0;
	const virtual glm::ivec2 getQuadSize() const = 0;
	virtual void setAnimations() = 0;
	virtual int setEndAnimation() { return 0; }
	virtual int setEndTime() { return 0; } //en milisegundos
	virtual void setHitboxes() {}
	virtual void makeEndSound() const {}
	virtual void makeEndEffect() const {}
	virtual void childUpdate(int deltaTime) = 0;
	virtual void endUpdate(int deltaTime) {};
	Hitbox getRotatedHitbox(Hitbox& hitbox, float angle) const;
	glm::vec2 rotatePoint(const glm::vec2& point, const glm::vec2& pivotPoint, float angle) const;

protected:
	Texture* spritesheet;
	Sprite* sprite;
	glm::ivec2 quadSize;
	glm::ivec2 tileMapDispl;
	glm::vec2 position;
	glm::vec2 pivotPoint;
	bool ended = false;
	int endTimer = 1;
	TileMap* tileMap;
	vector<vector<vector<Hitbox>>> hitboxes; //hitboxes de cada frame de cada animacion
	ShaderProgram* shader;
};
#endif // _ENTITY_INCLUDE
