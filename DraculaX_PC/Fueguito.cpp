#include "Fueguito.h"
#define UP_SPEED .5f

string Fueguito::getName() const
{
    return "Fueguito";
}

const string Fueguito::getSpritesheet() const
{
    return "images/effects/fueguito_v2.png";
}

const glm::vec2 Fueguito::getSizeInSpritesheet() const
{
    return glm::vec2(0.25f, 0.5f);
}

void Fueguito::setAnimations()
{
    sprite->setNumberAnimations(1);

    sprite->setAnimationSpeed(0, 15);
    sprite->addKeyframe(0, glm::vec2(0.0f, 0.0f));
    sprite->addKeyframe(0, glm::vec2(0.25f, 0.f));
    sprite->addKeyframe(0, glm::vec2(0.50f, 0.f));
    sprite->addKeyframe(0, glm::vec2(0.75f, 0.f));
    sprite->addKeyframe(0, glm::vec2(0.0f, 0.5f));
    sprite->addKeyframe(0, glm::vec2(0.25f, 0.5f));
    sprite->addKeyframe(0, glm::vec2(0.5f, 0.5f));
    sprite->addKeyframe(0, glm::vec2(0.75f, 0.5f));

    sprite->changeAnimation(0);
}

void Fueguito::childUpdate(int deltaTime)
{
    position.y -= UP_SPEED;
    setPosition(position);
    ended = sprite->animationEnded();
}
