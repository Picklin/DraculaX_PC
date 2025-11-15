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
	STATE_IDLE, STATE_WALKING, STATE_JUMPING, STATE_FALLING, STATE_CROUCHING, STATE_PREP_ATK, STATE_ATTACKING, STATE_DASHING
};

enum PlayerAnims
{
	IDLE, WALK, RUN, JUMP, JUMP_FW, JUMP_FINAL, FALL, FALL_FINAL, CROUCH, CROUCH_FINAL, GETUP, PREP_ATK, ATTACK, ATTACK_SUBWEAPON, ATTACK_CROUCH, SKID, DASH1, DASH1_FINAL, DASH1_GETUP
};

enum Inputs
{
	RIGHT, LEFT, DOWN, UP, A, X
};

std::map<int, int> inputMap = {
	{ RIGHT, 1 },
	{ LEFT, 1},
	{ A, 2 },
	{ DOWN, 4 },
	{ X, 8 },
	{ UP, 16 },
};

struct Command
{
	vector<int> sequence;
	std::chrono::milliseconds timeWindow;
	int index;
};

const Command RIGHT_RUN_COMMAND = {
	{GLFW_KEY_RIGHT,},
	std::chrono::milliseconds(128),
	64
};

const Command LEFT_RUN_COMMAND = {
	{GLFW_KEY_LEFT,},
	std::chrono::milliseconds(128),
	64
};

std::map<int, int> animMap = {
	{ 0, PlayerAnims::IDLE },
	{ 1, PlayerAnims::WALK },
	{ 2, PlayerAnims::JUMP },
	{ 3, PlayerAnims::JUMP_FW },
	{ 4, PlayerAnims::CROUCH },
	{ 6, PlayerAnims::DASH1 },
	{ 8, PlayerAnims::ATTACK },
	{ 9, PlayerAnims::ATTACK },
	{ 10, PlayerAnims::ATTACK },
	{ 12, PlayerAnims::ATTACK_CROUCH },
	{ 16, PlayerAnims::PREP_ATK},
	{ 17, PlayerAnims::WALK },
	{ 18, PlayerAnims::JUMP },
	{ 19, PlayerAnims::JUMP_FW },
	{ 20, PlayerAnims::CROUCH },
	{ 24, PlayerAnims::ATTACK_SUBWEAPON },
	{ 25, PlayerAnims::ATTACK_SUBWEAPON },
	{ 26, PlayerAnims::ATTACK_SUBWEAPON },
	{ 32, PlayerAnims::FALL },
	{ 33, PlayerAnims::FALL },
	{ 34, PlayerAnims::FALL },
	{ 36, PlayerAnims::FALL },
	{ 38, PlayerAnims::FALL },
	{ 40, PlayerAnims::ATTACK },
	{ 41, PlayerAnims::ATTACK },
	{ 44, PlayerAnims::ATTACK },
	{ 56, PlayerAnims::ATTACK_SUBWEAPON },
	{ 57, PlayerAnims::ATTACK_SUBWEAPON },
	{ 65, PlayerAnims::RUN },
	{ 67, PlayerAnims::JUMP_FW},
	{ 81, PlayerAnims::RUN },
};

