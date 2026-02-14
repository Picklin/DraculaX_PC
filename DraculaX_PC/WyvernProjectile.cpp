#include "WyvernProjectile.h"
#define BOUNCE_ANGLE_STEP 2

int WyvernProjectile::getDamage() const
{
    return 10;
}

const Hitbox WyvernProjectile::getHitbox() const
{
    return hitboxes[sprite->animation()];
}

const string WyvernProjectile::getSpritesheet() const
{
    return "images/bosses/wyvern/wyv_fire.png";
}

const glm::vec2 WyvernProjectile::getSizeInSpritesheet() const
{
    return glm::vec2(0.2f,0.5f);
}

const glm::ivec2 WyvernProjectile::getQuadSize() const
{
    return glm::ivec2(32,48);
}

void WyvernProjectile::childUpdate(int deltaTime)
{

}

void WyvernProjectile::setAnimations()
{
    sprite->setNumberAnimations(2);
    sprite->setAnimationSpeed(0, 60);
    sprite->animatorX(0, 5, 0.f, 0.2f, 0.f);
    sprite->setAnimationSpeed(1, 60);
    sprite->animatorX(1, 5, 0.f, 0.2f, 0.5f);
    sprite->changeAnimation(0);

    Hitbox hb;
    hb.min = glm::vec2(12 - 8 * (dir.x < 0), 26);
    hb.max = hb.min + glm::vec2(15);
    hitboxes[0] = hb;
    hb.min = glm::vec2(16 * (dir.x < 0), 32);
    hb.max = hb.min + glm::vec2(15);
    hitboxes[1] = hb;
}

glm::vec2 WyvernProjectile::setSpeed()
{
    return glm::vec2(3,6);
}

float WyvernProjectile::setEndTime()
{
    return 5.f;
}

int WyvernProjectile::setEndAnimation() const
{
    return 1;
}
