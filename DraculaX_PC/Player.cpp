#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Player.h"
#include "Game.h"
#include <map>

#define FALL_SPEED 4
#define DASH_KICK_HEIGHT 32

enum PlayerStates
{
	STATE_IDLE, STATE_WALKING, STATE_JUMPING, STATE_FALLING, STATE_CROUCHING, STATE_PREP_ATK, STATE_ATTACKING, STATE_DASHING, STATE_DASHKICKING, STATE_COMBO_DASHING, STATE_ULTING, STATE_CLIMBING
};

enum PlayerAnims
{
	IDLE, WALK, RUN, JUMP, JUMP_FW, JUMP_FINAL, FALL, BACKFLIP_FINAL, FALL_FINAL, CROUCH, CROUCH_FINAL, GETUP, PREP_ATK, ATTACK, ATTACK_SUBWEAPON, ATTACK_CROUCH, SKID, BACKFLIP_SKID, 
	DASH1, DASH1_FINAL, DASH1_GETUP, DASH_KICK, DASH_KICK_FINAL, DASH_COMBO, DASH_COMBO_FINAL, UPPERCUT, BACKFLIP, ULT, ULT_FINAL, CLIMB_IDLE_UP, CLIMB_IDLE_DOWN, UPSTAIRS, DOWNSTAIRS
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
	{GLFW_KEY_RIGHT, GLFW_KEY_RIGHT},
	std::chrono::milliseconds(256),
	32
};

const Command LEFT_RUN_COMMAND = {
	{GLFW_KEY_LEFT, GLFW_KEY_LEFT},
	std::chrono::milliseconds(256),
	32
};

const Command DASH_COMMAND = {
	{GLFW_KEY_UP, GLFW_KEY_DOWN},
	std::chrono::milliseconds(512),
	64
};

const Command UPPERCUT_COMMAND = {
	{GLFW_KEY_DOWN, GLFW_KEY_UP},
	std::chrono::milliseconds(128),
	128
};

const vector<glm::vec2> crouchWhipOffset = {
	glm::vec2(0,16), glm::vec2(1,14), glm::vec2(1,13), glm::vec2(0,11), glm::vec2(0,11), glm::vec2(0,16), glm::vec2(-1,16),glm::vec2(-1,16),glm::vec2(-1,16),glm::vec2(-1,16),glm::vec2(-1,16),glm::vec2(-1,16),
};

const vector<glm::vec2> leftCrouchWhipOffset = {
	glm::vec2(0,16), glm::vec2(-1,14), glm::vec2(-1,13), glm::vec2(0,11), glm::vec2(0,11), glm::vec2(0,16), glm::vec2(1,16), glm::vec2(1,16), glm::vec2(1,16), glm::vec2(1,16), glm::vec2(1,16), glm::vec2(1,16),
};

const vector<glm::vec2> stairsOffset = {
	glm::vec2(2,-3), glm::vec2(2,-1), glm::vec2(2,-2), glm::vec2(2,-2), glm::vec2(2,-2), glm::vec2(2,-2), glm::vec2(2,-2), glm::vec2(2,-2), //para arriba
	glm::vec2(2,2), glm::vec2(2,2), glm::vec2(2,2), glm::vec2(2,2), glm::vec2(2,2), glm::vec2(0,0), glm::vec2(2,2), glm::vec2(2,2), glm::vec2(2,2), glm::vec2(2,2) //para abajo
};

std::map<int, int> animMap = {
	{ 0, PlayerAnims::IDLE },
	{ inputMap[RIGHT], PlayerAnims::WALK},
	{ inputMap[DOWN], PlayerAnims::CROUCH},
	{ inputMap[DOWN] | inputMap[A], PlayerAnims::DASH1},
	{ inputMap[X], PlayerAnims::ATTACK},
	{ inputMap[X] | inputMap[RIGHT], PlayerAnims::ATTACK},
	{ inputMap[X] | inputMap[A], PlayerAnims::ATTACK},
	{ inputMap[X] | inputMap[DOWN], PlayerAnims::ATTACK_CROUCH},
	{ inputMap[X] | inputMap[DOWN] | inputMap[RIGHT], PlayerAnims::ATTACK_CROUCH},
	{ inputMap[UP], PlayerAnims::PREP_ATK},
	{ inputMap[UP] | inputMap[RIGHT], PlayerAnims::WALK},
	{ inputMap[UP] | inputMap[DOWN], PlayerAnims::CROUCH},
	{ inputMap[UP] | inputMap[X], PlayerAnims::ATTACK_SUBWEAPON},
	{ inputMap[UP] | inputMap[X] | inputMap[RIGHT], PlayerAnims::ATTACK_SUBWEAPON},
	{ RIGHT_RUN_COMMAND.index, PlayerAnims::RUN },
	{ DASH_COMMAND.index, PlayerAnims::DASH_COMBO },
	{ UPPERCUT_COMMAND.index, PlayerAnims::UPPERCUT },
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
	{ PlayerAnims::DASH_KICK, PlayerStates::STATE_DASHKICKING },
	{ PlayerAnims::DASH_KICK_FINAL, PlayerStates::STATE_DASHKICKING },
	{ PlayerAnims::DASH_COMBO, PlayerStates::STATE_COMBO_DASHING },
	{ PlayerAnims::DASH_COMBO_FINAL, PlayerStates::STATE_COMBO_DASHING },
	{ PlayerAnims::UPPERCUT, PlayerStates::STATE_JUMPING },
	{ PlayerAnims::BACKFLIP, PlayerStates::STATE_JUMPING },
	{ PlayerAnims::BACKFLIP_FINAL, PlayerStates::STATE_JUMPING },
	{ PlayerAnims::BACKFLIP_SKID, PlayerStates::STATE_IDLE },
	{ PlayerAnims::ULT, PlayerStates::STATE_ULTING },
	{ PlayerAnims::ULT_FINAL, PlayerStates::STATE_ULTING },
	{ PlayerAnims::CLIMB_IDLE_UP, PlayerStates::STATE_CLIMBING },
	{ PlayerAnims::CLIMB_IDLE_DOWN, PlayerStates::STATE_CLIMBING },
	{ PlayerAnims:: UPSTAIRS, PlayerStates::STATE_CLIMBING },
	{ PlayerAnims::DOWNSTAIRS, PlayerStates::STATE_CLIMBING }
};

