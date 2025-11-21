#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE

#include <chrono>
#include "TileMap.h"
#include "Entity.h"
#include "Afterimages.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player : public Entity
{
public:
	void render();
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);

protected:
	string getName() const;
	const string getSpritesheet() const;
	const glm::vec2 getSizeInSpritesheet() const;
	const glm::ivec2 getQuadSize() const;
	void setAnimations();
	void setHitboxes();
	void childUpdate(int deltaTime);

private:
	void registerInput(int key);
	bool checkCommand(const vector<int>& command, const std::chrono::milliseconds& timeWindow) const;
	const Hitbox getTerrainCollisionBox() const;
	void calcIncrement(float& valToInc, float targetVal, float factor);

private:
	Afterimages afterimages;

	struct InputEvent {
		int key;
		std::chrono::steady_clock::time_point timestamp;
	};
	vector<InputEvent> commandBuffer;
	vector<Hitbox> terrainCollisionBoxes;


	int jumpAngle = 0;
	int JUMP_HEIGHT = 64;
	int JUMP_ANGLE_STEP = 4;

	float velocityX = 0.f;
	float startY = 0.f;
	float timeRunning = 0.f;
	float dashDistance = 0.f;
	int ultTimeElapsed = 0;

	bool bJumping = false;
	bool bDashing = false;
	bool bUlting = false;
	bool prevGrounded = true;
	bool grounded = true;
	bool lookingLeft = false;
	bool prevRightPressed = false;
	bool prevLeftPressed = false;
	bool prevDownPressed = false;
	bool gainMomentum = false;
	bool loseMomentum = false;
	bool dashOffLedge = false;
	bool backflipping = false;
};


#endif // _PLAYER_INCLUDE


