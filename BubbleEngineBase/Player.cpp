#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Player.h"
#include "Game.h"

string Player::getName() const
{
	return "Player";
}

const string Player::getSpritesheet() const
{
	return "images/samus3.png";
}

const glm::vec2 Player::getSizeInSpritesheet() const
{
	return glm::vec2(0.2f,0.125f);
}

const glm::ivec2 Player::getQuadSize() const
{
	return glm::ivec2(32);
}

void Player::setAnimations()
{
}

void Player::setHitboxes()
{
}

void Player::childUpdate(int deltaTime)
{
}
