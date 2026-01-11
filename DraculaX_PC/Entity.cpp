#include "Entity.h"
#include "TextureManager.h"
#include "SpriteManager.h"

Entity::~Entity()
{
	if (sprite != NULL)
	{
		sprite->free();
		delete sprite;
	}
}

void Entity::init(const glm::ivec2& tileMapDispl, ShaderProgram& shaderProgram)
{
	this->tileMapDispl = tileMapDispl;
	this->shader = &shaderProgram;
	string name = getName();
	if (TextureManager::instance().exists(name))
	{
		spritesheet = TextureManager::instance().getTexture(name);
	}
	else
	{
		spritesheet = new Texture();
		spritesheet->loadFromFile(getSpritesheet(), TEXTURE_PIXEL_FORMAT_RGBA);
		spritesheet->setMagFilter(GL_NEAREST);
		TextureManager::instance().addTexture(name, spritesheet);
	}
	quadSize = getQuadSize();
	sprite = Sprite::createSprite(quadSize, getSizeInSpritesheet(), spritesheet, &shaderProgram);
	if (SpriteManager::instance().exists(name))
	{
		sprite->addAnimations(SpriteManager::instance().getAnimations(name));
		sprite->changeAnimation(0);
	}
	else
	{
		setAnimations();
		setHitboxes();
		SpriteManager::instance().addAnimations(name, sprite->getAnimations());
	}
	ended = false;
	setPivotPoint(myCenter());
}

void Entity::setTileMap(TileMap* tileMap)
{
	this->tileMap = tileMap;
}

void Entity::setPosition(const glm::vec2& pos)
{
	this->position = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
}

void Entity::setPivotPoint(const glm::vec2& pivotPoint)
{
	this->pivotPoint = pivotPoint;
	sprite->setCenter(pivotPoint);
}

const glm::vec2 Entity::myCenter() const
{
	return glm::vec2(quadSize.x / 2.f, quadSize.y / 2.f);
}

void Entity::update(int deltaTime)
{
	if (!ended)
	{
		childUpdate(deltaTime);
	}
	else
	{
		sprite->update(deltaTime);
		endUpdate(deltaTime);
		endTimer -= deltaTime;
	}
}

void Entity::render()
{
	sprite->render();
}

const Hitbox Entity::getHitbox() const
{
	Hitbox frame_hitbox = hitboxes[sprite->animation()][sprite->getCurrentKeyframe()];
	
	float angle = sprite->getAngleDegrees();
	if (angle != 0.f)
	{
		frame_hitbox = getRotatedHitbox(frame_hitbox, angle);
	}
	frame_hitbox.min += position;
	frame_hitbox.max += position;
	
	return frame_hitbox;
}

void Entity::rotate(float angle)
{
	sprite->setAngleDegrees(angle);
}

void Entity::end()
{
	ended = true;
	endTimer = setEndTime();
	sprite->changeAnimation(setEndAnimation());
	makeEndSound();
	makeEndEffect();
}

void Entity::free()
{
	string name = getName();
	TextureManager::instance().removeTexture(name);
}

Hitbox Entity::getRotatedHitbox(Hitbox& hitbox, float angle) const
{
	glm::vec2 upLeft = hitbox.min;
	glm::vec2 downRight = hitbox.max;
	glm::vec2 upRight(downRight.x, upLeft.y);
	glm::vec2 downLeft(upLeft.x, downRight.y);

	glm::vec2 r1 = rotatePoint(upLeft, pivotPoint, angle);
	glm::vec2 r2 = rotatePoint(upRight, pivotPoint, angle);
	glm::vec2 r3 = rotatePoint(downRight, pivotPoint, angle);
	glm::vec2 r4 = rotatePoint(downLeft, pivotPoint, angle);

	float minX = std::min({ r1.x, r2.x, r3.x, r4.x });
	float minY = std::min({ r1.y, r2.y, r3.y, r4.y });
	float maxX = std::max({ r1.x, r2.x, r3.x, r4.x });
	float maxY = std::max({ r1.y, r2.y, r3.y, r4.y });

	hitbox.min = glm::vec2(minX, minY);
	hitbox.max = glm::vec2(maxX, maxY);

	return hitbox;
}

glm::vec2 Entity::rotatePoint(const glm::vec2& point, const glm::vec2& pivotPoint, float angle) const
{
	float rad = glm::radians(angle);
	float cosA = cos(rad);
	float sinA = sin(rad);

	glm::vec2 rotated;
	rotated.x = pivotPoint.x + (point.x - pivotPoint.x) * cosA - (point.y - pivotPoint.y) * sinA;
	rotated.y = pivotPoint.y + (point.x - pivotPoint.x) * sinA + (point.y - pivotPoint.y) * cosA;

	return rotated;
}