void Player::render()
{
	shader->use();
	shader->setUniform1i("flip", lookingLeft);
	int anim = sprite->animation();
	if (gainMomentum || (loseMomentum && (anim == SKID || anim == BACKFLIP_SKID)) || sprite->animation() == UPPERCUT || bDashing || backflipping) afterimages.render();
	sprite->render();
	if (whipping)
	{
		whip->render();
	}
}

void Player::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	Entity::init(tileMapPos, shaderProgram);
	afterimages.sprite = sprite;
	whipTex.loadFromFile("images/Richter/whip.png", TEXTURE_PIXEL_FORMAT_RGBA);
	whipTex.setMagFilter(GL_NEAREST);
	whip = Sprite::createSprite(glm::ivec2(128, 64), glm::vec2(0.1f, 1.f), &whipTex, &shaderProgram);
	whip->setNumberAnimations(2);
	whip->setAnimationSpeed(0, 32);
	whip->animatorX(0, 6, 0.f, 0.1f, 0.f);
	whip->addKeyframe(0, glm::vec2(0.6f, 0.f));
	whip->addKeyframe(0, glm::vec2(0.7f, 0.f));
	whip->addKeyframe(0, glm::vec2(0.8f, 0.f));
	whip->addKeyframe(0, glm::vec2(0.6f, 0.f));
	whip->addKeyframe(0, glm::vec2(0.7f, 0.f));
	whip->addKeyframe(0, glm::vec2(0.8f, 0.f));
	whip->setAnimationSpeed(1, 0);
	whip->addKeyframe(1, glm::vec2(0.9f, 0.f));
	whip->setTransition(0, 1);
	whip->changeAnimation(0);
}

string Player::getName() const
{
	return "Player";
}