std::map<int, int> animToStateMap = {
	{ PlayerAnims::IDLE, PlayerStates::STATE_IDLE },
	{ PlayerAnims::WALK, PlayerStates::STATE_WALKING },
	{ PlayerAnims::RUN, PlayerStates::STATE_WALKING },
	{ PlayerAnims::JUMP, PlayerStates::STATE_JUMPING },
	{ PlayerAnims::JUMP_FW, PlayerStates::STATE_JUMPING },
	{ PlayerAnims::JUMP_FINAL, PlayerStates::STATE_JUMPING },
	{ PlayerAnims::FALL, PlayerStates::STATE_FALLING },
	{ PlayerAnims::FALL_FINAL, PlayerStates::STATE_FALLING },
	{ PlayerAnims::CROUCH, PlayerStates::STATE_CROUCHING },
	{ PlayerAnims::CROUCH_FINAL, PlayerStates::STATE_CROUCHING },
	{ PlayerAnims::GETUP, PlayerStates::STATE_IDLE },
	{ PlayerAnims::PREP_ATK, PlayerStates::STATE_PREP_ATK },
	{ PlayerAnims::ATTACK, PlayerStates::STATE_ATTACKING },
	{ PlayerAnims::ATTACK_CROUCH, PlayerStates::STATE_ATTACKING },
	{ PlayerAnims::ATTACK_SUBWEAPON, PlayerStates::STATE_ATTACKING },
	{ PlayerAnims::SKID, PlayerStates::STATE_IDLE },
	{ PlayerAnims::DASH1, PlayerStates::STATE_DASHING },
	{ PlayerAnims::DASH1_FINAL, PlayerStates::STATE_DASHING },
	{ PlayerAnims::DASH1_GETUP, PlayerStates::STATE_DASHING },
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

	sprite->setNumberAnimations(19);

	sprite->setAnimationSpeed(IDLE, 8);
	sprite->animatorX(IDLE, 4, 0.f, 0.1f, 0.f);

	sprite->setAnimationSpeed(WALK, 10);
	sprite->animatorX(WALK, 8, 0.f, 0.1f, 0.3f);

	sprite->setAnimationSpeed(RUN, 16);
	sprite->animatorX(RUN, 8, 0.f, 0.1f, 0.4f);

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

	sprite->setAnimationSpeed(PREP_ATK, 0);
	sprite->addKeyframe(PREP_ATK, glm::vec2(0.f, 0.1f));

	sprite->setAnimationSpeed(ATTACK, attackSpeed);
	sprite->animatorX(ATTACK, 7, 0.f, 0.1f, 0.1f);

	sprite->setAnimationSpeed(ATTACK_CROUCH, attackSpeed);
	sprite->animatorX(ATTACK_CROUCH, 7, 0.f, 0.1f, 0.2f);

	sprite->setAnimationSpeed(ATTACK_SUBWEAPON, 20);
	sprite->animatorX(ATTACK_SUBWEAPON, 5, 0.f, 0.1f, 0.1f);
	sprite->addKeyframe(ATTACK_SUBWEAPON, glm::vec2(0.7f, 0.1f));
	sprite->addKeyframe(ATTACK_SUBWEAPON, glm::vec2(0.7f, 0.1f));
	sprite->addKeyframe(ATTACK_SUBWEAPON, glm::vec2(0.7f, 0.1f));
	sprite->addKeyframe(ATTACK_SUBWEAPON, glm::vec2(0.7f, 0.1f));

	sprite->setAnimationSpeed(SKID, 32);
	sprite->addKeyframe(SKID, glm::vec2(0.7f, 0.6f));
	sprite->addKeyframe(SKID, glm::vec2(0.8f, 0.6f));
	sprite->addKeyframe(SKID, glm::vec2(0.8f, 0.6f));
	sprite->addKeyframe(SKID, glm::vec2(0.9f, 0.6f));
	sprite->addKeyframe(SKID, glm::vec2(0.9f, 0.6f));
	sprite->addKeyframe(SKID, glm::vec2(0.8f, 0.6f));
	sprite->addKeyframe(SKID, glm::vec2(0.8f, 0.6f));
	sprite->addKeyframe(SKID, glm::vec2(0.9f, 0.6f));
	sprite->addKeyframe(SKID, glm::vec2(0.9f, 0.6f));
	sprite->addKeyframe(SKID, glm::vec2(0.7f, 0.2f));

	sprite->setAnimationSpeed(DASH1, 24);
	sprite->addKeyframe(DASH1, glm::vec2(0.9f, 0.f));
	sprite->animatorX(DASH1, 5, 0.f, 0.1f, 0.5f);

	sprite->setAnimationSpeed(DASH1_FINAL, 0);
	sprite->addKeyframe(DASH1_FINAL, glm::vec2(0.4f, 0.5f));

	sprite->setAnimationSpeed(DASH1_GETUP, 12);
	sprite->animatorX(DASH1_GETUP, 2, 0.3f, -0.1f, 0.5f);
	sprite->addKeyframe(DASH1_GETUP, glm::vec2(0.8f, 0.2f));
	sprite->addKeyframe(DASH1_GETUP, glm::vec2(0.8f, 0.2f));
	sprite->addKeyframe(DASH1_GETUP, glm::vec2(0.8f, 0.2f));

	sprite->setTransition(JUMP, JUMP_FINAL);
	sprite->setTransition(JUMP_FW, JUMP_FINAL);
	sprite->setTransition(FALL, FALL_FINAL);
	sprite->setTransition(CROUCH, CROUCH_FINAL);
	sprite->setTransition(GETUP, IDLE);
	sprite->setTransition(ATTACK, IDLE);
	sprite->setTransition(ATTACK_CROUCH, CROUCH_FINAL);
	sprite->setTransition(ATTACK_SUBWEAPON, IDLE);
	sprite->setTransition(SKID, IDLE);
	sprite->setTransition(DASH1, DASH1_FINAL);
	sprite->setTransition(DASH1_GETUP, CROUCH_FINAL);

	sprite->changeAnimation(IDLE);
}

