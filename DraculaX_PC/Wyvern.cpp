#include "Wyvern.h"
#include "SoundEngine.h"
#include "TextureManager.h"
#include "ProjectileManager.h"
#include <iostream>

#define MAX_HP 92
#define DAMAGE 15
#define POINTS 2500

namespace
{
    enum WyvernAnims
    {
        APPEAR, APPEAR_FINAL, IDLE, TURN, ATTACK, ATTACK_FINAL, COOLFIRE, COOLFIRE_FINAL, DIE
    };
    enum  WyvernAttacks
    {
		GRAB, SHOOT_FIRE, SHOOT_COOLFIRE
	};
	const int attacks[3] = { GRAB, SHOOT_FIRE, SHOOT_COOLFIRE };
    const int leftBound = 32 * 16;
    const int rightBound = 42 * 16;
}

Wyvern::~Wyvern()
{
    sprite->free();
    delete sprite;
}

void Wyvern::init(const glm::ivec2& tileMapDispl, ShaderProgram& shader, const glm::vec2& pos)
{
    this->tileMapDispl = tileMapDispl;
    this->shader = &shader;
    if (TextureManager::instance().exists("wyvern"))
    {
        tex = TextureManager::instance().getTexture("wyvern");
		headTex = TextureManager::instance().getTexture("wyvern_head");
    }
    else
    {
        tex = new Texture();
        headTex = new Texture();
        tex->loadFromFile("images/bosses/wyvern/wyvern.png", TEXTURE_PIXEL_FORMAT_RGBA);
		headTex->loadFromFile("images/bosses/wyvern/wyvernHead.png", TEXTURE_PIXEL_FORMAT_RGBA);
        TextureManager::instance().addTexture("wyvern", tex);
		TextureManager::instance().addTexture("wyvern_head", headTex);
	}
    sprite = Sprite::createSprite(glm::ivec2(128), glm::vec2(0.1f, 0.5f), tex, &shader);
    sprite->setNumberAnimations(9);
    sprite->setAnimationSpeed(APPEAR, 8);
    sprite->animatorX(APPEAR, 3, 0.f, 0.1f, 0.f);
    sprite->setAnimationSpeed(APPEAR_FINAL, 0);
    sprite->addKeyframe(APPEAR_FINAL, glm::vec2(0.3f, 0.f));
    sprite->setAnimationSpeed(IDLE, 11);
    sprite->animatorX(IDLE, 6, 0.4f, 0.1f, 0.f);
    sprite->setAnimationSpeed(TURN, 12);
    sprite->animatorX(TURN, 2, 0.3f, 0.1f, 0.5f);
    sprite->setAnimationSpeed(ATTACK, 8);
    sprite->addKeyframe(ATTACK, glm::vec2(0.f, 0.5f));
    sprite->setAnimationSpeed(ATTACK_FINAL, 1);
    sprite->addKeyframe(ATTACK_FINAL, glm::vec2(0.1f, 0.5f));
    sprite->setAnimationSpeed(COOLFIRE, 8);
    sprite->animatorX(COOLFIRE, 2, 0.f, 0.1f, 0.5f);
    sprite->setAnimationSpeed(COOLFIRE_FINAL, 1);
    sprite->addKeyframe(COOLFIRE_FINAL, glm::vec2(0.2f, 0.5f));
    sprite->setAnimationSpeed(DIE, 0);
    sprite->addKeyframe(DIE, glm::vec2(0.5f, 0.5f));
    sprite->setTransition(APPEAR, APPEAR_FINAL);
    //sprite->setTransition(TURN, IDLE);
    sprite->setTransition(ATTACK, ATTACK_FINAL);
    sprite->setTransition(COOLFIRE, COOLFIRE_FINAL);
    sprite->setTransition(ATTACK_FINAL, IDLE);
    sprite->setTransition(COOLFIRE_FINAL, IDLE);
    sprite->changeAnimation(APPEAR);
	head = Sprite::createSprite(glm::ivec2(64), glm::vec2(0.25f, 1.f), headTex, &shader);
	head->setNumberAnimations(4);
	head->setAnimationSpeed(0, 10);
	head->addKeyframe(0, glm::vec2(0.f, 0.f));
    head->setAnimationSpeed(1, 2);
	head->addKeyframe(1, glm::vec2(0.25f, 0.f));
	head->setAnimationSpeed(2, 15);
	head->addKeyframe(2, glm::vec2(0.5f, 0.f));
	head->addKeyframe(2, glm::vec2(0.75f, 0.f));
	head->setAnimationSpeed(3, 0);
	head->addKeyframe(3, glm::vec2(0.75f, 0.f));
	head->setTransition(0, 1);
    head->setTransition(1, 2);
	head->setTransition(2, 3);
	head->changeAnimation(0);
    setPosition(pos);
    currentHP = MAX_HP;
	currentAttack = GRAB;
    appeared = attacking = firing = ended = removed = false;
    lungeAngle = 0.f;
    lungeAngleStep = 2.f;
    lungeDist = 96.f;
    lunging = true;
    startY = position.y;
    moveSpeed = 3.f;
    woundedCooldown = attackCooldown = deathTimeElapsed = 0.f;
    alpha = colorValue = 1.f;
}