const string Player::getSpritesheet() const
{
	return "images/Richter/Richter_v2.png";
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
	int attackSpeed = 32;

	sprite->setNumberAnimations(33);

	sprite->setAnimationSpeed(IDLE, 8);
	sprite->animatorX(IDLE, 4, 0.f, 0.1f, 0.f);

	sprite->setAnimationSpeed(WALK, 10);
	sprite->animatorX(WALK, 8, 0.f, 0.1f, 0.3f);

	sprite->setAnimationSpeed(RUN, 16);
	sprite->animatorX(RUN, 8, 0.f, 0.1f, 0.4f);

	sprite->setAnimationSpeed(JUMP, 16);
	sprite->addKeyframe(JUMP, glm::vec2(0.6f, 0.f));
	sprite->addKeyframe(JUMP, glm::vec2(0.6f, 0.f));
	sprite->addKeyframe(JUMP, glm::vec2(0.6f, 0.2f));

	sprite->setAnimationSpeed(JUMP_FW, 16);
	sprite->addKeyframe(JUMP_FW, glm::vec2(0.7f, 0.f));
	sprite->addKeyframe(JUMP_FW, glm::vec2(0.7f, 0.f));
	sprite->addKeyframe(JUMP_FW, glm::vec2(0.6f, 0.2f));

	sprite->setAnimationSpeed(JUMP_FINAL, 0);
	sprite->addKeyframe(JUMP_FINAL, glm::vec2(0.7f, 0.2f));

	sprite->setAnimationSpeed(FALL, 16);
	sprite->addKeyframe(FALL, glm::vec2(0.6f, 0.2f));
	sprite->addKeyframe(FALL, glm::vec2(0.8f, 0.f));

	sprite->setAnimationSpeed(FALL_FINAL, 0);
	sprite->addKeyframe(FALL_FINAL, glm::vec2(0.9f, 0.f));

	sprite->setAnimationSpeed(CROUCH, crouchSpeed);
	sprite->animatorX(CROUCH, 3, 0.6f, 0.1f, 0.2f);

	sprite->setAnimationSpeed(CROUCH_FINAL, 0);
	sprite->addKeyframe(CROUCH_FINAL, glm::vec2(0.8f, 0.2f));

	sprite->setAnimationSpeed(GETUP, crouchSpeed);
	sprite->animatorX(GETUP, 3, 0.8f, -0.1f, 0.2f);

	sprite->setAnimationSpeed(PREP_ATK, 0);
	sprite->addKeyframe(PREP_ATK, glm::vec2(0.f, 0.1f));

	sprite->setAnimationSpeed(ATTACK, attackSpeed);
	sprite->animatorX(ATTACK, 6, 0.f, 0.1f, 0.1f);
	sprite->addKeyframe(ATTACK, glm::vec2(0.6f, 0.1));
	sprite->addKeyframe(ATTACK, glm::vec2(0.6f, 0.1));
	sprite->addKeyframe(ATTACK, glm::vec2(0.6f, 0.1));
	sprite->addKeyframe(ATTACK, glm::vec2(0.6f, 0.1));
	sprite->addKeyframe(ATTACK, glm::vec2(0.6f, 0.1));
	sprite->addKeyframe(ATTACK, glm::vec2(0.6f, 0.1));

	sprite->setAnimationSpeed(ATTACK_CROUCH, attackSpeed);
	sprite->animatorX(ATTACK_CROUCH, 5, 0.f, 0.1f, 0.2f);
	sprite->addKeyframe(ATTACK_CROUCH, glm::vec2(0.5f, 0.2f));
	sprite->addKeyframe(ATTACK_CROUCH, glm::vec2(0.5f, 0.2f));
	sprite->addKeyframe(ATTACK_CROUCH, glm::vec2(0.5f, 0.2f));
	sprite->addKeyframe(ATTACK_CROUCH, glm::vec2(0.5f, 0.2f));
	sprite->addKeyframe(ATTACK_CROUCH, glm::vec2(0.5f, 0.2f));
	sprite->addKeyframe(ATTACK_CROUCH, glm::vec2(0.5f, 0.2f));
	sprite->addKeyframe(ATTACK_CROUCH, glm::vec2(0.5f, 0.2f));

	sprite->setAnimationSpeed(ATTACK_SUBWEAPON, 20);
	sprite->animatorX(ATTACK_SUBWEAPON, 5, 0.f, 0.1f, 0.1f);
	sprite->addKeyframe(ATTACK_SUBWEAPON, glm::vec2(0.7f, 0.1f));
	sprite->addKeyframe(ATTACK_SUBWEAPON, glm::vec2(0.7f, 0.1f));
	sprite->addKeyframe(ATTACK_SUBWEAPON, glm::vec2(0.7f, 0.1f));
	sprite->addKeyframe(ATTACK_SUBWEAPON, glm::vec2(0.7f, 0.1f));

	sprite->setAnimationSpeed(SKID, 32);
	sprite->addKeyframe(SKID, glm::vec2(0.8f, 0.4f));
	sprite->addKeyframe(SKID, glm::vec2(0.9f, 0.4f));
	sprite->addKeyframe(SKID, glm::vec2(0.9f, 0.4f));
	sprite->addKeyframe(SKID, glm::vec2(0.0f, 0.5f));
	sprite->addKeyframe(SKID, glm::vec2(0.0f, 0.5f));
	sprite->addKeyframe(SKID, glm::vec2(0.9f, 0.4f));
	sprite->addKeyframe(SKID, glm::vec2(0.9f, 0.4f));
	sprite->addKeyframe(SKID, glm::vec2(0.0f, 0.5f));
	sprite->addKeyframe(SKID, glm::vec2(0.0f, 0.5f));

	sprite->setAnimationSpeed(DASH1, 24);
	sprite->addKeyframe(DASH1, glm::vec2(0.9f, 0.f));
	sprite->animatorX(DASH1, 4, 0.f, 0.1f, 0.5f);

	sprite->setAnimationSpeed(DASH1_FINAL, 0);
	sprite->addKeyframe(DASH1_FINAL, glm::vec2(0.4f, 0.5f));

	sprite->setAnimationSpeed(DASH1_GETUP, 12);
	sprite->animatorX(DASH1_GETUP, 2, 0.3f, -0.1f, 0.5f);
	sprite->addKeyframe(DASH1_GETUP, glm::vec2(0.8f, 0.2f));
	sprite->addKeyframe(DASH1_GETUP, glm::vec2(0.8f, 0.2f));
	sprite->addKeyframe(DASH1_GETUP, glm::vec2(0.8f, 0.2f));

	sprite->setAnimationSpeed(DASH_KICK, 16);
	sprite->addKeyframe(DASH_KICK, glm::vec2(0.5f, 0.5f));
	sprite->addKeyframe(DASH_KICK, glm::vec2(0.5f, 0.5f));
	sprite->addKeyframe(DASH_KICK, glm::vec2(0.5f, 0.5f));
	sprite->addKeyframe(DASH_KICK, glm::vec2(0.5f, 0.5f));
	sprite->addKeyframe(DASH_KICK, glm::vec2(0.6f, 0.5f));
	sprite->addKeyframe(DASH_KICK, glm::vec2(0.6f, 0.5f));
	sprite->addKeyframe(DASH_KICK, glm::vec2(0.6f, 0.5f));
	sprite->addKeyframe(DASH_KICK, glm::vec2(0.6f, 0.6f));
	sprite->addKeyframe(DASH_KICK, glm::vec2(0.6f, 0.6f));
	sprite->addKeyframe(DASH_KICK, glm::vec2(0.7f, 0.5f));
	sprite->addKeyframe(DASH_KICK, glm::vec2(0.8f, 0.5f));
	sprite->addKeyframe(DASH_KICK, glm::vec2(0.9f, 0.f));
	sprite->addKeyframe(DASH_KICK, glm::vec2(0.8f, 0.6f));

	sprite->setAnimationSpeed(DASH_KICK_FINAL, 0);
	sprite->addKeyframe(DASH_KICK_FINAL, glm::vec2(0.8f, 0.6f));

	sprite->setAnimationSpeed(DASH_COMBO, 16);
	sprite->addKeyframe(DASH_COMBO, glm::vec2(0.8f, 0.3f));
	sprite->addKeyframe(DASH_COMBO, glm::vec2(0.9f, 0.3f));
	sprite->animatorX(DASH_COMBO, 5, 0.9f, 0.f, 0.3f);

	sprite->setAnimationSpeed(DASH_COMBO_FINAL, 0);
	sprite->addKeyframe(DASH_COMBO_FINAL, glm::vec2(0.9f, 0.3f));

	sprite->setAnimationSpeed(UPPERCUT, 0);
	sprite->addKeyframe(UPPERCUT, glm::vec2(0.9f, 0.6f));

	sprite->setAnimationSpeed(BACKFLIP, 16);
	sprite->animatorX(BACKFLIP, 7, 0.f, 0.1f, 0.6f);

	sprite->setAnimationSpeed(BACKFLIP_FINAL, 0);
	sprite->addKeyframe(BACKFLIP_FINAL, glm::vec2(0.7f, 0.6f));

	sprite->setAnimationSpeed(BACKFLIP_SKID, 2);
	sprite->addKeyframe(BACKFLIP_SKID, glm::vec2(0.8f, 0.6f));

	sprite->setAnimationSpeed(ULT, 16);
	sprite->addKeyframe(ULT, glm::vec2(0.6f, 0.f));
	sprite->addKeyframe(ULT, glm::vec2(0.6f, 0.f));
	sprite->animatorX(ULT, 3, 0.7f, 0.1f, 0.5f);

	sprite->setAnimationSpeed(ULT_FINAL, 30);
	sprite->addKeyframe(ULT_FINAL, glm::vec2(0.8f, 0.9f));
	sprite->addKeyframe(ULT_FINAL, glm::vec2(0.9f, 0.9f));

	sprite->setAnimationSpeed(CLIMB_IDLE_UP, 0);
	sprite->addKeyframe(CLIMB_IDLE_UP, glm::vec2(0.f, 0.7f));

	sprite->setAnimationSpeed(CLIMB_IDLE_DOWN, 0);
	sprite->addKeyframe(CLIMB_IDLE_DOWN, glm::vec2(0.9f, 0.7f));

	sprite->setAnimationSpeed(UPSTAIRS, 20);
	sprite->animatorX(UPSTAIRS, 8, 0.1f, 0.1f, 0.7f);

	sprite->setAnimationSpeed(DOWNSTAIRS, 20);
	sprite->animatorX(DOWNSTAIRS, 10, 0.f, 0.1f, 0.8f);

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
	sprite->setTransition(DASH_KICK, DASH_KICK_FINAL);
	sprite->setTransition(DASH_COMBO, DASH_COMBO_FINAL);
	sprite->setTransition(BACKFLIP, BACKFLIP_FINAL);
	sprite->setTransition(BACKFLIP_SKID, IDLE);
	sprite->setTransition(ULT, ULT_FINAL);

	sprite->changeAnimation(IDLE);
}

