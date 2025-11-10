#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Player.h"
#include "Game.h"

#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 64
#define FALL_SPEED 4

enum PlayerAnims
{
	IDLE
};

void Player::render()
{
	program->use();
	program->setUniform1i("flip", lookingLeft);
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
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(IDLE, 8);
	sprite->animatorX(IDLE, 4, 0.f, 0.1f, 0.f);

	sprite->changeAnimation(IDLE);
}

void Player::setHitboxes()
{
}

void Player::childUpdate(int deltaTime)
{
	if (bJumping)
	{
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
			if (jumpAngle > 90) bJumping = !tileMap->collisionMoveDown(getTerrainCollisionBox(), &position.y, quadSize.y);
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
	}
	
	bool rightPressed = Game::instance().getKey(GLFW_KEY_RIGHT);
	bool leftPressed = Game::instance().getKey(GLFW_KEY_LEFT);
	lookingLeft = leftPressed || (lookingLeft && !rightPressed);
	position.x += (rightPressed - leftPressed) * 1.f;
	setPosition(position);
	sprite->update(deltaTime);
}

const Hitbox Player::getTerrainCollisionBox() const
{
	Hitbox box;
	box.min = position;
	box.max = box.min + glm::vec2(quadSize);
	return box;
}
