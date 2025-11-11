#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Player.h"
#include "Game.h"
#include <map>

#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 64
#define FALL_SPEED 4

enum PlayerStates
{
	STATE_IDLE, STATE_WALKING, STATE_JUMPING, STATE_FALLING, STATE_CROUCHING, STATE_ATTACKING
};

enum PlayerAnims
{
	IDLE, WALK, JUMP, JUMP_FW, JUMP_FINAL, FALL, FALL_FINAL, CROUCH, CROUCH_FINAL, GETUP, ATTACK, ATTACK_CROUCH
};

enum Inputs
{
	RIGHT_OR_LEFT, DOWN, UNPRESS_DOWN, UP, A, X
};

std::map<int, int> inputMap = {
	{ RIGHT_OR_LEFT, 1 },
	{ A, 2 },
	{ DOWN, 4 },
	{ UNPRESS_DOWN, 8 },
	{ X, 16 },
	{ UP, 32 },
};

std::map<int, int> animMap = {
	{ 0, PlayerAnims::IDLE },
	{ 1, PlayerAnims::WALK },
	{ 2, PlayerAnims::JUMP },
	{ 3, PlayerAnims::JUMP_FW },
	{ 4, PlayerAnims::CROUCH },
	{ 8, PlayerAnims::GETUP },
	{ 16, PlayerAnims::ATTACK },
	{ 17, PlayerAnims::ATTACK },
	{ 18, PlayerAnims::ATTACK },
	{ 20, PlayerAnims::ATTACK_CROUCH },
	{ 64, PlayerAnims::FALL },
	{ 65, PlayerAnims::FALL },
};

std::map<int, int> animToStateMap = {
	{ PlayerAnims::IDLE, PlayerStates::STATE_IDLE },
	{ PlayerAnims::WALK, PlayerStates::STATE_WALKING },
	{ PlayerAnims::JUMP, PlayerStates::STATE_JUMPING },
	{ PlayerAnims::JUMP_FW, PlayerStates::STATE_JUMPING },
	{ PlayerAnims::JUMP_FINAL, PlayerStates::STATE_JUMPING },
	{ PlayerAnims::FALL, PlayerStates::STATE_FALLING },
	{ PlayerAnims::FALL_FINAL, PlayerStates::STATE_FALLING },
	{ PlayerAnims::CROUCH, PlayerStates::STATE_CROUCHING },
	{ PlayerAnims::CROUCH_FINAL, PlayerStates::STATE_CROUCHING },
	{ PlayerAnims::GETUP, PlayerStates::STATE_IDLE },
	{ PlayerAnims::ATTACK, PlayerStates::STATE_ATTACKING },
	{ PlayerAnims::ATTACK_CROUCH, PlayerStates::STATE_ATTACKING },
};

void Player::render()
{
	shader->use();
	shader->setUniform1i("flip", lookingLeft);
	sprite->render();
}

string Player::getName() const
{
	return "Player";
}

const string Player::getSpritesheet() const
{
	return "images/Richter/Richter.png";
}

const glm::vec2 Player::getSizeInSpritesheet() const
{
	return glm::vec2(0.1f,0.1f);
}

const glm::ivec2 Player::getQuadSize() const
{
	return glm::ivec2(64);
}