void Player::setHitboxes()
{
}

void Player::childUpdate(int deltaTime)
{
	sprite->update(deltaTime);
	//cout << Game::instance().getKey(GLFW_KEY_Z) << endl;
	int anim = sprite->animation();
	//cout << "Anim: " << anim << " State: " << animToStateMap[anim] << endl;
	int state = animToStateMap[anim];

	if (Game::instance().getKey(GLFW_KEY_TAB) && state != STATE_ATTACKING && !bDashing && !bUlting)
	{
		sprite->changeAnimation(ULT);
		bJumping = false;
		bUlting = true;
		backflipping = false;
		jumpAngle = 0;
		startY = position.y;
		Game::instance().keyReleased(GLFW_KEY_TAB);
	}
	else if (!bUlting && !bClimbing)
	{
		bool rightPressed = Game::instance().getKey(GLFW_KEY_RIGHT) * !bDashing;
		bool leftPressed = Game::instance().getKey(GLFW_KEY_LEFT) * !rightPressed * !bDashing;
		lookingLeft = (leftPressed || (lookingLeft && !rightPressed)) * (state != STATE_ATTACKING) + lookingLeftAtk * (state == STATE_ATTACKING);

		if (loseMomentum)
		{
			if (abs(velocityX) == (1 + backflipping) && anim != (SKID + backflipping))
			{
				sprite->changeAnimation(SKID+backflipping);
				anim = SKID+backflipping;
				state = STATE_IDLE;
				backflipping = false;
			}
			velocityX *= 0.9f;
			if (abs(velocityX) < 0.1f)
			{
				velocityX = 0.f;
				loseMomentum = false;
			}
		}
	
		bool getup = (prevDownPressed && grounded && !bDashing && !Game::instance().getKey(GLFW_KEY_DOWN) && anim != GETUP && (state != STATE_DASHING && state != STATE_DASHKICKING))
			|| ((state == STATE_DASHING || state == STATE_DASHKICKING) && !bDashing && !Game::instance().getKey(GLFW_KEY_DOWN));

		if (bJumping)
		{
			velocityX = (rightPressed - leftPressed) * (1.f + 1.f * gainMomentum);
			if (Game::instance().getKey(GLFW_KEY_X) && state != STATE_ATTACKING)
			{
				Hitbox cb = getTerrainCollisionBox();
				if (checkCommand(DASH_COMMAND.sequence, DASH_COMMAND.timeWindow) && ((rightPressed && !tileMap->tileRight(cb)) || leftPressed && !tileMap->tileLeft(cb)))
				{
					sprite->changeAnimation(DASH_COMBO);
					velocityX = ((!lookingLeft - lookingLeft) * 8.f);
					bDashing = true;
					bJumping = false;
					commandBuffer.clear();
				}
				else
				{
					sprite->changeAnimation(ATTACK + Game::instance().getKey(GLFW_KEY_UP));
					whip->changeAnimation(0);
					lookingLeftAtk = lookingLeft;
				}
			}
			else
			{
				int inputToRegister =
					+GLFW_KEY_UP * Game::instance().getKey(GLFW_KEY_UP)
					+ GLFW_KEY_DOWN * Game::instance().getKey(GLFW_KEY_DOWN);
				if (inputToRegister != 0) registerInput(inputToRegister);

				jumpAngle += JUMP_ANGLE_STEP;
				if (jumpAngle < 45 && !backflipping && Game::instance().getKey(GLFW_KEY_Z))
				{
					backflipping = true;
					sprite->changeAnimation(BACKFLIP);
				}
				else if (backflipping)
				{
					velocityX = (lookingLeft - !lookingLeft) * 2.f;
				}
				int tile = 0;
				if (jumpAngle >= 180)
				{
					bJumping = false;
					loseMomentum = true;
					position.y = startY;
					JUMP_HEIGHT = 64;
					jumpAngle = 0;
				}
				else if (Game::instance().getKey(GLFW_KEY_UP) && jumpAngle >= 90
					&& ((tile = stairs->collisionMoveDownWithTileNum(getStairsCollisionBox(), &position.y, quadSize.y)) != -1) 
					&& ((tile % 8 == 1) || (tile % 8 == 2)))
				{
					JUMP_HEIGHT = 64;
					JUMP_ANGLE_STEP = 4;
					bJumping = false;
					backflipping = false;
					bClimbing = true;
					linedUpStair = true;
					position.y += 8;
					sprite->changeAnimation(CLIMB_IDLE_UP);
					if (tile == 1)
					{
						stairs->collisionMoveRight(getStairsCollisionBox(), &position.x);
						rightUpStair = true;
						lookingLeft = false;
						position.x += 5;
					}
					else
					{
						stairs->collisionMoveLeft(getStairsCollisionBox(), &position.x);
						rightUpStair = false;
						lookingLeft = true;
						position.x -= 5;
					}
					goDown = (lookingLeft && rightUpStair) || (!lookingLeft && !rightUpStair);
					//stairPosX = (int)position.x;
					//stairPosY = (int)position.y;
				}
				else
				{
					position.y = startY - JUMP_HEIGHT * sin(glm::radians((float)jumpAngle));
					bJumping = jumpAngle < 90 || (!tileMap->collisionMoveDown(getTerrainCollisionBox(), &position.y, quadSize.y) 
						&& !platforms->collisionMoveDown(getTerrainCollisionBox(), &position.y, quadSize.y));
					JUMP_HEIGHT = JUMP_HEIGHT * bJumping + 64 * (!bJumping);
					JUMP_ANGLE_STEP = 2 + 2 * (jumpAngle < 90 || JUMP_HEIGHT == 64);
					if (state != STATE_FALLING && jumpAngle >= 72 && state != STATE_ATTACKING && !backflipping) sprite->changeAnimation(FALL);
					else if (!bJumping && backflipping)
					{
						loseMomentum = true;
					}
				}
			}
		}
		else if (getup)
		{
			if (anim == ATTACK_CROUCH && !Game::instance().getKey(GLFW_KEY_DOWN))
			{
				int keyframe = sprite->getCurrentKeyframe();
				sprite->changeAnimation(ATTACK);
				sprite->setKeyframe(keyframe);
			}
			else sprite->changeAnimation(GETUP);
		}
		else if (!bDashing && !backflipping)
		{
			position.y += FALL_SPEED;
			grounded = tileMap->collisionMoveDown(getTerrainCollisionBox(), &position.y, quadSize.y) 
				|| platforms->collisionMoveDown(getTerrainCollisionBox(), &position.y, quadSize.y);
			if (Game::instance().getKey(GLFW_KEY_Z) && !Game::instance().getKey(GLFW_KEY_DOWN) && grounded && state != STATE_ATTACKING)
			{
				if (checkCommand(UPPERCUT_COMMAND.sequence, UPPERCUT_COMMAND.timeWindow))
				{
					JUMP_HEIGHT = 128;
					sprite->changeAnimation(UPPERCUT);
				}
				else sprite->changeAnimation(JUMP + (rightPressed || leftPressed));
				bJumping = true;
				grounded = false;
				backflipping = false;
				jumpAngle = 0;
				startY = position.y;
				Game::instance().keyReleased(GLFW_KEY_Z);
			}
			else
			{
				timeRunning = (timeRunning + (deltaTime / 1000.f)) * (anim == RUN);
				if (dashOffLedge)
				{
					calcIncrement(velocityX, 0.f, 0.075f);
					dashOffLedge = velocityX != 0;
				}
				else if (!loseMomentum)
				{
					loseMomentum = (timeRunning >= .5f) && gainMomentum && !(rightPressed || leftPressed);
					gainMomentum = gainMomentum && (rightPressed || leftPressed);
					velocityX = (((rightPressed - leftPressed) + ((!lookingLeft - lookingLeft) * loseMomentum)) * (1.f + 1.f * gainMomentum)) * (state != STATE_ATTACKING && state != STATE_CROUCHING);
				}
				// Calculate animation based on input
				int inputIndex = 0;
				inputIndex += inputMap[RIGHT] * rightPressed
					+ inputMap[LEFT] * leftPressed
					+ inputMap[UP] * (Game::instance().getKey(GLFW_KEY_UP) * (anim != GETUP))
					+ inputMap[DOWN] * Game::instance().getKey(GLFW_KEY_DOWN)
					+ inputMap[A] * Game::instance().getKey(GLFW_KEY_Z)
					+ inputMap[X] * Game::instance().getKey(GLFW_KEY_X);
				//Register inputs for command detection
				int inputToRegister = GLFW_KEY_RIGHT * (!prevRightPressed && rightPressed)
					+ GLFW_KEY_LEFT * (!prevLeftPressed && leftPressed)
					+ GLFW_KEY_UP * Game::instance().getKey(GLFW_KEY_UP)
					+ GLFW_KEY_DOWN * Game::instance().getKey(GLFW_KEY_DOWN);
				if (inputToRegister != 0) registerInput(inputToRegister);
				int tile = 0;
				int distance = 0;
				if (rightPressed || leftPressed)
				{
					int commandInputIndex = RIGHT_RUN_COMMAND.index * checkCommand(RIGHT_RUN_COMMAND.sequence, RIGHT_RUN_COMMAND.timeWindow)
						+ LEFT_RUN_COMMAND.index * checkCommand(LEFT_RUN_COMMAND.sequence, LEFT_RUN_COMMAND.timeWindow)
						+ RIGHT_RUN_COMMAND.index * gainMomentum;

					if (Game::instance().getKey(GLFW_KEY_X))
					{
						commandInputIndex = DASH_COMMAND.index * (checkCommand(DASH_COMMAND.sequence, DASH_COMMAND.timeWindow) && (rightPressed || leftPressed));
					}
					if (commandInputIndex > 0)
					{
						commandBuffer.clear();
						inputIndex = commandInputIndex;
					}
				}
				else if (inputIndex == inputMap[UP] && grounded && ((tile = stairs->distanceFromStairTile(getStairsDetectionCollisionBox(), distance)) != -1))
				{
					bClimbing = true; 
					linedUpStair = false;
					goDown = false;
					rightUpStair = tile == 1;
					stairPosX = (int)position.x - distance;
					stairPosY = (int)position.y;
				}
				else if (inputIndex == inputMap[DOWN] && grounded && ((tile = stairs->distanceFromBelowStairTile(getBelowStairsDetectionCollisionBox(), distance)) != -1))
				{
					bClimbing = true;
					linedUpStair = false;
					goDown = true;
					rightUpStair = tile == 1;
					stairPosX = (int)position.x - distance;
					stairPosY = (int)position.y + 8;
				}
				loseMomentum = loseMomentum && (inputIndex == 0);
				// Low dash or dash combo
				Hitbox cb = getTerrainCollisionBox();
				if (state != STATE_ATTACKING && ((inputIndex == 6 && state != STATE_DASHING) || inputIndex == DASH_COMMAND.index) && ((!lookingLeft && !tileMap->tileRight(cb)) || (lookingLeft && !tileMap->tileLeft(cb))))
				{
					bDashing = true;
					velocityX = (!lookingLeft - lookingLeft) * 8.f;
					Game::instance().keyReleased(GLFW_KEY_Z);
				}
				//cout << "Input Index: " << inputIndex << endl;
				auto it = animMap.find(inputIndex);
				if (it != animMap.end() && state != animToStateMap[it->second] && state != STATE_ATTACKING && grounded)
				{
					sprite->changeAnimation(it->second);
					if (sprite->animation() == (ATTACK + bCrouching * 2)) whip->changeAnimation(0);
					lookingLeftAtk = lookingLeft;
				}
				else if (!grounded && state != STATE_FALLING)
				{
					sprite->changeAnimation(FALL + backflipping);
				}
				else if (anim == ATTACK && Game::instance().getKey(GLFW_KEY_DOWN))
				{
					int keyframe = sprite->getCurrentKeyframe();
					sprite->changeAnimation(ATTACK_CROUCH);
					sprite->setKeyframe(keyframe);
				}
				gainMomentum = anim == RUN;
				int crouchanim = sprite->animation();
				bCrouching = (crouchanim == CROUCH) || (crouchanim == CROUCH_FINAL) || (crouchanim == ATTACK_CROUCH);
			}
		}
		else
		{
			//cout << anim << endl;
			if (state == STATE_DASHING)
			{
				calcIncrement(velocityX, 0.f, 0.075f);
				bDashing = velocityX != 0;
				if (bDashing && anim == DASH1 && Game::instance().getKey(GLFW_KEY_Z))
				{
					sprite->changeAnimation(DASH_KICK);
					velocityX = ((!lookingLeft - lookingLeft) * 10.f);
					jumpAngle = 0;
					startY = position.y;
				}
				else if (bDashing && anim == DASH1_FINAL && Game::instance().getKey(GLFW_KEY_Z))
				{
					sprite->changeAnimation(DASH1);
					velocityX = ((!lookingLeft - lookingLeft) * 8.f);
				}
				else if (anim != DASH1_GETUP && abs(velocityX) <= 1.f)
				{
					sprite->changeAnimation(DASH1_GETUP);
				}
			}
			else if (state == STATE_DASHKICKING)
			{
				calcIncrement(velocityX, 0.f, 0.05f);
				jumpAngle += 3;
				if (jumpAngle >= 180)
				{
					//cout << jumpAngle << endl;
					jumpAngle = 0;
					bDashing = false;
					sprite->changeAnimation(DASH_KICK_FINAL);
				}
				else if (anim != DASH_KICK_FINAL)
				{
					position.y = startY - DASH_KICK_HEIGHT * sin(glm::radians((float)jumpAngle));
					Hitbox cb = getTerrainCollisionBox();
					tileMap->collisionMoveDown(cb, &position.y, quadSize.y);
					platforms->collisionMoveDown(cb, &position.y, quadSize.y);
					//cout << jumpAngle << endl;
				}
				bDashing = velocityX != 0 || (anim != DASH_KICK_FINAL);
				if (!bDashing)
				{
					sprite->changeAnimation(CROUCH_FINAL);
				}
			}
			else if (state == STATE_COMBO_DASHING)
			{
				calcIncrement(velocityX, 0.f, 0.05f);
				if (abs(velocityX) <= 1.f)
				{
					velocityX = (!lookingLeft - lookingLeft) * 1.f;
					bDashing = false;
					loseMomentum = true;
				}
			}
			Game::instance().keyReleased(GLFW_KEY_Z);
			position.y += FALL_SPEED * (state != STATE_COMBO_DASHING);
			grounded = tileMap->collisionMoveDown(getTerrainCollisionBox(), &position.y, quadSize.y)
				|| platforms->collisionMoveDown(getTerrainCollisionBox(), &position.y, quadSize.y);
			if (!grounded && !bDashing)
			{
				sprite->changeAnimation(FALL);
				bDashing = false;
				dashOffLedge = true;
			}
		}
		afterimages.updateAfterimage(deltaTime, glm::vec2(tileMapDispl) + position, anim, sprite->getCurrentKeyframe());
		Game::instance().keyReleased(GLFW_KEY_X);
		position.x += velocityX;
		Hitbox cb = getTerrainCollisionBox();
		if (velocityX > 0)
		{
			tileMap->collisionMoveRight(cb, &position.x);
		}
		else if (velocityX < 0)
		{
			tileMap->collisionMoveLeft(cb, &position.x);
		}
		ultTimeElapsed = 0;
		prevRightPressed = Game::instance().getKey(GLFW_KEY_RIGHT);
		prevLeftPressed = Game::instance().getKey(GLFW_KEY_LEFT) * !rightPressed;
		prevDownPressed = Game::instance().getKey(GLFW_KEY_DOWN);
		int newAnim = sprite->animation();
		whipping = ((newAnim == ATTACK) || (newAnim == ATTACK_CROUCH));	
		if (whipping)
		{
			whip->update(deltaTime);
			int whipKF = whip->getCurrentKeyframe();
			whip->setPosition(glm::vec2(position.x + crouchWhipOffset[whipKF].x*!lookingLeft+leftCrouchWhipOffset[whipKF].x*lookingLeft * bCrouching + tileMapDispl.x - 64 * lookingLeft, position.y + 1 + crouchWhipOffset[whipKF].y * bCrouching + tileMapDispl.y));
		}
		//cout << "Position :" << crouchWhipOffset[whip->getCurrentKeyframe()].x << ' ' << crouchWhipOffset[whip->getCurrentKeyframe()].y << endl;
		//cout << "Keyframe: " << whip->getCurrentKeyframe() << endl;
	}
	else if (bClimbing)
	{
		if (linedUpStair)
		{
			prevYpos = position.y;
			stairMovement();
			bool goingUp = (prevYpos - position.y) > 0;
			Hitbox cb = getStairsCollisionBox();
			if (goingUp) bClimbing = stairs->collisionMoveDown(cb);
			else bClimbing = !platforms->collisionMoveDown(cb, &position.y, quadSize.y) && !tileMap->collisionMoveDown(cb, &position.y, quadSize.y);
			bClimbing = bClimbing && !(Game::instance().getKey(GLFW_KEY_DOWN) && Game::instance().getKey(GLFW_KEY_Z));
			linedUpStair = bClimbing;
			Game::instance().keyReleased(GLFW_KEY_Z);
		}
		else if ((int)position.x != stairPosX)
		{
			if (anim != WALK) sprite->changeAnimation(WALK);
			bool goLeft = position.x > stairPosX;
			position.x += (!goLeft - goLeft);
		}
		else if ((int)position.y != stairPosY)
		{
			//lookingLeft = rightUpStair && goDown || (!rightUpStair && !goDown);
			position.y++;
		}
		else
		{
			sprite->changeAnimation(CLIMB_IDLE_UP+goDown);
			lookingLeft = rightUpStair && goDown || (!rightUpStair && !goDown);
			linedUpStair = true;
			prevDownPressed = false;
		}
	}
	else
	{
		if (startY - position.y < 64 && ultTimeElapsed == 0)
		{
			position.y -= 4;
			if (startY - position.y == 64)
			{
				startY = position.y;
				ultTimeElapsed++;
			}
		}
		else
		{
			ultTimeElapsed += deltaTime;
			bUlting = ultTimeElapsed < 1500;
			startY += (ultTimeElapsed > 1250);
		}
	}
	prevAnim = anim;
	setPosition(position);
}

