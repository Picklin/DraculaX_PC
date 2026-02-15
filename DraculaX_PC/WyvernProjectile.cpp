#include "WyvernProjectile.h"
#define BOUNCE_ANGLE_STEP 2

WyvernProjectile::WyvernProjectile() : xSpeeds{ .25f, .5f, .75f, 1.f, 1.25f, 1.5f, 1.75f, 2.f}, bounceYDists{ 128, 112, 96, 80, 64 }
{
    Hitbox hb;
    hb.min = glm::vec2(12 - 8 * (dir.x < 0), 26);
    hb.max = hb.min + glm::vec2(15);
    hitboxes[0] = hb;
    hb.min = glm::vec2(16 * (dir.x < 0), 32);
    hb.max = hb.min + glm::vec2(15);
    hitboxes[1] = hb;
}

void WyvernProjectile::render()
{
	shader->setUniform1f("frameWidth", 0.2f);
	shader->setUniform1i("flip", dir.x > 0);
	if (!ended) sprite->render();
    else
    {
		if (renderAlternate) sprite->render();
		renderAlternate = !renderAlternate;
    }
	shader->setUniform1i("flip", false);
}

int WyvernProjectile::getDamage() const
{
    return 10;
}

const Hitbox WyvernProjectile::getHitbox() const
{
	Hitbox hb = hitboxes[sprite->animation()];
	hb.min += position;
    hb.max += position;
    return hb;
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
    if (!bouncing)
    {
        position += speed * dir;
        if (tileMap->collisionMoveDown(getHitbox()))
        {
            bouncing = true;
			bounceYDist = bounceYDists[rand() % 5];
			bounceXSpeed = xSpeeds[rand() % 8] * dir.x;
			startY = (int)position.y;
		}
    }
    else
    {
        bounceAngle += BOUNCE_ANGLE_STEP;
        position.x += bounceXSpeed;
        position.y = startY - bounceYDist * sin(glm::radians((float)bounceAngle));
        if (bounceAngle >= 180)
        {
            bouncing = false;
            bounceAngle = 0;
            bounceXSpeed = 0;
            end();
		}
    }
    setPosition(position);
}

void WyvernProjectile::setAnimations()
{
    sprite->setNumberAnimations(2);
    sprite->setAnimationSpeed(0, 60);
    sprite->animatorX(0, 5, 0.f, 0.2f, 0.f);
    sprite->setAnimationSpeed(1, 60);
    sprite->animatorX(1, 5, 0.f, 0.2f, 0.5f);
    sprite->changeAnimation(0);
}

glm::vec2 WyvernProjectile::setSpeed()
{
    return glm::vec2(3,6);
}

float WyvernProjectile::setEndTime()
{
    return 3.f;
}