void Wyvern::setPosition(const glm::vec2& pos)
{
    this->position = pos;
    sprite->setPosition(glm::vec2(tileMapDispl) + pos);
}

void Wyvern::update(int deltaTime)
{
    sprite->update(deltaTime);
    if (!ended)
    {
        if (lunging)
        {
            lungeAngle += lungeAngleStep;
            if (lungeAngle >= 180)
            {
                lunging = false;
                position.y = startY;
                lungeAngle = 0;
                lungeAngleStep = 2.f;
                lungeDist = 80;
                if (appeared) SoundEngine::instance().playLoopedSFX(SoundEngine::WYVERN_WINGS);
            }
            else
            {
                if (!appeared) lungeAngleStep = 2.f - 1.f * (lungeAngle >= 45 && lungeAngle <= 135);
                position.y = startY + lungeDist * sin(glm::radians((float)lungeAngle));
            }
        }
        if (!appeared)
        {
            calcIncrement(moveSpeed, 0.f, 0.035f);
            if (!lunging)
            {
                position.y -= 2.f;
                position.x += 0.5f;
            }
            else position.x -= moveSpeed;
            appeared = position.y <= -112.f;
            if (appeared)
            {
                SoundEngine::instance().playLoopedSFX(SoundEngine::WYVERN_WINGS);
                SoundEngine::instance().playNonStageSong(SoundEngine::BOSS, true);
                sprite->changeAnimation(IDLE);
                moveSpeed = 1.f;
                position.x = 49 * 16;
                flip = true;
            }
        }
        else if (restCooldown > 0)
        {
            restCooldown -= deltaTime / 1000.f;
            if (restCooldown <= 0)
            {
                restCooldown = 0.f;
                SoundEngine::instance().playLoopedSFX(SoundEngine::WYVERN_WINGS);
			}
		}
        else
        {
            int anim = sprite->animation();
            if (attackCooldown == 0 && anim == IDLE && position.y < 0.f)
            {
                position.y += 1.f;
                position.x += moveSpeed * (!flip - flip);
            }
            else if (attackCooldown == 0 && anim == IDLE && !firing)
            {
                if ((position.x < leftBound && flip) || (position.x > rightBound && !flip))
                {
                    sprite->changeAnimation(TURN);
                    flip = !flip;
                    //takeDmg(92);
                }
                position.x += moveSpeed * (!flip - flip);
            }
            else if (anim == TURN && sprite->animationEnded())
            {
                attackCooldown = 1.f;
                sprite->changeAnimation(IDLE);
                position.x += 32 * (flip - !flip);
            }
            else if (attackCooldown < 0)
            {
                if (currentAttack == GRAB)
                {
                    sprite->changeAnimation(ATTACK);
                    lunging = true;
                    startY = position.y;
                    attackCooldown = 0.f;
                    attacking = true;
                    moveSpeed = 3.f;
					currentAttack = SHOOT_FIRE;
                    SoundEngine::instance().stopLoopedSFX(SoundEngine::WYVERN_WINGS);
                    SoundEngine::instance().playSFX(SoundEngine::WYVERN_ATTACK);
                }
                else if (currentAttack == SHOOT_FIRE)
                {
                    sprite->changeAnimation(COOLFIRE);
                    attackCooldown = 0.f;
					firingTimeElapsed = 0;
                    firing = true;
                    currentAttack = SHOOT_COOLFIRE;
                    SoundEngine::instance().stopLoopedSFX(SoundEngine::WYVERN_WINGS);
                }
                else if (currentAttack == SHOOT_COOLFIRE)
                {
                    sprite->changeAnimation(COOLFIRE);
                    attackCooldown = 0.f;
                    coolfiring = true;
                    currentAttack = GRAB;
                    SoundEngine::instance().stopLoopedSFX(SoundEngine::WYVERN_WINGS);
                }
            }
			else if (attackCooldown == 0)
            {
                if (attacking)
                {
                    calcIncrement(moveSpeed, 0.f, 0.05f);
                    if (moveSpeed <= .3f)
                    {
                        attacking = false;
                        moveSpeed = 1.f;
                    }
                    position.x += moveSpeed * (!flip - flip);
                }
                else if (firing)
                {
                    if (sprite->animation() == COOLFIRE_FINAL && firingTimeElapsed < 576)
                    {
                        if (firingTimeElapsed >= 128 && firingTimeElapsed <= 512 && firingTimeElapsed % 64 == 0)
                        {
                            if (firingTimeElapsed == 128) SoundEngine::instance().playSFX(SoundEngine::WYVERN_FIRE);
                            ProjectileManager::instance().createEnemyProjectile(position + glm::vec2(112 - 129 * flip, 48), glm::vec2(!flip - flip, 1), ProjectileManager::WYVERN);
                        }
                        firingTimeElapsed += deltaTime;
                    }
                    else if (sprite->animation() == IDLE)
                    {
                        firing = false;
                        SoundEngine::instance().playLoopedSFX(SoundEngine::WYVERN_WINGS);
                    }
                }
                else if (coolfiring && anim == COOLFIRE_FINAL)
                {
                    ProjectileManager::instance().createEnemyProjectile(position + glm::vec2(112 - 129 * flip, 40), glm::vec2(!flip - flip, 1), ProjectileManager::WYVERN_COOL);
                    coolfiring = false;
                    SoundEngine::instance().playSFX(SoundEngine::WYVERN_ROAR);
                    restCooldown = 1.f;
                }
            }
            else attackCooldown -= deltaTime / 1000.f;
            //cout << anim << endl;
        }
        if (woundedCooldown > 0) woundedCooldown -= deltaTime / 1000.f;
    }
    else
    {
        deathTimeElapsed += deltaTime / 1000.f;
        sprite->setColor(glm::vec3(colorValue));
        if (colorValue < 0.2f)
        {
            colorValue = 0.2f;
            sprite->changeAnimation(DIE);
        }
        else colorValue -= deltaTime / 1000.f;
        if (deathTimeElapsed >= 6)
        {
            alpha -= deltaTime / 1000.f;
			sprite->setAlpha(alpha);
        }
        position.y += 0.25f;
        removed = alpha <= 0;
    }
    setPosition(position);
}

