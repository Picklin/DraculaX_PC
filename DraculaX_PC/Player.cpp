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
	IDLE, WALK, JUMP, JUMP_FW, JUMP_FINAL, FALL, BACKFLIP_FINAL, FALL_FINAL, CROUCH, CROUCH_FINAL, GETUP, PREP_ATK, ATTACK, ATTACK_SUBWEAPON, ATTACK_CROUCH, ATTACK_UPSTAIRS, ATTACK_UPSTAIRS_SUBWEAPON, ATTACK_DOWNSTAIRS, ATTACK_DOWNSTAIRS_SUBWEAPON,
	SKID, BACKFLIP_SKID, DASH1, DASH1_FINAL, DASH1_GETUP, DASH_KICK, DASH_KICK_FINAL, DASH_COMBO, DASH_COMBO_FINAL, UPPERCUT, BACKFLIP, ULT, ULT_FINAL, CLIMB_IDLE_UP, CLIMB_IDLE_DOWN, UPSTAIRS, DOWNSTAIRS
};

struct Command
{
	vector<int> sequence;
	std::chrono::milliseconds timeWindow;
	int index;
};

const Command DASH_COMMAND = {
	{GLFW_KEY_UP, GLFW_KEY_DOWN},
	std::chrono::milliseconds(512),
	64
};