void Player::stairMovement()
{
	int anim = sprite->animation();
	bool up = Game::instance().getKey(GLFW_KEY_UP);
	bool down = Game::instance().getKey(GLFW_KEY_DOWN);
	bool right = Game::instance().getKey(GLFW_KEY_RIGHT);
	bool left = Game::instance().getKey(GLFW_KEY_LEFT);
	int xDispl = 0;
	int yDispl = 0;
	if (rightUpStair)
	{
		bool keypressed = (up || down || right || left);
		xDispl += (!lookingLeft - lookingLeft) * keypressed;
		yDispl += (lookingLeft - !lookingLeft) * keypressed;
		lookingLeft = lookingLeft && !(up || right) || (down || left);
		int kf = sprite->getCurrentKeyframe();
		stepping = stepping && (anim == UPSTAIRS + (down || left)) && ((kf < 5 && !stepping2) || (stepping2 && (kf >= 5 || kf == 0 || keypressed)));
		stepping2 = stepping2 && kf != 1 && (!sprite->animationEnded() || kf == 0);
		//cout << "keyframe: " << sprite->getCurrentKeyframe() <<endl<< "keypressed: " << keypressed << endl;
		if (!stepping)
		{
			if (!keypressed && anim != (CLIMB_IDLE_UP + lookingLeft))
			{
				sprite->changeAnimation(CLIMB_IDLE_UP + lookingLeft);
				position.y -= (prevAnim == UPSTAIRS + (down || left)) * (prevKeyframe != 0);
				stepping2 = false;
			}
			else if (keypressed && anim != (UPSTAIRS + (down || left)))
			{
				sprite->changeAnimation(UPSTAIRS + (down || left));
				if (sprite->animation() == DOWNSTAIRS)
				{
					position.x += stairsOffset[kf + 8 * (down || left)].x * (!lookingLeft - lookingLeft) * (prevAnim == anim);
					position.y += stairsOffset[kf + 8 * (down || left)].y * (prevAnim == anim);
				}
				stepping = true;
			}
			else if (anim == (UPSTAIRS + (down || left)) && kf == 5)
			{
				stepping = true;
				stepping2 = true;
				position.x += stairsOffset[kf + 8 * (down || left)].x * (!lookingLeft - lookingLeft) * (prevAnim == anim);
				position.y += stairsOffset[kf + 8 * (down || left)].y * (prevAnim == anim);
			}
		}
		else
		{
			int currentKeyframe = sprite->getCurrentKeyframe();
			if (currentKeyframe != prevKeyframe)
			{
				int currentAnim = sprite->animation();
				position.x += stairsOffset[currentKeyframe + 8 * (down || left)].x * (!lookingLeft - lookingLeft) * (prevAnim == currentAnim);
				position.y += stairsOffset[currentKeyframe + 8 * (down || left)].y * (prevAnim == currentAnim);
			}
		}
		prevAnim = sprite->animation();
	}
	else
	{
		bool keypressed = (up || down || right || left);
		xDispl += (!lookingLeft - lookingLeft) * keypressed;
		yDispl -= (lookingLeft - !lookingLeft) * keypressed;
		lookingLeft = lookingLeft && !(down || right) || (up || left);
		stepping = stepping && keypressed && (prevKeyframe == sprite->getCurrentKeyframe());
		if (!stepping)
		{
			if (!keypressed && anim != (CLIMB_IDLE_UP + !lookingLeft)) sprite->changeAnimation(CLIMB_IDLE_UP + !lookingLeft);
			else if (keypressed)
			{
				if (anim != (UPSTAIRS + (down || right))) sprite->changeAnimation(UPSTAIRS + (down || right));
				int currentKeyframe = sprite->getCurrentKeyframe();
				int currentAnim = sprite->animation();
				position.x += stairsOffset[currentKeyframe + 8 * (down || right)].x * (!lookingLeft - lookingLeft) * (prevAnim == currentAnim);
				position.y += stairsOffset[currentKeyframe + 8 * (down || right)].y * (prevAnim == currentAnim);
				stepping = true;
			}
		}
		prevAnim = sprite->animation();
	}
	prevKeyframe = sprite->getCurrentKeyframe();
}

