#include "Item.h"

#define FALL_SPEED 3.f

void Item::init(const glm::ivec2& tileMapDispl, ShaderProgram& shader, const glm::vec2& topLeft, const glm::vec2& bottomRight, Texture& itemsTex)
{
    this->tileMapDispl = tileMapDispl;
    this->quadSize = glm::ivec2((bottomRight.x - topLeft.x) * itemsTex.width(), (bottomRight.y - topLeft.y) * itemsTex.height());
    sprite = Sprite::createSprite(quadSize, topLeft, bottomRight, &itemsTex, &shader);
}

void Item::setTileMap(TileMap& map)
{
    this->tileMap = &map;
}

void Item::render()
{
    sprite->render();
}

void Item::setPosition(const glm::vec2& pos)
{
    this->position = pos;
    sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
}

const Hitbox Item::getHitbox() const
{
    return Hitbox{ position, position + glm::vec2(quadSize) };
}

void Item::setUngrabable()
{
    timeElapsed = 0;
}

bool Item::isGrabable() const
{
    return timeElapsed >= ungrabableTime;
}

bool Item::isEnded() const
{
    return ended;
}

bool Item::getsRemoved() const
{
    return endTimer <= 0;
}

void Item::eject(int xDir)
{
    this->xDir = xDir;
    velocityX = xDir * minSpeedX;
    velocityY = -minSpeedY;
    beingEjected = true;
}

void Item::end()
{
    ended = true;
    makeEndSound();
}

void Item::update(int deltaTime)
{
    if (ended)
    {
        endTimer -= deltaTime;
    }
    else
    {
        if (beingEjected)
        {
            velocityY += gravity;
            velocityX -= xDir * friction;
            if (abs(velocityX) < friction)
            {
                velocityX = 0;
                beingEjected = false;
            }
        }
        else velocityY = FALL_SPEED;

        position.y += velocityY;
        position.x += velocityX;
        if (!tileMap->collisionMoveDown(getHitbox(), &position.y, quadSize.y) && platforms != nullptr)
        {
            platforms->collisionMoveDown(getHitbox(), &position.y, quadSize.y);
        }
        setPosition(position);
        timeElapsed += deltaTime;
        if (timeElapsed < ungrabableTime)
        {
            if ((timeElapsed / (deltaTime * 2)) % 4 == 0) sprite->white();
            else sprite->setColor(glm::vec4(1.f));
        }
        /*else if (lifeSpan - timeElapsed <= 2000)
        {
            if ((timeElapsed / (deltaTime * 2)) % 4 == 0) sprite->setColor(glm::vec4(0.f));
            else sprite->setColor(glm::vec4(1.f));
            if (timeElapsed >= lifeSpan) end();
        }*/
    }
}

const glm::vec2 Item::myCenter() const
{
    return glm::vec2(quadSize) / 2.f;
}
