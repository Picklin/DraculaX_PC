#include "CoolIntro.h"
#include "TextureManager.h"
#include "Game.h"

void CoolIntro::initChild()
{
	Texture* bgTex = TextureManager::instance().getTexture("coolIntroBg");
	glm::ivec2 fullScreen(SCREEN_WIDTH, SCREEN_HEIGHT);
	glm::vec2 offset(0.125f, 0.2f);
	Texture* lvl0Tex = TextureManager::instance().getTexture("prologueTexs");
	Texture* boltsTex;
	if (TextureManager::instance().exists("bolts"))
	{
		boltsTex = TextureManager::instance().getTexture("bolts");
	}
	else
	{
		boltsTex = new Texture();
		boltsTex->loadFromFile("images/screens/bolts.png", TEXTURE_PIXEL_FORMAT_RGBA);
		TextureManager::instance().addTexture("bolts", boltsTex);
	}
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
	sp = Sprite::createSprite(glm::ivec2(64, 32), glm::vec2(0.125f, 0.125f), lvl0Tex, shader);
	sp->setNumberAnimations(1);
	sp->setAnimationSpeed(0, 15);
	sp->animatorX(0, 2, 0.25f, 0.125f, 0.71875f);
	sp->changeAnimation(0);
	Texture* skytex = new Texture();
	skytex->loadFromFile("images/levels/lvl0/sky.png", TEXTURE_PIXEL_FORMAT_RGBA);
	TextureManager::instance().addTexture("skyPrologue", skytex);
	Texture* treestex = new Texture();
	treestex->loadFromFile("images/levels/lvl0/trees_far.png", TEXTURE_PIXEL_FORMAT_RGBA);
	TextureManager::instance().addTexture("treesPrologue", treestex);
	introQuads[SKY] = TexturedQuad::createTexturedQuad(glm::vec2(0.f, 0.f), glm::vec2(8.f, 1.f), *skytex, *shader);
	introQuads[TREES_FAR] = TexturedQuad::createTexturedQuad(glm::vec2(0.f, 0.f), glm::vec2(16.f, 1.f), *treestex, *shader);
	introQuads[TREES_CLOSE] = TexturedQuad::createTexturedQuad(glm::vec2(0.f, 0.3125f), glm::vec2(1.f, 0.5f), *lvl0Tex, *shader);
	introQuads[CAR_TOP] = TexturedQuad::createTexturedQuad(glm::vec2(0.0625f, 0.25f), glm::vec2(0.125f, 0.3125f), *lvl0Tex, *shader);
	introQuads[CAR] = TexturedQuad::createTexturedQuad(glm::vec2(0.f, 0.5f), glm::vec2(0.40625f, 0.71875f), *lvl0Tex, *shader);
	introQuads[GRASS] = TexturedQuad::createTexturedQuad(glm::vec2(0.f, 0.84375f), glm::vec2(1.f, 1.f), *lvl0Tex, *shader);
	introSprites[WHEELS] = Sprite::createSprite(glm::ivec2(32), glm::vec2(0.0625f, 0.125f), lvl0Tex, shader);
	introSprites[WHEELS]->setNumberAnimations(1);
	introSprites[WHEELS]->setAnimationSpeed(0, 30);
	introSprites[WHEELS]->animatorX(0, 4, 0.f, 0.0625f, 0.71875f);
	introSprites[WHEELS]->changeAnimation(0);
	introSprites[HORSES] = Sprite::createSprite(glm::ivec2(128, 80), glm::vec2(0.25f, 0.3125f), lvl0Tex, shader);
	introSprites[HORSES]->setNumberAnimations(1);
	introSprites[HORSES]->setAnimationSpeed(0, 12);
	introSprites[HORSES]->animatorX(0, 3, 0.125f, 0.25f, 0.f);
	introSprites[HORSES]->animatorX(0, 2, 0.5f, 0.25f, 0.5f);
	introSprites[HORSES]->changeAnimation(0);
	introQuads[TREES_FAR]->setPosition(glm::vec2(0, 64));
	introQuads[TREES_CLOSE]->setPosition(glm::vec2(0, 80));
	introQuads[CAR_TOP]->setPosition(glm::vec2(16, 112));
	introQuads[CAR]->setPosition(glm::vec2(0, 128));
	introQuads[GRASS]->setPosition(glm::vec2(0, 224 - 40));
	introSprites[HORSES]->setPosition(glm::vec2(120, 120));
	sp->setPosition(glm::vec2(72, 134));
	bg.bg = sp;
	bg.time = 2.75f;
	bg.duration = 6.25f;
	bg.id = HORSES;
	film.push(bg);
}

void CoolIntro::filmUpdate(int deltaTime)
{
	int filmId = film.front().id;
	if (filmId == HORSES)
	{
		bgXScroll += 0.0125f;
		introSprites[WHEELS]->update(deltaTime);
		introSprites[HORSES]->update(deltaTime);
	}
}

void CoolIntro::render()
{
	if (renderBg)
	{
		int filmId = film.front().id;
		if (filmId == HORSES)
		{
			shader->setUniform1f("xOffset", bgXScroll * 0.25f);
			introQuads[SKY]->render();
			shader->setUniform1f("xOffset", bgXScroll);
			introQuads[TREES_FAR]->render();
			shader->setUniform1f("xOffset", bgXScroll * 0.5f);
			introQuads[TREES_CLOSE]->render();
			shader->setUniform1f("xOffset", 0.f);
			introQuads[CAR_TOP]->render();
			introQuads[CAR]->render();
			introSprites[WHEELS]->setPosition(glm::vec2(16, 168));
			introSprites[WHEELS]->render();
			introSprites[WHEELS]->setPosition(glm::vec2(78, 168));
			introSprites[WHEELS]->render();
			introSprites[HORSES]->render();
			shader->setUniform1f("xOffset", bgXScroll);
			introQuads[GRASS]->render();
			shader->setUniform1f("xOffset", 0.f);
			film.front().bg->render();
		}
		else film.front().bg->render();
	}
}

float CoolIntro::setEndTime() const
{
	return 64.f;
}