void Player::setHitboxes()
{
}

void Player::childUpdate(int deltaTime)
{
	int anim = sprite->animation();
	//cout << "Anim: " << anim << " State: " << animToStateMap[anim] << endl;
	int state = animToStateMap[anim];
	bool rightPressed = Game::instance().getKey(GLFW_KEY_RIGHT) * !bDashing;
	bool leftPressed = Game::instance().getKey(GLFW_KEY_LEFT) * !rightPressed * !bDashing;
	lookingLeft = leftPressed || (lookingLeft && !rightPressed);

	if (loseMomentum)
	{
		if (abs(velocityX) == 1 && anim != SKID)
		{
			sprite->changeAnimation(SKID);
		}
		velocityX *= 0.9f;
		if (abs(velocityX) < 0.1f) velocityX = 0.f;
		loseMomentum = velocityX != 0.f;
	}
	
	bool getup = (prevDownPressed && grounded && !Game::instance().getKey(GLFW_KEY_DOWN) && anim != GETUP && state != STATE_DASHING)
		|| (state == STATE_DASHING && !bDashing && !Game::instance().getKey(GLFW_KEY_DOWN));

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
			sprite->changeAnimation(ATTACK+Game::instance().getKey(GLFW_KEY_UP));
		}
	}
	else if (getup)
	{
		sprite->changeAnimation(GETUP);
	}
	else if (!bDashing)
	{
		timeRunning = (timeRunning + (deltaTime / 1000.f)) * (anim == RUN);
		if (dashOffLedge)
		{
			calcIncrement(velocityX, 0.f, 0.075f);
			dashOffLedge = velocityX != 0;
		}
		else
		{
			loseMomentum = (timeRunning >= .5f) && !bJumping && gainMomentum && !(rightPressed || leftPressed);
			gainMomentum = gainMomentum && (rightPressed || leftPressed);
			velocityX = (((rightPressed - leftPressed) + ((!lookingLeft - lookingLeft) * loseMomentum)) * (1.f + 1.f * gainMomentum)) * ((state != STATE_ATTACKING && state != STATE_CROUCHING) || bJumping);
		}
		int inputIndex = 0;
		inputIndex += inputMap[RIGHT] * rightPressed
			+ inputMap[LEFT] * leftPressed
			+ inputMap[UP] * (Game::instance().getKey(GLFW_KEY_UP) * (anim != GETUP))
			+ inputMap[DOWN] * Game::instance().getKey(GLFW_KEY_DOWN)
			+ inputMap[A] * Game::instance().getKey(GLFW_KEY_Z)
			+ inputMap[X] * Game::instance().getKey(GLFW_KEY_X)
			+ 32 * !grounded;
		registerInput(
			GLFW_KEY_RIGHT * (prevRightPressed && !rightPressed)
			+ GLFW_KEY_LEFT * (prevLeftPressed && !leftPressed)
		);
		int commandInputIndex = 0;
		commandInputIndex = RIGHT_RUN_COMMAND.index * (checkCommand(RIGHT_RUN_COMMAND.sequence, RIGHT_RUN_COMMAND.timeWindow) && rightPressed)
			+ LEFT_RUN_COMMAND.index * (checkCommand(LEFT_RUN_COMMAND.sequence, LEFT_RUN_COMMAND.timeWindow) && leftPressed)
			+ RIGHT_RUN_COMMAND.index * (gainMomentum && (rightPressed || leftPressed));
		if (commandInputIndex > 0)
		{
			commandBuffer.clear();
		}
		inputIndex += commandInputIndex;
		loseMomentum = loseMomentum && (inputIndex == 0);
		velocityX = velocityX * (inputIndex != 6) + ((!lookingLeft - lookingLeft) * 8) * (inputIndex == 6);
		bDashing = (inputIndex == 6);
		//cout << "Input Index: " << inputIndex << endl;
		auto it = animMap.find(inputIndex);
		if (it != animMap.end() && state != animToStateMap[it->second] && state != STATE_ATTACKING)
		{
			sprite->changeAnimation(it->second);
		}
		gainMomentum = anim == RUN;
		prevRightPressed = Game::instance().getKey(GLFW_KEY_RIGHT);
		prevLeftPressed = Game::instance().getKey(GLFW_KEY_LEFT) * !rightPressed;
		prevDownPressed = Game::instance().getKey(GLFW_KEY_DOWN);
		
		position.y += FALL_SPEED;
		grounded = tileMap->collisionMoveDown(getTerrainCollisionBox(), &position.y, quadSize.y);
		if (Game::instance().getKey(GLFW_KEY_Z) && !Game::instance().getKey(GLFW_KEY_DOWN) && grounded)
		{
			bJumping = true;
			grounded = false;
			jumpAngle = 0;
			startY = position.y;
		}
	}
	else
	{
		calcIncrement(velocityX, 0.f, 0.075f);
		bDashing = velocityX != 0;
		if (bDashing && anim == DASH1_FINAL && Game::instance().getKey(GLFW_KEY_Z))
		{
			sprite->changeAnimation(DASH1);
			velocityX = ((!lookingLeft - lookingLeft) * 8.f);
		}
		else if (anim != DASH1_GETUP && abs(velocityX) <= 1.f)
		{
			sprite->changeAnimation(DASH1_GETUP);
		}
		Game::instance().keyReleased(GLFW_KEY_Z);
		position.y += FALL_SPEED;
		grounded = tileMap->collisionMoveDown(getTerrainCollisionBox(), &position.y, quadSize.y);
		if (!grounded)
		{
			sprite->changeAnimation(FALL);
			bDashing = false;
			dashOffLedge = true;
		}
	}
	Game::instance().keyReleased(GLFW_KEY_X);
	position.x += velocityX;
	//cout << jumpAngle << endl;
	setPosition(position);
	sprite->update(deltaTime);
}

