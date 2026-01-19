#include "Wyvern.h"

#define MAX_HP 92
#define DAMAGE 15
#define POINTS 2500

enum WyvernAnims
{
    APPEAR, APPEAR_FINAL, IDLE, TURN, ATTACK, ATTACK_FINAL, FIRE, FIRE_FINAL, DIE
};

void Wyvern::init(const glm::ivec2& tileMapDispl, ShaderProgram& shader)
{
    this->tileMapDispl = tileMapDispl;
    tex.loadFromFile("images/bosses/wyvern/wyvern.png", TEXTURE_PIXEL_FORMAT_RGBA);
    tex.setMagFilter(GL_NEAREST);
    sprite = Sprite::createSprite(glm::ivec2(128), glm::vec2(0.1f, 0.5f), &tex, &shader);
    sprite->setNumberAnimations(9);
    sprite->setAnimationSpeed(APPEAR, 8);
    sprite->animatorX(APPEAR, 3, 0.f, 0.1f, 0.f);
    sprite->addKeyframe(APPEAR_FINAL, glm::vec2(0.3f, 0.f));
    sprite->setAnimationSpeed(IDLE, 12);
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
    sprite->addKeyframe(DIE, glm::vec2(0.5f, 0.5f));
    sprite->setTransition(APPEAR, APPEAR_FINAL);
    sprite->setTransition(TURN, IDLE);
    sprite->setTransition(ATTACK, ATTACK_FINAL);
    sprite->setTransition(FIRE, FIRE_FINAL);
    sprite->setTransition(ATTACK_FINAL, IDLE);
    sprite->setTransition(FIRE_FINAL, IDLE);
    sprite->changeAnimation(APPEAR);
    currentHP = MAX_HP;
}

void Wyvern::setPosition(const glm::vec2& pos)
{
    this->position = pos;
    sprite->setPosition(glm::vec2(tileMapDispl) + pos);
}

void Wyvern::update(int deltaTime)
{
    sprite->update(deltaTime);
}

void Wyvern::render()
{
    sprite->render();
}

void Wyvern::takeDmg(int dmg)
{
    currentHP -= dmg;
    if (currentHP < 0) currentHP = 0;
    ended = currentHP == 0;
}

const Hitbox Wyvern::getHitbox() const
{
    return Hitbox();
}

int Wyvern::getPoints() const
{
    return POINTS;
}

bool Wyvern::isEnded() const
{
    return ended;
}

bool Wyvern::isRemoved() const
{
    return removed;
}
