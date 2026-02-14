#include "GolemProjectile.h"

void GolemProjectile::childUpdate(int deltaTime)
{
    if (tileMap->collisionMoveDown(getHitbox())) end();
}

void GolemProjectile::setTileMap(TileMap* tileMap)
{
    this->tileMap = tileMap; 
}

const Hitbox GolemProjectile::getHitbox() const
{
    Hitbox hb;
    hb.min = position + glm::vec2(12.f);
    hb.max = hb.min + glm::vec2(8.f);
    return hb;
}

bool GolemProjectile::getsRemoved() const
{
    return sprite->animationEnded();
}

int GolemProjectile::getDamage() const
{
    return 10;
}

const string GolemProjectile::getSpritesheet() const
{
    return "images/enemies/golem/golem_projectile.png";
}

const glm::vec2 GolemProjectile::getSizeInSpritesheet() const
{
    return glm::vec2(0.1f, 0.5f);
}

const glm::ivec2 GolemProjectile::getQuadSize() const
{
    return glm::ivec2(32);
}

void GolemProjectile::setAnimations()
{
    sprite->setNumberAnimations(2);
    sprite->setAnimationSpeed(0, 60);
    sprite->animatorX(0, 4, 0.f, 0.1f, 0.f);
    sprite->setAnimationSpeed(1, 20);
    sprite->animatorX(1, 6, 0.4f, 0.1f, 0.f);
    sprite->addKeyframe(1, glm::vec2(0.f, 0.5f));
    sprite->changeAnimation(0);
}

glm::vec2 GolemProjectile::setSpeed()
{
    return glm::vec2(1);
}

int GolemProjectile::setEndAnimation() const
{
    return 1;
}
