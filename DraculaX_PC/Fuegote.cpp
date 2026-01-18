#include "Fuegote.h"
#define UP_SPEED 1.f

string Fuegote::getName() const
{
    return "Fuegote";
}

const string Fuegote::getSpritesheet() const
{
    return "images/effects/fuegote.png";
}

void Fuegote::setAnimations()
{
    sprite->setNumberAnimations(1);

    sprite->setAnimationSpeed(0, 15);
    sprite->addKeyframe(0, glm::vec2(0.0f, 0.0f));
    sprite->addKeyframe(0, glm::vec2(0.25f, 0.f));
    sprite->addKeyframe(0, glm::vec2(0.50f, 0.f));
    sprite->addKeyframe(0, glm::vec2(0.75f, 0.f));
    sprite->addKeyframe(0, glm::vec2(0.0f, 0.25f));
    sprite->addKeyframe(0, glm::vec2(0.25f, 0.25f));
    sprite->addKeyframe(0, glm::vec2(0.5f, 0.25f));
    sprite->addKeyframe(0, glm::vec2(0.75f, 0.25f));
    sprite->addKeyframe(0, glm::vec2(0.0f, 0.5f));
    sprite->addKeyframe(0, glm::vec2(0.25f, 0.5f));
    sprite->addKeyframe(0, glm::vec2(0.5f, 0.5f));
    sprite->addKeyframe(0, glm::vec2(0.75f, 0.5f));

    sprite->changeAnimation(0);
}

void Fuegote::childUpdate(int deltaTime)
{
    position.y -= UP_SPEED;
    setPosition(position);
    ended = sprite->animationEnded();
}