void Wyvern::render()
{
    shader->setUniform1i("flip", flip);
    shader->setUniform1f("frameWidth", 0.1f);
    sprite->render();
}

void Wyvern::takeDmg(int dmg)
{
    currentHP -= dmg;
    if (currentHP <= 0)
    {
        currentHP = 0;
        sprite->setAnimationSpeed(IDLE, 2);
        sprite->changeAnimation(IDLE);
        ended = true;
        SoundEngine::instance().stopLoopedSFX(SoundEngine::WYVERN_WINGS);
        SoundEngine::instance().playSFX(SoundEngine::WYVERN_DEATH);
    }
    else
    {
        sprite->invertColor();
        SoundEngine::instance().playSFX(SoundEngine::ENEMY_HURT);
    }
    woundedCooldown = 1.f;
}

const Hitbox Wyvern::getHitbox() const
{
    Hitbox hb;
    hb.min = position + glm::vec2(48-32*flip, 48);
    hb.max = hb.min + glm::vec2(64, 48);
    return hb;
}

int Wyvern::getPoints() const
{
    return POINTS;
}

bool Wyvern::battleStarted() const
{
    return appeared;
}

bool Wyvern::isWounded() const
{
    return woundedCooldown > 0;
}

bool Wyvern::isEnded() const
{
    return sprite->animation() == DIE;
}

bool Wyvern::isRemoved() const
{
    return removed;
}

void Wyvern::setHeadPosition()
{
	head->setPosition(glm::vec2(tileMapDispl) + position + glm::vec2(64 - flip, 16));
}

void Wyvern::calcIncrement(float& valToInc, float targetVal, float factor)
{
    if (abs(valToInc - targetVal) > 0.2f) valToInc = valToInc + (targetVal - valToInc) * factor;
    else valToInc = targetVal;
}