void Player::registerInput(int key)
{
	auto now = std::chrono::steady_clock::now();
	InputEvent event = { key, now };
	commandBuffer.push_back(event);

	const auto expirationTime = std::chrono::milliseconds(500);
	// Remove expired events
	commandBuffer.erase(
		std::remove_if(commandBuffer.begin(), commandBuffer.end(),
			[now, expirationTime](const InputEvent& event) {
				return now - event.timestamp > expirationTime;
			}),
		commandBuffer.end());
	//cout << commandBuffer.size() << endl;
	
	//if (commandBuffer.size()>1) cout << commandBuffer[commandBuffer.size()-2].key << ' ' << commandBuffer[commandBuffer.size() - 1].key << endl;
	
}

bool Player::checkCommand(const vector<int>& command, const std::chrono::milliseconds& timeWindow) const
{
	if (command.size() > commandBuffer.size()) return false;
	auto now = std::chrono::steady_clock::now();
	size_t commandIndex = command.size() - 1;
	for (size_t i = commandBuffer.size(); i-- > 0; ) {
		if (commandBuffer[i].key == command[commandIndex]) {
			if (now - commandBuffer[i].timestamp > timeWindow) {
				return false; // Command took too long
			}
			if (commandIndex == 0) {
				return true; // Full command matched
			}
			--commandIndex;
		}
	}
	return false;
}

const Hitbox Player::getTerrainCollisionBox() const
{
	Hitbox box;
	box.min = position + glm::vec2(22, 21);
	box.max = position + glm::vec2(43, 64);
	return box;
}

void Player::calcIncrement(float& valToInc, float targetVal, float factor)
{
	if (abs(valToInc - targetVal) > 0.2f) valToInc = valToInc + (targetVal - valToInc) * factor;
	else valToInc = targetVal;
}
