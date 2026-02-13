#include "Projectile.h"
#include "TextureManager.h"

void Projectile::init(const glm::ivec2& tileMapDispl, ShaderProgram& shaderProgram, const glm::vec2& dir)
{
	this->tileMapDispl = tileMapDispl;
	this->dir = dir;
	speed = setSpeed();
	endTimer = setEndTime();
	ended = false;
	string name = getSpritesheet();
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
	sprite = Sprite::createSprite(getQuadSize(), getSizeInSpritesheet(), spritesheet, &shaderProgram);
	setAnimations();
}

void Projectile::update(int deltaTime)
{	
	sprite->update(deltaTime);
	if (!ended)
	{
		position += speed * dir;
		setPosition(position);
		childUpdate(deltaTime);
	}
	else endTimer -= deltaTime / 1000.f;
}

void Projectile::render() 
{ 
	sprite->render(); 
} 

void Projectile::setPosition(const glm::vec2& pos) 
{ 
	this->position = pos; 
	sprite->setPosition(pos + glm::vec2(tileMapDispl)); 
} 

void Projectile::end() 
{ 
	ended = true; 
	endTimer = setEndTime();
	sprite->changeAnimation(setEndAnimation());
}

const glm::vec2& Projectile::getPosition() const
{
	return position; 
}

float Projectile::setEndTime() 
{ 
	return 0.f;
}

bool Projectile::isEnded() const 
{ 
	return ended; 
} 

bool Projectile::getsRemoved() const
{ 
	return endTimer <= 0; 
}