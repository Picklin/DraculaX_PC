#include "Wyvern.h"
#include "SoundEngine.h"
#include <iostream>

#define MAX_HP 92
#define DAMAGE 15
#define POINTS 2500

namespace
{
    enum WyvernAnims
    {
        APPEAR, APPEAR_FINAL, IDLE, TURN, ATTACK, ATTACK_FINAL, FIRE, FIRE_FINAL, DIE
    };
    const int leftBound = 32 * 16;
    const int rightBound = 42 * 16;
}

Wyvern::~Wyvern()
{
    tex.free();
    sprite->free();
    delete sprite;
}

void Wyvern::init(const glm::ivec2& tileMapDispl, ShaderProgram& shader, const glm::vec2& pos)
{
    this->tileMapDispl = tileMapDispl;
    this->shader = &shader;
    tex.loadFromFile("images/bosses/wyvern/wyvern.png", TEXTURE_PIXEL_FORMAT_RGBA);
    tex.setMagFilter(GL_NEAREST);
    sprite = Sprite::createSprite(glm::ivec2(128), glm::vec2(0.1f, 0.5f), &tex, &shader);
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
    sprite->setAnimationSpeed(FIRE, 8);
    sprite->animatorX(FIRE, 2, 0.f, 0.1f, 0.5f);
    sprite->setAnimationSpeed(FIRE_FINAL, 1);
    sprite->addKeyframe(FIRE_FINAL, glm::vec2(0.2f, 0.5f));
    sprite->setAnimationSpeed(DIE, 0);
    sprite->addKeyframe(DIE, glm::vec2(0.5f, 0.5f));
    sprite->setTransition(APPEAR, APPEAR_FINAL);
    //sprite->setTransition(TURN, IDLE);
    sprite->setTransition(ATTACK, ATTACK_FINAL);
    sprite->setTransition(FIRE, FIRE_FINAL);
    sprite->setTransition(ATTACK_FINAL, IDLE);
    sprite->setTransition(FIRE_FINAL, IDLE);
    sprite->changeAnimation(APPEAR);
    setPosition(pos);
    currentHP = MAX_HP;
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
                SoundEngine::instance().playNonStageSong(SoundEngine::BOSS);
                sprite->changeAnimation(IDLE);
                moveSpeed = 1.f;
                position.x = 49 * 16;
                flip = true;
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
            else if (attackCooldown == 0 && anim == IDLE)
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
                sprite->changeAnimation(ATTACK);
                lunging = true;
                startY = position.y;
                attackCooldown = 0.f;
                attacking = true;
                moveSpeed = 3.f;
                SoundEngine::instance().stopLoopedSFX(SoundEngine::WYVERN_WINGS);
                SoundEngine::instance().playSFX(SoundEngine::WYVERN_ATTACK);
            }
            else if (attacking)
            {
                calcIncrement(moveSpeed, 0.f, 0.05f);
                if (moveSpeed <= .3f)
                {
                    attacking = false;
                    moveSpeed = 1.f;
                }
                position.x += moveSpeed * (!flip - flip);
            }
            if (attackCooldown > 0) attackCooldown -= deltaTime / 1000.f;
        }
        if (woundedCooldown > 0) woundedCooldown -= deltaTime / 1000.f;
    }
    else
    {
        deathTimeElapsed += deltaTime / 1000.f;
        sprite->setColor(glm::vec4(glm::vec3(colorValue), alpha));
        if (colorValue < 0.2f)
        {
            colorValue = 0.2f;
            sprite->changeAnimation(DIE);
        }
        else colorValue -= deltaTime / 1000.f;
        if (deathTimeElapsed >= 6) alpha -= deltaTime / 1000.f;
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

void Wyvern::calcIncrement(float& valToInc, float targetVal, float factor)
{
    if (abs(valToInc - targetVal) > 0.2f) valToInc = valToInc + (targetVal - valToInc) * factor;
    else valToInc = targetVal;
}