void Player::setAnimations()
{
	int crouchSpeed = 20;
	int attackSpeed = 16;

	sprite->setNumberAnimations(12);

	sprite->setAnimationSpeed(IDLE, 8);
	sprite->animatorX(IDLE, 4, 0.f, 0.1f, 0.f);

	sprite->setAnimationSpeed(WALK, 10);
	sprite->animatorX(WALK, 8, 0.f, 0.1f, 0.3f);

	sprite->setAnimationSpeed(JUMP, 16);
	sprite->addKeyframe(JUMP, glm::vec2(0.6f, 0.f));
	sprite->addKeyframe(JUMP, glm::vec2(0.6f, 0.f));
	sprite->addKeyframe(JUMP, glm::vec2(0.7f, 0.2f));

	sprite->setAnimationSpeed(JUMP_FW, 16);
	sprite->addKeyframe(JUMP_FW, glm::vec2(0.7f, 0.f));
	sprite->addKeyframe(JUMP_FW, glm::vec2(0.7f, 0.f));
	sprite->addKeyframe(JUMP_FW, glm::vec2(0.7f, 0.2f));

	sprite->setAnimationSpeed(JUMP_FINAL, 0);
	sprite->addKeyframe(JUMP_FINAL, glm::vec2(0.8f, 0.2f));

	sprite->setAnimationSpeed(FALL, 16);
	sprite->addKeyframe(FALL, glm::vec2(0.7f, 0.2f));
	sprite->addKeyframe(FALL, glm::vec2(0.8f, 0.f));

	sprite->setAnimationSpeed(FALL_FINAL, 0);
	sprite->addKeyframe(FALL_FINAL, glm::vec2(0.9f, 0.f));

	sprite->setAnimationSpeed(CROUCH, crouchSpeed);
	sprite->animatorX(CROUCH, 3, 0.7f, 0.1f, 0.2f);

	sprite->setAnimationSpeed(CROUCH_FINAL, 0);
	sprite->addKeyframe(CROUCH_FINAL, glm::vec2(0.9f, 0.2f));

	sprite->setAnimationSpeed(GETUP, crouchSpeed);
	sprite->animatorX(GETUP, 3, 0.9f, -0.1f, 0.2f);

	sprite->setAnimationSpeed(ATTACK, attackSpeed);
	sprite->animatorX(ATTACK, 7, 0.f, 0.1f, 0.1f);

	sprite->setAnimationSpeed(ATTACK_CROUCH, attackSpeed);
	sprite->animatorX(ATTACK_CROUCH, 7, 0.f, 0.1f, 0.2f);

	sprite->setTransition(JUMP, JUMP_FINAL);
	sprite->setTransition(JUMP_FW, JUMP_FINAL);
	sprite->setTransition(FALL, FALL_FINAL);
	sprite->setTransition(CROUCH, CROUCH_FINAL);
	sprite->setTransition(GETUP, IDLE);
	sprite->setTransition(ATTACK, IDLE);
	sprite->setTransition(ATTACK_CROUCH, CROUCH_FINAL);

	sprite->changeAnimation(IDLE);
}

void Player::setHitboxes()
{
}

void Player::childUpdate(int deltaTime)
{
	int anim = sprite->animation();
	int state = animToStateMap[anim];
	bool rightPressed = Game::instance().getKey(GLFW_KEY_RIGHT);
	bool leftPressed = Game::instance().getKey(GLFW_KEY_LEFT);

	lookingLeft = leftPressed || (lookingLeft && !rightPressed);
	position.x += ((rightPressed - leftPressed) * 1.f) * ((state != STATE_ATTACKING && state != STATE_CROUCHING) || bJumping);

	if (bJumping)
	{
		Game::instance().keyReleased(GLFW_KEY_Z);
		jumpAngle += JUMP_ANGLE_STEP;
		if (jumpAngle >= 180)
		{
			jumpAngle = 0;
			bJumping = false;
			grounded = true;
			position.y = startY;
		}
		else
		{
			position.y = startY - JUMP_HEIGHT * sin(glm::radians((float)jumpAngle));
			bJumping = jumpAngle > 90 || !tileMap->collisionMoveDown(getTerrainCollisionBox(), &position.y, quadSize.y);
			if (jumpAngle == 72 && state != STATE_ATTACKING) sprite->changeAnimation(FALL);
		}
		if (Game::instance().getKey(GLFW_KEY_X) && state != STATE_ATTACKING)
		{
			sprite->changeAnimation(ATTACK);
		}
	}
	else
	{
		bool getup = downPressed && grounded && !Game::instance().getKey(GLFW_KEY_DOWN);
		int inputIndex = 0;
		inputIndex += inputMap[RIGHT_OR_LEFT] * (rightPressed || leftPressed)
			+ inputMap[UP] * Game::instance().getKey(GLFW_KEY_UP)
			+ inputMap[DOWN] * Game::instance().getKey(GLFW_KEY_DOWN)
			+ inputMap[UNPRESS_DOWN] * getup
			+ inputMap[A] * Game::instance().getKey(GLFW_KEY_Z)
			+ inputMap[X] * Game::instance().getKey(GLFW_KEY_X)
			+ 64 * !grounded;
		auto it = animMap.find(inputIndex);
		if (it != animMap.end() && state != animToStateMap[it->second] && state != STATE_ATTACKING)
		{
			sprite->changeAnimation(it->second);
		}
		downPressed = Game::instance().getKey(GLFW_KEY_DOWN);
		
		position.y += FALL_SPEED;
		grounded = tileMap->collisionMoveDown(getTerrainCollisionBox(), &position.y, quadSize.y);
		if (Game::instance().getKey(GLFW_KEY_Z) && grounded)
		{
			bJumping = true;
			grounded = false;
			jumpAngle = 0;
			startY = position.y;
		}
	}
	Game::instance().keyReleased(GLFW_KEY_X);
	
	//cout << jumpAngle << endl;
	setPosition(position);
	sprite->update(deltaTime);
}

const Hitbox Player::getTerrainCollisionBox() const
{
	Hitbox box;
	box.min = position + glm::vec2(22, 21);
	box.max = position + glm::vec2(43, 64);
	return box;
}