const Command UPPERCUT_COMMAND = {
	{GLFW_KEY_DOWN, GLFW_KEY_UP},
	std::chrono::milliseconds(256),
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
	glm::vec2(0,-2), glm::vec2(2,2), glm::vec2(2,2), glm::vec2(2,4), glm::vec2(2,2), glm::vec2(1,0), glm::vec2(2,2), glm::vec2(2,2), glm::vec2(2,2), glm::vec2(1,2) //para abajo
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

std::map<int, int> animMap = {
	{ 0, PlayerAnims::IDLE },
	{ inputMap[RIGHT], PlayerAnims::WALK},
	{ inputMap[DOWN], PlayerAnims::CROUCH},
	{ inputMap[DOWN] | inputMap[A], PlayerAnims::DASH1},
	{ inputMap[DOWN] | inputMap[RIGHT], PlayerAnims::CROUCH},
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
	{ DASH_COMMAND.index, PlayerAnims::DASH_COMBO },
	{ UPPERCUT_COMMAND.index, PlayerAnims::UPPERCUT },
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
	{ PlayerAnims::DOWNSTAIRS, PlayerStates::STATE_CLIMBING },
	{ PlayerAnims::ATTACK_UPSTAIRS, PlayerStates::STATE_ATTACKING },
	{ PlayerAnims::ATTACK_UPSTAIRS_SUBWEAPON, PlayerStates::STATE_ATTACKING },
	{ PlayerAnims::ATTACK_DOWNSTAIRS, PlayerStates::STATE_ATTACKING },
	{ PlayerAnims::ATTACK_DOWNSTAIRS_SUBWEAPON, PlayerStates::STATE_ATTACKING },
};

void Player::render()
{
	shader->use();
	shader->setUniform1i("flip", lookingLeft);
	int anim = sprite->animation();
	if ((loseMomentum && (anim == SKID || anim == BACKFLIP_SKID)) || sprite->animation() == UPPERCUT || bDashing || backflipping) afterimages.render();
	sprite->render();
	if (whipping)
	{
		whip->render();
	}
	shader->setUniform1i("flip", false);
}

void Player::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	Entity::init(tileMapPos, shaderProgram);
	afterimages.sprite = sprite;
	whipTex.loadFromFile("images/Richter/whip.png", TEXTURE_PIXEL_FORMAT_RGBA);
	whipTex.setMagFilter(GL_NEAREST);
	whip = Sprite::createSprite(glm::ivec2(128, 64), glm::vec2(0.1f, 1.f), &whipTex, &shaderProgram);
	whip->setNumberAnimations(2);
	whip->setAnimationSpeed(0, 30);
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

void Player::setKey()
{
	hasKey = true;
}

void Player::useKey()
{
	hasKey = false;
}

string Player::getName() const
{
	return "Player";
}

const vector<Hitbox> Player::getHitbox() const
{
	return vector<Hitbox>(1, getTerrainCollisionBox());
}

const string Player::getSpritesheet() const
{
	return "images/Richter/Richter_v4.png";
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

	sprite->setNumberAnimations(37);

	sprite->setAnimationSpeed(IDLE, 8);
	sprite->animatorX(IDLE, 4, 0.f, 0.1f, 0.f);

	sprite->setAnimationSpeed(WALK, 10);
	sprite->animatorX(WALK, 8, 0.f, 0.1f, 0.3f);

	sprite->setAnimationSpeed(JUMP, 12);
	sprite->addKeyframe(JUMP, glm::vec2(0.f, 0.4f));
	sprite->addKeyframe(JUMP, glm::vec2(0.f, 0.4f));
	sprite->addKeyframe(JUMP, glm::vec2(0.2f, 0.4f));

	sprite->setAnimationSpeed(JUMP_FW, 12);
	sprite->addKeyframe(JUMP_FW, glm::vec2(0.1f, 0.4f));
	sprite->addKeyframe(JUMP_FW, glm::vec2(0.1f, 0.4f));
	sprite->addKeyframe(JUMP_FW, glm::vec2(0.2f, 0.4f));

	sprite->setAnimationSpeed(JUMP_FINAL, 0);
	sprite->addKeyframe(JUMP_FINAL, glm::vec2(0.3f, 0.4f));

	sprite->setAnimationSpeed(FALL, 12);
	sprite->addKeyframe(FALL, glm::vec2(0.4f, 0.4f));
	sprite->addKeyframe(FALL, glm::vec2(0.5f, 0.4f));

	sprite->setAnimationSpeed(FALL_FINAL, 0);
	sprite->addKeyframe(FALL_FINAL, glm::vec2(0.5f, 0.4f));

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
	sprite->animatorX(ATTACK, 6, 0.6f, 0.f, 0.1f);

	sprite->setAnimationSpeed(ATTACK_CROUCH, attackSpeed);
	sprite->animatorX(ATTACK_CROUCH, 5, 0.f, 0.1f, 0.2f);
	sprite->animatorX(ATTACK_CROUCH, 7, 0.5f, 0.f, 0.2f);

	sprite->setAnimationSpeed(ATTACK_UPSTAIRS, attackSpeed);
	sprite->addKeyframe(ATTACK_UPSTAIRS, glm::vec2(0.8f, 0.4f));
	sprite->addKeyframe(ATTACK_UPSTAIRS, glm::vec2(0.7f, 0.5f));
	sprite->addKeyframe(ATTACK_UPSTAIRS, glm::vec2(0.8f, 0.5f));
	sprite->addKeyframe(ATTACK_UPSTAIRS, glm::vec2(0.5f, 0.6f));
	sprite->addKeyframe(ATTACK_UPSTAIRS, glm::vec2(0.6f, 0.6f));
	sprite->addKeyframe(ATTACK_UPSTAIRS, glm::vec2(0.7f, 0.6f));
	sprite->animatorX(ATTACK_UPSTAIRS, 6, 0.8f, 0.f, 0.6f);

	sprite->setAnimationSpeed(ATTACK_DOWNSTAIRS, attackSpeed);
	sprite->animatorX(ATTACK_DOWNSTAIRS, 6, 0.f, 0.1f, 0.9f);
	sprite->animatorX(ATTACK_DOWNSTAIRS, 6, 0.6f, 0.f, 0.9f);

	sprite->setAnimationSpeed(ATTACK_SUBWEAPON, 20);
	sprite->animatorX(ATTACK_SUBWEAPON, 5, 0.f, 0.1f, 0.1f);
	sprite->animatorX(ATTACK_SUBWEAPON, 4, 0.7f, 0.f, 0.1f);

	sprite->setAnimationSpeed(ATTACK_UPSTAIRS_SUBWEAPON, 20);
	sprite->addKeyframe(ATTACK_UPSTAIRS_SUBWEAPON, glm::vec2(0.8f, 0.4f));
	sprite->addKeyframe(ATTACK_UPSTAIRS_SUBWEAPON, glm::vec2(0.7f, 0.5f));
	sprite->addKeyframe(ATTACK_UPSTAIRS_SUBWEAPON, glm::vec2(0.8f, 0.5f));
	sprite->addKeyframe(ATTACK_UPSTAIRS_SUBWEAPON, glm::vec2(0.5f, 0.6f));
	sprite->addKeyframe(ATTACK_UPSTAIRS_SUBWEAPON, glm::vec2(0.6f, 0.6f));
	sprite->animatorX(ATTACK_UPSTAIRS_SUBWEAPON, 4, 0.8f, 0.f, 0.9f);

	sprite->setAnimationSpeed(ATTACK_DOWNSTAIRS_SUBWEAPON, 20);
	sprite->animatorX(ATTACK_DOWNSTAIRS_SUBWEAPON, 5, 0.f, 0.1f, 0.9f);
	sprite->animatorX(ATTACK_DOWNSTAIRS_SUBWEAPON, 4, 0.7f, 0.f, 0.9f);

	sprite->setAnimationSpeed(SKID, 32);
	sprite->addKeyframe(SKID, glm::vec2(0.9f, 0.4f));
	sprite->addKeyframe(SKID, glm::vec2(0.9f, 0.4f));
	sprite->addKeyframe(SKID, glm::vec2(0.0f, 0.5f));
	sprite->addKeyframe(SKID, glm::vec2(0.0f, 0.5f));
	sprite->addKeyframe(SKID, glm::vec2(0.9f, 0.4f));
	sprite->addKeyframe(SKID, glm::vec2(0.9f, 0.4f));
	sprite->addKeyframe(SKID, glm::vec2(0.0f, 0.5f));
	sprite->addKeyframe(SKID, glm::vec2(0.0f, 0.5f));

	sprite->setAnimationSpeed(DASH1, 24);
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
	//sprite->addKeyframe(DASH_KICK, glm::vec2(0.6f, 0.6f));
	//sprite->addKeyframe(DASH_KICK, glm::vec2(0.6f, 0.6f));
	//sprite->addKeyframe(DASH_KICK, glm::vec2(0.7f, 0.5f));
	//sprite->addKeyframe(DASH_KICK, glm::vec2(0.8f, 0.5f));
	//sprite->addKeyframe(DASH_KICK, glm::vec2(0.9f, 0.f));

	sprite->setAnimationSpeed(DASH_KICK_FINAL, 0);
	sprite->addKeyframe(DASH_KICK_FINAL, glm::vec2(0.4f, 0.6f));

	sprite->setAnimationSpeed(DASH_COMBO, 16);
	sprite->addKeyframe(DASH_COMBO, glm::vec2(0.8f, 0.3f));
	sprite->addKeyframe(DASH_COMBO, glm::vec2(0.9f, 0.3f));
	sprite->animatorX(DASH_COMBO, 5, 0.9f, 0.f, 0.3f);

	sprite->setAnimationSpeed(DASH_COMBO_FINAL, 0);
	sprite->addKeyframe(DASH_COMBO_FINAL, glm::vec2(0.9f, 0.3f));

	sprite->setAnimationSpeed(UPPERCUT, 0);
	sprite->addKeyframe(UPPERCUT, glm::vec2(0.9f, 0.6f));

	sprite->setAnimationSpeed(BACKFLIP, 10);
	sprite->animatorX(BACKFLIP, 3, 0.f, 0.1f, 0.6f);

	sprite->setAnimationSpeed(BACKFLIP_FINAL, 0);
	sprite->addKeyframe(BACKFLIP_FINAL, glm::vec2(0.3f, 0.6f));

	sprite->setAnimationSpeed(BACKFLIP_SKID, 2);
	sprite->addKeyframe(BACKFLIP_SKID, glm::vec2(0.4f, 0.6f));

	sprite->setAnimationSpeed(ULT, 16);
	sprite->addKeyframe(ULT, glm::vec2(0.6f, 0.f));
	sprite->addKeyframe(ULT, glm::vec2(0.6f, 0.f));
	sprite->addKeyframe(ULT, glm::vec2(0.9f, 0.5f));

	sprite->setAnimationSpeed(ULT_FINAL, 0);
	sprite->addKeyframe(ULT_FINAL, glm::vec2(0.9f, 0.9f));

	sprite->setAnimationSpeed(CLIMB_IDLE_UP, 0);
	sprite->addKeyframe(CLIMB_IDLE_UP, glm::vec2(0.f, 0.7f));

	sprite->setAnimationSpeed(CLIMB_IDLE_DOWN, 0);
	sprite->addKeyframe(CLIMB_IDLE_DOWN, glm::vec2(0.9f, 0.7f));

	sprite->setAnimationSpeed(UPSTAIRS, 20);
	sprite->animatorX(UPSTAIRS, 8, 0.1f, 0.1f, 0.7f);

	sprite->setAnimationSpeed(DOWNSTAIRS, 24);
	sprite->animatorX(DOWNSTAIRS, 10, 0.f, 0.1f, 0.8f);

	sprite->setTransition(JUMP, JUMP_FINAL);
	sprite->setTransition(JUMP_FW, JUMP_FINAL);
	sprite->setTransition(FALL, FALL_FINAL);
	sprite->setTransition(CROUCH, CROUCH_FINAL);
	sprite->setTransition(GETUP, IDLE);
	sprite->setTransition(ATTACK, IDLE);
	sprite->setTransition(ATTACK_CROUCH, CROUCH_FINAL);
	sprite->setTransition(ATTACK_UPSTAIRS, CLIMB_IDLE_UP);
	sprite->setTransition(ATTACK_DOWNSTAIRS, CLIMB_IDLE_DOWN);
	sprite->setTransition(ATTACK_SUBWEAPON, IDLE);
	sprite->setTransition(ATTACK_UPSTAIRS_SUBWEAPON, CLIMB_IDLE_UP);
	sprite->setTransition(ATTACK_DOWNSTAIRS_SUBWEAPON, CLIMB_IDLE_DOWN);
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
		bool rightPressed = Game::instance().getKey(GLFW_KEY_RIGHT) && !bDashing && !backflipping && anim != BACKFLIP_SKID;
		bool leftPressed = Game::instance().getKey(GLFW_KEY_LEFT) && !rightPressed && !bDashing && !backflipping && anim != BACKFLIP_SKID;
		lookingLeft = (leftPressed || (lookingLeft && !rightPressed)) * (state != STATE_ATTACKING && state != STATE_CROUCHING) + lookingLeftAtk * (state == STATE_ATTACKING || state == STATE_CROUCHING);

		if (loseMomentum)
		{
			if (abs(velocityX) == (1 + backflipping) && anim != (SKID + backflipping))
			{
				if (anim != ATTACK)
				{
					sprite->changeAnimation(SKID + backflipping);
					anim = SKID + backflipping;
					state = STATE_IDLE;
				}
				else
				{
					loseMomentum = false;
					backflipping = false;
					velocityX = 0.f;
				}
				backflipping = false;
			}
			velocityX *= 0.9f;
			if (abs(velocityX) < 0.1f)
			{
				velocityX = 0.f;
				loseMomentum = false;
				backflipping = false;
			}
		}
	
		bool getup = (prevDownPressed && grounded && !bDashing && !Game::instance().getKey(GLFW_KEY_DOWN) && anim != GETUP && (state != STATE_DASHING && state != STATE_DASHKICKING))
			|| ((state == STATE_DASHING || state == STATE_DASHKICKING) && !bDashing && !Game::instance().getKey(GLFW_KEY_DOWN)) || (timeRecoveringFromJump >= 0.5 && recoverFromJump);

		if (bJumping)
		{
			velocityX = float(rightPressed - leftPressed);
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
				Hitbox pixBoxR, pixBoxL;
				glm::vec2 pixelR(42, 63); glm::vec2 pixelL(24, 63);
				pixBoxR.min = position + pixelR; pixBoxL.min = position + pixelL;
				pixBoxR.max = pixBoxR.min; pixBoxL.max = pixBoxL.min;
				if (Game::instance().getKey(GLFW_KEY_UP) && jumpAngle >= 90
					&& (((tile = stairs->collisionMoveDownWithTileNum(pixBoxR)) == 1)
						|| ((tile = stairs->collisionMoveDownWithTileNum(pixBoxL)) == 2)))
				{
					climbToStair(tile);
				}
				else if (jumpAngle >= 180)
				{
					bJumping = false;
					//loseMomentum = backflipping && !whipping;
					position.y = startY;
					JUMP_HEIGHT = 64;
					jumpAngle = 0;
				}
				else
				{
					position.y = startY - JUMP_HEIGHT * sin(glm::radians((float)jumpAngle));
					bJumping = jumpAngle < 90 || (!tileMap->collisionMoveDown(getTerrainCollisionBox(), &position.y, quadSize.y) 
						&& !platforms->collisionMoveDown(getTerrainCollisionBox(), &position.y, quadSize.y));
					JUMP_HEIGHT = JUMP_HEIGHT * bJumping + 64 * (!bJumping);
					JUMP_ANGLE_STEP = 2 + 2 * (jumpAngle < 90 || JUMP_HEIGHT == 64);
					if (state != STATE_FALLING && jumpAngle >= 128 && state != STATE_ATTACKING && !backflipping) sprite->changeAnimation(FALL);
					else if (!bJumping && backflipping)
					{
						loseMomentum = !whipping;
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
			else
			{
				sprite->changeAnimation(GETUP);
				recoverFromJump = false;
				timeRecoveringFromJump = 0;
				lookingLeftAtk = lookingLeft;
			}
		}
		else if (recoverFromJump && grounded)
		{
			if (state != STATE_CROUCHING && anim != ATTACK_CROUCH)
			{
				if (whipping)
				{
					int keyframe = sprite->getCurrentKeyframe();
					sprite->changeAnimation(ATTACK_CROUCH);
					sprite->setKeyframe(keyframe);
					bCrouching = true;
				}
				else sprite->changeAnimation(CROUCH);
				timeRecoveringFromJump = 0;
				bDashing = false;
				velocityX = 0;
				fallDistance = 0;
			}
			else
			{
				timeRecoveringFromJump += deltaTime / 1000.f;
			}
		}
		else if (!bDashing)
		{
			position.y += FALL_SPEED;
			grounded = tileMap->collisionMoveDown(getTerrainCollisionBox(), &position.y, quadSize.y) 
				|| platforms->collisionMoveDown(getTerrainCollisionBox(), &position.y, quadSize.y);
			canJump = canJump || (grounded && !Game::instance().getKey(GLFW_KEY_Z));
			if (!grounded)
			{
				canJump = false;
				fallDistance += FALL_SPEED;
				recoverFromJump = fallDistance >= JUMP_HEIGHT;
				int tile = 0;
				Hitbox pixBoxR, pixBoxL;
				glm::vec2 pixelR(42, 63); glm::vec2 pixelL(24, 63);
				pixBoxR.min = position + pixelR; pixBoxL.min = position + pixelL;
				pixBoxR.max = pixBoxR.min; pixBoxL.max = pixBoxL.min;
				if (Game::instance().getKey(GLFW_KEY_X) && !whipping)
				{
					sprite->changeAnimation(ATTACK + Game::instance().getKey(GLFW_KEY_UP));
					whip->changeAnimation(0);
					whipping = true;
				}
				else if (Game::instance().getKey(GLFW_KEY_UP)
					&& (((tile = stairs->collisionMoveDownWithTileNum(pixBoxR)) == 1)
						|| ((tile = stairs->collisionMoveDownWithTileNum(pixBoxL)) == 2)))
				{
					climbToStair(tile);
				}
			}
			else if (anim == BACKFLIP_FINAL)
			{
				fallDistance = 0;
				loseMomentum = true;
			}
			else if (whipping)
			{
				fallDistance = 0;
				backflipping = false;
				loseMomentum = false;
			}
			else fallDistance = 0;
			if (Game::instance().getKey(GLFW_KEY_Z) && !Game::instance().getKey(GLFW_KEY_DOWN) && canJump && grounded && anim != BACKFLIP_SKID && state != STATE_ATTACKING)
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
				canJump = false;
				jumpAngle = 0;
				startY = position.y;
				Game::instance().keyReleased(GLFW_KEY_Z);
			}
			else
			{
				if (dashOffLedge)
				{
					calcIncrement(velocityX, 0.f, 0.075f);
					dashOffLedge = velocityX != 0;
				}
				else if (!loseMomentum)
				{
					loseMomentum = !(rightPressed || leftPressed) && backflipping;
					velocityX = float(((rightPressed - leftPressed) + (lookingLeft - !lookingLeft)*backflipping*2 
						+ ((!lookingLeft - lookingLeft) * loseMomentum))) * (state != STATE_ATTACKING && state != STATE_CROUCHING);
				}
				// Calculate animation based on input
				int inputIndex = 0;
				if (anim != BACKFLIP_SKID && anim != BACKFLIP_FINAL)
				{
					inputIndex += inputMap[RIGHT] * rightPressed
						+ inputMap[LEFT] * leftPressed
						+ inputMap[UP] * (Game::instance().getKey(GLFW_KEY_UP) * (anim != GETUP))
						+ inputMap[DOWN] * Game::instance().getKey(GLFW_KEY_DOWN)
						+ inputMap[A] * (Game::instance().getKey(GLFW_KEY_Z) && canJump)
						+ inputMap[X] * Game::instance().getKey(GLFW_KEY_X);
					//Register inputs for command detection
					int inputToRegister = GLFW_KEY_RIGHT * (!prevRightPressed && rightPressed)
						+ GLFW_KEY_LEFT * (!prevLeftPressed && leftPressed)
						+ GLFW_KEY_UP * Game::instance().getKey(GLFW_KEY_UP)
						+ GLFW_KEY_DOWN * Game::instance().getKey(GLFW_KEY_DOWN);
					if (inputToRegister != 0) registerInput(inputToRegister);
				}
				int tile = 0;
				int distance = 0;
				int commandInputIndex = 0;
				if (Game::instance().getKey(GLFW_KEY_X) && (rightPressed || leftPressed)
					&& (commandInputIndex = DASH_COMMAND.index * (checkCommand(DASH_COMMAND.sequence, DASH_COMMAND.timeWindow) && (rightPressed || leftPressed))) > 0)
				{
					commandBuffer.clear();
					inputIndex = commandInputIndex;
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
				//cout << "Input Index: " << inputIndex << endl;
				// Low dash or dash combo
				Hitbox cb = getTerrainCollisionBox();
				if (state != STATE_ATTACKING && ((inputIndex == 6 && state != STATE_DASHING && state != STATE_FALLING) || inputIndex == DASH_COMMAND.index) && ((!lookingLeft && !tileMap->tileRight(cb)) || (lookingLeft && !tileMap->tileLeft(cb))))
				{
					bDashing = true;
					velocityX = (!lookingLeft - lookingLeft) * 8.f;
					Game::instance().keyReleased(GLFW_KEY_Z);
				}
				auto it = animMap.find(inputIndex);
				if (it != animMap.end() && state != animToStateMap[it->second] && state != STATE_ATTACKING && grounded && !whipping)
				{
					sprite->changeAnimation(it->second);
					if (sprite->animation() == (ATTACK + bCrouching * 2)) whip->changeAnimation(0);
					else if (sprite->animation() == ATTACK_SUBWEAPON && hasKey) sprite->changeAnimation(ATTACK);
					lookingLeftAtk = lookingLeft;
				}
				else if (!grounded && state != STATE_FALLING && !whipping)
				{
					sprite->changeAnimation(FALL + backflipping);
				}
				else if (anim == ATTACK && Game::instance().getKey(GLFW_KEY_DOWN))
				{
					int keyframe = sprite->getCurrentKeyframe();
					sprite->changeAnimation(ATTACK_CROUCH);
					sprite->setKeyframe(keyframe);
				}
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
		whipUpdate(deltaTime);
	}
	else if (bClimbing)
	{
		if (linedUpStair)
		{
			if (Game::instance().getKey(GLFW_KEY_Z) && !Game::instance().getKey(GLFW_KEY_DOWN))
			{
				jump();
			}
			else
			{
				prevYpos = position.y;
				if (state != STATE_ATTACKING) stairMovement();
				if (Game::instance().getKey(GLFW_KEY_X) && state != STATE_ATTACKING)
				{
					attackInStairs = true;
					useSubweaponInStairs = Game::instance().getKey(GLFW_KEY_UP);
				}
				bool goingUp = (prevYpos - position.y) > 0;
				bool goingDown = (prevYpos - position.y) < 0;
				Hitbox cb = getStairsCollisionBox();
				int kf = sprite->getCurrentKeyframe();
				if (goingUp) bClimbing = stairs->collisionMoveDown(cb);
				else if (goingDown) bClimbing = !platforms->collisionMoveDown(cb, &position.y, quadSize.y) && !tileMap->collisionMoveDown(cb, &position.y, quadSize.y);
				else if (((anim == CLIMB_IDLE_UP || (anim == UPSTAIRS && kf == 4)) || (anim == CLIMB_IDLE_DOWN || (anim == DOWNSTAIRS && kf == 4))) && (attackInStairs || useSubweaponInStairs))
				{
					sprite->changeAnimation(ATTACK_UPSTAIRS + (anim == CLIMB_IDLE_DOWN || anim == DOWNSTAIRS) * 2 + useSubweaponInStairs * !hasKey);
					if (!useSubweaponInStairs || (useSubweaponInStairs && !hasKey)) whip->changeAnimation(0);
					whipping = true;
					attackInStairs = useSubweaponInStairs = false;
					int correction = (anim == UPSTAIRS) + (anim == DOWNSTAIRS) * 2;
					position.y -= float(correction);
				}
				bClimbing = bClimbing && !(Game::instance().getKey(GLFW_KEY_DOWN) && Game::instance().getKey(GLFW_KEY_Z));
				linedUpStair = bClimbing;
				whipUpdate(deltaTime);
			}
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
			bCrouching = false;
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
	//cout << position.x << ' ' << position.y << endl;
}

void Player::stairMovement()
{
	int anim = sprite->animation();
	bool up = Game::instance().getKey(GLFW_KEY_UP);
	bool down = Game::instance().getKey(GLFW_KEY_DOWN);
	bool right = Game::instance().getKey(GLFW_KEY_RIGHT);
	bool left = Game::instance().getKey(GLFW_KEY_LEFT);
	if (!stepping && (up || (right && rightUpStair) || (left && !rightUpStair)))
	{
		goDown = false;
		lookingLeft = !rightUpStair;
	}
	else if (!stepping && (down || (left && rightUpStair) || (right && !rightUpStair)))
	{
		goDown = true;
		lookingLeft = rightUpStair;
	}
	//parriba
	if (!goDown)
	{
		//puede parecer que es redundante pero es necesario por la lazy evaluation del primer if de la funcion
		bool keypressed = up || (right && rightUpStair) || (left && !rightUpStair);
		int kf = sprite->getCurrentKeyframe();
		stepping = stepping && (anim == UPSTAIRS) && ((kf < 5 && !stepping2) || (stepping2 && (kf >= 5 || kf == 0 || keypressed)));
		stepping2 = stepping2 && kf != 1 && (!sprite->animationEnded() || kf == 0);
		//cout << "keyframe: " << sprite->getCurrentKeyframe() <<endl<< "keypressed: " << keypressed << endl;
		if (!stepping)
		{
			if (!keypressed && anim != CLIMB_IDLE_UP)
			{
				sprite->changeAnimation(CLIMB_IDLE_UP);
				position.y -= prevKeyframe != 0;
				stepping2 = false;
			}
			else if (keypressed && anim != UPSTAIRS)
			{
				sprite->changeAnimation(UPSTAIRS);
				stepping = true;
			}
			else if (anim == UPSTAIRS && kf == 5)
			{
				stepping = true;
				stepping2 = true;
				float xDispl = stairsOffset[kf].x * (prevAnim == anim);
				position.x += xDispl * (rightUpStair - !rightUpStair);
				position.y += stairsOffset[kf].y * (prevAnim == anim);
			}
		}
		else
		{
			int currentKeyframe = sprite->getCurrentKeyframe();
			if (currentKeyframe != prevKeyframe)
			{
				int currentAnim = sprite->animation();
				float xDispl = stairsOffset[kf].x * (prevAnim == anim);
				position.x += xDispl * (rightUpStair - !rightUpStair);
				position.y += stairsOffset[currentKeyframe].y * (prevAnim == currentAnim);
			}
		}
		prevAnim = sprite->animation();
	}
	//pabajo
	else
	{
		bool keypressed = down || (left && rightUpStair) || (right && !rightUpStair);
		int kf = sprite->getCurrentKeyframe();
		stepping = stepping && (anim == DOWNSTAIRS) && ((kf < 5 && !stepping2) || (stepping2 && (kf >= 5 || kf == 0 || keypressed)));
		stepping2 = stepping2 && kf != 1 && (!sprite->animationEnded() || kf == 0);
		if (!stepping)
		{
			if (!keypressed && anim != CLIMB_IDLE_DOWN)
			{
				sprite->changeAnimation(CLIMB_IDLE_DOWN);
				position.y -= 2 * (prevKeyframe != 0);
				stepping2 = false;
			}
			else if (keypressed && anim != DOWNSTAIRS)
			{
				sprite->changeAnimation(DOWNSTAIRS);
				stepping = true;
			}
			else if (anim == DOWNSTAIRS && kf == 5)
			{
				stepping = true;
				stepping2 = true;
				float xDispl = stairsOffset[kf+8].x * (prevAnim == anim);
				position.x += xDispl * (!rightUpStair - rightUpStair);
				position.y += stairsOffset[kf+8].y * (prevAnim == anim);
			}
		}
		else
		{
			int currentKeyframe = sprite->getCurrentKeyframe();
			if (currentKeyframe != prevKeyframe)
			{
				int currentAnim = sprite->animation();
				float xDispl = stairsOffset[kf+8].x * (prevAnim == anim);
				//bool reset = prevKeyframe == 9 && currentKeyframe == 0;
				position.x += (xDispl/* - 2 * reset*/) * (!rightUpStair - rightUpStair);
				position.y += stairsOffset[currentKeyframe + 8].y * (prevAnim == currentAnim)/* - 4 * reset*/;
			}
		}
		prevAnim = sprite->animation();
	}
	prevKeyframe = sprite->getCurrentKeyframe();
}

void Player::climbToStair(int tile)
{
	//cout << "altura: " << startY - position.y << endl;
	//cout << "angulo: " << jumpAngle << endl;
	//cout << "posicion X antes del ajuste: " << position.x << endl;
	//cout << "posicion Y antes del ajuste: " << position.y << endl;
	JUMP_HEIGHT = 64;
	JUMP_ANGLE_STEP = 4;
	bJumping = false;
	bClimbing = true;
	bCrouching = false;
	backflipping = false;
	linedUpStair = true;
	goDown = false;
	velocityX = 0;
	sprite->changeAnimation(CLIMB_IDLE_UP);
	rightUpStair = tile == 1;
	lookingLeft = tile == 2;
	int height = int(std::round(startY - position.y));
	if (height < JUMP_HEIGHT) position.y += 8;
	position.x = (float)alignPosXToGrid(position.x);
	position.y = (float)alignPosYToGrid(position.y);
	//extra adjustment to stair position
	if (rightUpStair)
	{
		Hitbox pixBox;
		glm::vec2 pixel(42, 63);
		pixBox.min = position + pixel;
		pixBox.max = pixBox.min;
		position.x -= 8 * (stairs->collisionMoveDownWithTileNum(pixBox) != 1);
		//cout << "posicion X despues del ajuste: " << position.x << endl;
		//cout << "posicion Y despues del ajuste: " << position.y << endl;
	}
	else
	{
		Hitbox pixBox;
		glm::vec2 pixel(24, 63);
		pixBox.min = position + pixel;
		pixBox.max = pixBox.min;
		position.x += 8 * (stairs->collisionMoveDownWithTileNum(pixBox) != 2);
		//cout << "posicion X despues del ajuste: " << position.x << endl;
		//cout << "posicion Y despues del ajuste: " << position.y << endl;
	}
}

void Player::jump()
{
	sprite->changeAnimation(JUMP + (Game::instance().getKey(GLFW_KEY_RIGHT) || Game::instance().getKey(GLFW_KEY_LEFT)));
	bJumping = true;
	grounded = false;
	backflipping = false;
	bClimbing = false;
	jumpAngle = 0;
	startY = position.y;
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

void Player::whipUpdate(int deltaTime)
{
	int anim = sprite->animation();
	whipping = ((anim == ATTACK) || (anim == ATTACK_CROUCH) || (anim == ATTACK_UPSTAIRS) || (anim == ATTACK_DOWNSTAIRS));
	if (whipping)
	{
		whip->update(deltaTime);
		int whipKF = whip->getCurrentKeyframe();
		whip->setPosition(glm::vec2(position.x + crouchWhipOffset[whipKF].x * !lookingLeft + leftCrouchWhipOffset[whipKF].x * lookingLeft * bCrouching + tileMapDispl.x - 64 * lookingLeft, position.y + 1 + crouchWhipOffset[whipKF].y * bCrouching + tileMapDispl.y));
	}
	Game::instance().keyReleased(GLFW_KEY_X);
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

int Player::alignPosYToGrid(float axisPos) const
{
	float div = axisPos / 8;
	if (int(div * 10) % 5 == 0) div = float(int(div));
	else div = std::round(div);
	return int(div * 8);
	//return axisPos + (8 - int(axisPos) % 8);
}

int Player::alignPosXToGrid(float axisPos) const
{
	int newPos;
	int def, exc;
	def = exc = 0;
	if (rightUpStair)
	{
		def = (int)axisPos - int(axisPos) % 8 + 2;
		exc = (int)axisPos + (8 - int(axisPos) % 8) + 2;
	}
	else
	{
		def = (int)axisPos - int(axisPos) % 8 + 5;
		exc = (int)axisPos + (8 - int(axisPos) % 8) + 5;
	}
	int defdiff = (int)axisPos - def;
	int excdiff = exc - (int)axisPos;
	if (defdiff < excdiff) newPos = def;
	else newPos = exc;
	return newPos;
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
	int leftUpStairOffset = 6 * !rightUpStair;
	box.min = position + glm::vec2(27 - leftUpStairOffset, 63);
	box.max = position + glm::vec2(42 - leftUpStairOffset, 63);
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
	box.min = position + glm::vec2(31, 72);
	box.max = position + glm::vec2(38, 78);
	return box;
}