void Player::calcIncrement(float& valToInc, float targetVal, float factor)
{
	if (abs(valToInc - targetVal) > 0.2f) valToInc = valToInc + (targetVal - valToInc) * factor;
	else valToInc = targetVal;
}

void Player::registerInput(int key)
{
	auto now = std::chrono::steady_clock::now();
	InputEvent event = { key, now };
	commandBuffer.push_back(event);

	const auto expirationTime = std::chrono::milliseconds(1000);
	// Remove expired events
	commandBuffer.erase(
		std::remove_if(commandBuffer.begin(), commandBuffer.end(),
			[now, expirationTime](const InputEvent& event) {
				return now - event.timestamp > expirationTime;
			}),
		commandBuffer.end());
	//cout << commandBuffer.size() << endl;
	
	//if (commandBuffer.size()>2) cout << commandBuffer[commandBuffer.size() - 3].key << ' ' << commandBuffer[commandBuffer.size()-2].key << ' ' << commandBuffer[commandBuffer.size() - 1].key << endl;
	
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
	//cout << "Command not matched" << endl;
	return false;
}

bool Player::collision(const Hitbox& hitbox1, const Hitbox& hitbox2)
{
	return ((hitbox1.min.x < hitbox2.max.x) && (hitbox2.min.x < hitbox1.max.x) && (hitbox1.min.y < hitbox2.max.y) && (hitbox2.min.y < hitbox1.max.y));
}

const Hitbox Player::getTerrainCollisionBox() const
{
	Hitbox box;
	box.min = position + glm::vec2(22, 21);
	box.max = position + glm::vec2(42, 63);
	return box;
}

const Hitbox Player::getStairsCollisionBox() const
{
	Hitbox box;
	box.min = position + glm::vec2(22, 56);
	box.max = position + glm::vec2(42, 63);
	return box;
}

const Hitbox Player::getStairsDetectionCollisionBox() const
{
	Hitbox box;
	box.min = position + glm::vec2(31, 56);
	box.max = position + glm::vec2(38, 63);
	return box;
}

const Hitbox Player::getBelowStairsDetectionCollisionBox() const
{
	Hitbox box;
	box.min = position + glm::vec2(31, 64);
	box.max = position + glm::vec2(38, 79);
	return box;
}

