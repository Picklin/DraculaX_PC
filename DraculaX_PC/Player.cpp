#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Player.h"
#include "Game.h"
#include <map>

#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 64
#define FALL_SPEED 4

enum PlayerAnims
{
	IDLE, WALK, JUMP, JUMP_FW, JUMP_FINAL, FALL, FALL_FINAL, CROUCH
};

enum Inputs
{
	RIGHT_OR_LEFT, DOWN, UP, A, X
};

std::map<int, int> inputMap = {
	{ RIGHT_OR_LEFT, 1 },
	{ A, 2 },
	{ DOWN, 4 },
	{ X, 8 },
	{ UP, 16 },
};

std::map<int, PlayerAnims> animMap = {
	{ 0, PlayerAnims::IDLE },
	{ 1, PlayerAnims::WALK },
	{ 2, PlayerAnims::JUMP },
	{ 3, PlayerAnims::JUMP_FW },
	{ 4, PlayerAnims::CROUCH },
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
	sprite->setNumberAnimations(7);

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

	sprite->setTransition(JUMP, JUMP_FINAL);
	sprite->setTransition(JUMP_FW, JUMP_FINAL);
	sprite->setTransition(FALL, FALL_FINAL);

	sprite->changeAnimation(IDLE);
}

void Player::setHitboxes()
{
}

void Player::childUpdate(int deltaTime)
{
	bool rightPressed = Game::instance().getKey(GLFW_KEY_RIGHT);
	bool leftPressed = Game::instance().getKey(GLFW_KEY_LEFT);

	lookingLeft = leftPressed || (lookingLeft && !rightPressed);
	position.x += (rightPressed - leftPressed) * 1.f;

	int anim = sprite->animation();
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
			if (jumpAngle == 72) sprite->changeAnimation(FALL);
			
		}
	}
	else
	{
		position.y += FALL_SPEED;
		grounded = tileMap->collisionMoveDown(getTerrainCollisionBox(), &position.y, quadSize.y);
		if (Game::instance().getKey(GLFW_KEY_Z) && grounded)
		{
			bJumping = true;
			grounded = false;
			jumpAngle = 0;
			startY = position.y;
		}
		int inputIndex = 0;
		inputIndex += inputMap[RIGHT_OR_LEFT] * (rightPressed || leftPressed)
			+ inputMap[UP] * Game::instance().getKey(GLFW_KEY_UP)
			+ inputMap[DOWN] * Game::instance().getKey(GLFW_KEY_DOWN)
			+ inputMap[A] * Game::instance().getKey(GLFW_KEY_Z)
			+ inputMap[X] * Game::instance().getKey(GLFW_KEY_X);
		auto it = animMap.find(inputIndex);
		if (it != animMap.end() && sprite->animation() != it->second)
		{
			sprite->changeAnimation(it->second);
		}
	}
	
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
