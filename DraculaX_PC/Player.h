#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE

#include "TileMap.h"
#include "Entity.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player : public Entity
{
public:
	void render();

protected:
	string getName() const;
	const string getSpritesheet() const;
	const glm::vec2 getSizeInSpritesheet() const;
	const glm::ivec2 getQuadSize() const;
	void setAnimations();
	void setHitboxes();
	void childUpdate(int deltaTime);

private:
	const Hitbox getTerrainCollisionBox() const;

private:
	vector<Hitbox> terrainCollisionBoxes;

	int jumpAngle = 0;

	float velocityX = 0.f;
	float startY = 0.f;

	bool bJumping = false;
	bool prevGrounded = true;
	bool grounded = true;
	bool lookingLeft = false;
};


#endif // _PLAYER_INCLUDE


