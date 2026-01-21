#include "GolemProjectile.h"

const Hitbox GolemProjectile::getHitbox() const
{
    Hitbox hb;
    hb.min = position + glm::vec2(2.f);
    hb.max = hb.min + glm::vec2(6.f);
    return hb;
}

int GolemProjectile::getDamage() const
{
    return 10;
}

string GolemProjectile::getName() const
{
    return "GolemProjectile";
}

const string GolemProjectile::getSpritesheet() const
{
    return "images/enemies/golem/golem_projectile.png";
}

const glm::vec2 GolemProjectile::getSizeInSpritesheet() const
{
    return glm::vec2(1.f, 0.25f);
}

const glm::ivec2 GolemProjectile::getQuadSize() const
{
    return glm::ivec2(10, 10);
}

void GolemProjectile::setAnimations()
{
    sprite->setNumberAnimations(1);
    sprite->setAnimationSpeed(0, 16);
    sprite->addKeyframe(0, glm::vec2(0.f, 0.f));
    sprite->addKeyframe(0, glm::vec2(0.f, 0.25f));
    sprite->addKeyframe(0, glm::vec2(0.f, 0.5f));
    sprite->addKeyframe(0, glm::vec2(0.f, 0.75f));
    sprite->changeAnimation(0);
}
