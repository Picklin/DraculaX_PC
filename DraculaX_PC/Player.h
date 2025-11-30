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
	void setPlatforms(TileMap* platforms) { this->platforms = platforms; }
	void setStairsMap(TileMap* stairs) { this->stairs = stairs; }

protected:
	string getName() const;
	const string getSpritesheet() const;
	const glm::vec2 getSizeInSpritesheet() const;
	const glm::ivec2 getQuadSize() const;
	void setAnimations();
	void setHitboxes();
	void childUpdate(int deltaTime);

private:
	void stairMovement();
	void calcIncrement(float& valToInc, float targetVal, float factor);
	void registerInput(int key);
	bool checkCommand(const vector<int>& command, const std::chrono::milliseconds& timeWindow) const;
	bool collision(const Hitbox& hitbox1, const Hitbox& hitbox2);
	const Hitbox getTerrainCollisionBox() const;
	const Hitbox getStairsCollisionBox() const;
	const Hitbox getStairsDetectionCollisionBox() const;
	const Hitbox getBelowStairsDetectionCollisionBox() const;

private:
	Afterimages afterimages;

	Texture whipTex;
	Sprite* whip;
	struct InputEvent {
		int key;
		std::chrono::steady_clock::time_point timestamp;
	};
	vector<InputEvent> commandBuffer;
	vector<Hitbox> terrainCollisionBoxes;

	TileMap* platforms;
	TileMap* stairs;

	int jumpAngle = 0;
	int JUMP_HEIGHT = 64;
	int JUMP_ANGLE_STEP = 4;
	int ultTimeElapsed = 0;
	int stairPosX = 0;
	int stairPosY = 0;
	int prevKeyframe = 0;
	int prevAnim = 0;

	float velocityX = 0.f;
	float startY = 0.f;
	float timeRunning = 0.f;
	float dashDistance = 0.f;
	float prevYpos = 0.f;

	//state variables
	bool bJumping = false;
	bool bCrouching = false;
	bool bDashing = false;
	bool bUlting = false;
	bool backflipping = false;
	bool whipping = false;
	bool prevGrounded = true;
	bool grounded = true;
	bool lookingLeft = false;
	bool lookingLeftAtk = false;

	bool prevRightPressed = false;
	bool prevLeftPressed = false;
	bool prevDownPressed = false;
	bool gainMomentum = false;
	bool loseMomentum = false;
	bool dashOffLedge = false;

	//stairs booleans
	bool bClimbing = false;
	bool linedUpStair = false;
	bool rightUpStair = false;
	bool goDown = false;
	bool stepping = false;
	bool stepping2 = false;
};


#endif // _PLAYER_INCLUDE


