#include "CoolIntro.h"
#include "TextureManager.h"
#include "Game.h"

void CoolIntro::initChild()
{
	Texture* bgTex = TextureManager::instance().getTexture("coolIntroBg");
	glm::ivec2 fullScreen(SCREEN_WIDTH, SCREEN_HEIGHT);
	glm::vec2 offset(0.125f, 0.2f); 
	Texture* boltsTex = new Texture();
	boltsTex->loadFromFile("images/screens/bolts.png", TEXTURE_PIXEL_FORMAT_RGBA);
	TextureManager::instance().addTexture("bolts", boltsTex);
	bolts[0] = TexturedQuad::createTexturedQuad(glm::vec2(0.f, 0.f), glm::vec2(0.25f, 1.f), *boltsTex, *shader);
	bolts[1] = TexturedQuad::createTexturedQuad(glm::vec2(0.25f, 0.f), glm::vec2(0.5f, 1.f), *boltsTex, *shader);
	bolts[0]->setPosition(glm::vec2(88, 15));
	bolts[1]->setPosition(glm::vec2(145, 55));
	Sprite* sp = Sprite::createSprite(fullScreen, offset, bgTex, shader);
	sp->setNumberAnimations(1);
	sp->setAnimationSpeed(0, 0);
	sp->addKeyframe(0, glm::vec2(0.f, 0.f));
	sp->changeAnimation(0);
	Bg bg;
	bg.bg = sp;
	bg.time = 0.f;
	bg.duration = 2.75f;
	bg.alpha = 1.f;
	bg.id = MAP;
	film.push(bg);
}

void CoolIntro::filmUpdate(int deltaTime)
{
	int filmId = film.front().id;
}

void CoolIntro::render()
{
	if (renderBg) film.front().bg->render();
}

float CoolIntro::setEndTime() const
{
	return 64.f;
}
