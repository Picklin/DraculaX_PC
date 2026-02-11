#include "Intro.h"
#include "TextureManager.h"
#include "Game.h"
#include <iostream>

#define BOLT_DURATION 64	//milisecs

void Intro::initChild()
{
	cinematicQuads.resize(COUNTQUADS);
	cinematicSprites.resize(COUNTSPRITES);
	Texture* introTexs1 = TextureManager::instance().getTexture("introTexs");
	//Sprite* testSp = Sprite::createSprite(glm::vec2(0.f, 0.f), glm::vec2(0.2f, 0.2f), introTexs1, &shader);
	glm::ivec2 fullScreen(SCREEN_WIDTH, SCREEN_HEIGHT);
	glm::vec2 offset(0.2f);
	bool arr = Game::instance().isArranged();
	cinematicQuads[COFFIN] = TexturedQuad::createTexturedQuad(glm::vec2(0.f, 0.2f), glm::vec2(0.2f, 0.4f), *introTexs1, *shader);
	Texture* boltsTex = new Texture();
	boltsTex->loadFromFile("images/screens/bolts.png", TEXTURE_PIXEL_FORMAT_RGBA);
	TextureManager::instance().addTexture("bolts", boltsTex);
	bolts[0] = TexturedQuad::createTexturedQuad(glm::vec2(0.5f, 0.f), glm::vec2(0.75f, 1.f), *boltsTex, *shader);
	bolts[1] = TexturedQuad::createTexturedQuad(glm::vec2(0.75f, 0.f), glm::vec2(1.f, 1.f), *boltsTex, *shader);
	bolts[2] = TexturedQuad::createTexturedQuad(glm::vec2(0.f, 0.f), glm::vec2(0.25f, 1.f), *boltsTex, *shader);
	bolts[3] = TexturedQuad::createTexturedQuad(glm::vec2(0.25f, 0.f), glm::vec2(0.5f, 1.f), *boltsTex, *shader);
	bolts[0]->setPosition(glm::vec2(120, 0));
	bolts[1]->setPosition(glm::vec2(168, 0));
	bolts[2]->setPosition(glm::vec2(88, 15));
	bolts[3]->setPosition(glm::vec2(145, 55));
	blood[0] = TexturedQuad::createTexturedQuad(glm::vec2(0.6f, 0.2f), glm::vec2(0.8f, 0.4f), *introTexs1, *shader);
	blood[1] = TexturedQuad::createTexturedQuad(glm::vec2(0.6f, 0.4f), glm::vec2(0.8f, 0.6f), *introTexs1, *shader);
	cinematicSprites[LIGHTS] = Sprite::createSprite(glm::ivec2(64, 128), glm::vec2(0.25f, 0.5f), TextureManager::instance().getTexture("introTexs2"), shader);
	cinematicSprites[LIGHTS]->setNumberAnimations(2);
	cinematicSprites[LIGHTS]->setAnimationSpeed(0, 8);
	cinematicSprites[LIGHTS]->addKeyframe(0, glm::vec2(0.75f, 0.5f));
	cinematicSprites[LIGHTS]->setAnimationSpeed(1, 0);
	cinematicSprites[LIGHTS]->addKeyframe(1, glm::vec2(0.5f, 0.5f));
	cinematicSprites[LIGHTS]->setTransition(0, 1);
	cinematicSprites[LIGHTS]->changeAnimation(0);
	cinematicSprites[DRACULA] = Sprite::createSprite(fullScreen, glm::vec2(1.f, 0.25f), TextureManager::instance().getTexture("dracula_intro"), shader);
	cinematicSprites[DRACULA]->setNumberAnimations(3);
	cinematicSprites[DRACULA]->setAnimationSpeed(0, 0);
	cinematicSprites[DRACULA]->addKeyframe(0, glm::vec2(0.f));
	cinematicSprites[DRACULA]->setAnimationSpeed(1, 30);
	cinematicSprites[DRACULA]->animatorY(1, 2, 0.25f, 0.25f, 0.f);
	cinematicSprites[DRACULA]->setAnimationSpeed(2, 0);
	cinematicSprites[DRACULA]->addKeyframe(2, glm::vec2(0.f, 0.75f));
	cinematicSprites[DRACULA]->changeAnimation(0);
	cinematicSprites[DRACULA]->setPosition(glm::vec2(0, SCREEN_HEIGHT / 2));
	cinematicSprites[BAT] = Sprite::createSprite(fullScreen, glm::vec2(1.f, 0.125f), TextureManager::instance().getTexture("bat_intro"), shader);
	cinematicSprites[BAT]->setNumberAnimations(2);
	cinematicSprites[BAT]->setAnimationSpeed(0, 15);
	cinematicSprites[BAT]->animatorY(0, 6, 0.f, 0.125f, 0.f);
	cinematicSprites[BAT]->setAnimationSpeed(1, 15);
	cinematicSprites[BAT]->animatorY(1, 2, 0.75f, 0.125f, 0.f);
	cinematicSprites[BAT]->setTransition(0, 1);
	cinematicSprites[BAT]->changeAnimation(0);
	Sprite* testSp = Sprite::createSprite(fullScreen, offset, introTexs1, shader);
	testSp->setNumberAnimations(3);
	testSp->setAnimationSpeed(0, 0);
	testSp->addKeyframe(0, glm::vec2(0.f, 0.f));
	testSp->setAnimationSpeed(1, 20);
	testSp->addKeyframe(1, glm::vec2(0.2f, 0.f));
	testSp->addKeyframe(1, glm::vec2(0.f, 0.f));
	testSp->addKeyframe(1, glm::vec2(0.2f, 0.f));
	testSp->setAnimationSpeed(2, 20);
	testSp->addKeyframe(2, glm::vec2(0.2f, 0.f));
	testSp->addKeyframe(2, glm::vec2(0.f, 0.f));
	testSp->addKeyframe(2, glm::vec2(0.2f, 0.f));
	testSp->addKeyframe(2, glm::vec2(0.f, 0.f));
	testSp->addKeyframe(2, glm::vec2(0.2f, 0.f));
	testSp->addKeyframe(2, glm::vec2(0.f, 0.f));
	testSp->addKeyframe(2, glm::vec2(0.2f, 0.f));
	testSp->setTransition(1, 0);
	testSp->setTransition(2, 0);
	testSp->changeAnimation(0);
	Bg bg;
	bg.bg = testSp;
	bg.time = 11.75f;
	bg.duration = 8.25f + 0.45f * !arr;
	bg.alpha = 1.f;
	bg.id = CASTLE_OUTSIDE;
	film.push(bg);
	testSp = Sprite::createSprite(fullScreen, offset, introTexs1, shader);
	testSp->setNumberAnimations(2);
	testSp->setAnimationSpeed(0, 0);
	testSp->addKeyframe(0, glm::vec2(0.4f, 0.f));
	testSp->setAnimationSpeed(1, 20);
	testSp->addKeyframe(1, glm::vec2(0.6f, 0.f));
	testSp->addKeyframe(1, glm::vec2(0.4f, 0.f));
	testSp->addKeyframe(1, glm::vec2(0.6f, 0.f));
	testSp->setTransition(1, 0);
	testSp->changeAnimation(0);
	bg.bg = testSp;
	bg.time = 21.5f;
	bg.duration = 16.25f + 0.5f * !arr;
	bg.alpha = 0.f;
	bg.id = CASTLE_INSIDE;
	film.push(bg);
	testSp = Sprite::createSprite(fullScreen, offset, introTexs1, shader);
	testSp->setNumberAnimations(1);
	testSp->setAnimationSpeed(0, 0);
	testSp->addKeyframe(0, glm::vec2(0.f, 0.6f));
	testSp->changeAnimation(0);
	bg.bg = testSp;
	bg.time = 39.5f;
	bg.duration = 9.f;
	bg.alpha = 0.f;
	bg.id = GIRL_ON_COFFIN;
	film.push(bg);
	testSp = Sprite::createSprite(fullScreen, offset, introTexs1, shader);
	testSp->setNumberAnimations(1);
	testSp->setAnimationSpeed(0, 0);
	testSp->addKeyframe(0, glm::vec2(0.2f, 0.2f));
	testSp->changeAnimation(0);
	bg.bg = testSp;
	bg.time = 47.5f;
	bg.duration = 6.75f;
	bg.alpha = 0.f;
	bg.id = SOLDIERS;
	film.push(bg);
	testSp = Sprite::createSprite(glm::ivec2(SCREEN_WIDTH, SCREEN_HEIGHT*2), glm::vec2(0.2f,0.4f), introTexs1, shader);
	testSp->setNumberAnimations(1);
	testSp->setAnimationSpeed(0, 0);
	testSp->addKeyframe(0, glm::vec2(0.4f, 0.2f));
	testSp->changeAnimation(0);
	testSp->setPosition(glm::vec2(0.f, -SCREEN_HEIGHT));
	bg.bg = testSp;
	bg.time = 55.25f + 0.25f * !arr;
	bg.duration = 2.75f;
	bg.alpha = 1.f;
	bg.id = STAB;
	film.push(bg);
	testSp = Sprite::createSprite(fullScreen, offset, introTexs1, shader);
	testSp->setNumberAnimations(2);
	testSp->setAnimationSpeed(0, 15);
	testSp->animatorX(0, 4, 0.2f, 0.2f, 0.6f);
	testSp->animatorX(0, 5, 0.0f, 0.2f, 0.8f);
	testSp->setAnimationSpeed(1, 0);
	testSp->addKeyframe(1, glm::vec2(0.f, 0.4f));
	testSp->setTransition(0, 1);
	testSp->changeAnimation(0);
	bg.bg = testSp;
	bg.time = 57.5f;
	bg.duration = 4.f;
	bg.alpha = 1.f;
	bg.id = COFFIN_BREAK;
	film.push(bg);
	testSp = Sprite::createSprite(glm::ivec2(SCREEN_WIDTH, SCREEN_HEIGHT * 3), glm::vec2(0.2f, 0.6f), introTexs1, shader);
	testSp->setNumberAnimations(1);
	testSp->setAnimationSpeed(0, 0);
	testSp->addKeyframe(0, glm::vec2(0.8f, 0.f));
	testSp->changeAnimation(0);
	testSp->setPosition(glm::vec2(0, -SCREEN_HEIGHT * 2));
	bg.bg = testSp;
	bg.time = 63.f;
	bg.duration = 9.f;
	bg.alpha = 1.f;
	bg.id = DRACULA_APPEAR;
	film.push(bg);
	film.push(bg);
	testSp = Sprite::createSprite(fullScreen, offset, introTexs1, shader);
	testSp->setNumberAnimations(1);
	testSp->setAnimationSpeed(0, 0);
	testSp->addKeyframe(0, glm::vec2(0.2f, 0.4f));
	testSp->changeAnimation(0);
	bg.bg = testSp;
	bg.time = 73.f;
	bg.duration = 9.5f + 0.5f * !arr;
	bg.alpha = 1.f;
	bg.id = CASTLEVANIA_CLOSE;
	film.push(bg);
	Texture* titleBgTex = new Texture();
	titleBgTex->loadFromFile("images/screens/titleBG.png", TEXTURE_PIXEL_FORMAT_RGBA);
	TextureManager::instance().addTexture("titleBG", titleBgTex);
	Texture* titleTex = new Texture();
	titleTex->loadFromFile("images/screens/titleOriginal.png", TEXTURE_PIXEL_FORMAT_RGBA);
	TextureManager::instance().addTexture("titleOriginal", titleTex);
	cinematicSprites[TITLE_BG] = Sprite::createSprite(fullScreen, glm::vec2(0.5f, 0.25f), titleBgTex, shader);
	cinematicSprites[TITLE_BG]->setNumberAnimations(4);
	cinematicSprites[TITLE_BG]->setAnimationSpeed(0, 10);
	cinematicSprites[TITLE_BG]->animatorY(0, 3, 0.f, 0.25f, 0.5f);
	cinematicSprites[TITLE_BG]->setAnimationSpeed(1, 1);
	cinematicSprites[TITLE_BG]->addKeyframe(1, glm::vec2(0.5f, 0.75f));
	cinematicSprites[TITLE_BG]->setAnimationSpeed(2, 0);
	cinematicSprites[TITLE_BG]->addKeyframe(2, glm::vec2(0.f, 0.f));
	cinematicSprites[TITLE_BG]->setAnimationSpeed(3, 20);
	cinematicSprites[TITLE_BG]->addKeyframe(3, glm::vec2(0.5f, 0.f));
	cinematicSprites[TITLE_BG]->addKeyframe(3, glm::vec2(0.f, 0.f));
	cinematicSprites[TITLE_BG]->addKeyframe(3, glm::vec2(0.5f, 0.f));
	cinematicSprites[TITLE_BG]->setTransition(0, 1);
	cinematicSprites[TITLE_BG]->setTransition(1, 2);
	cinematicSprites[TITLE_BG]->setTransition(3, 2);
	cinematicSprites[TITLE_BG]->changeAnimation(2);
	cinematicSprites[TITLE] = Sprite::createSprite(fullScreen, glm::vec2(1.f, 0.125f), TextureManager::instance().getTexture("titleOriginal"), shader);
	cinematicSprites[TITLE]->setNumberAnimations(4);
	cinematicSprites[TITLE]->setAnimationSpeed(0, 5);
	cinematicSprites[TITLE]->animatorY(0, 2, 0.f, 0.125f, 0.f);
	cinematicSprites[TITLE]->setAnimationSpeed(1, 0);
	cinematicSprites[TITLE]->addKeyframe(1, glm::vec2(0.f, 0.25f));
	cinematicSprites[TITLE]->setAnimationSpeed(2, 3);
	cinematicSprites[TITLE]->animatorY(2, 3, 0.375f, 0.125f, 0.f);
	cinematicSprites[TITLE]->setAnimationSpeed(3, 0);
	cinematicSprites[TITLE]->addKeyframe(3, glm::vec2(0.f, 0.75f));
	cinematicSprites[TITLE]->setTransition(0, 1);
	cinematicSprites[TITLE]->setTransition(2, 3);
	cinematicSprites[TITLE]->changeAnimation(0);
	testSp = Sprite::createSprite(glm::ivec2(SCREEN_WIDTH, 672), glm::vec2(0.5f, 0.75f), titleBgTex, shader);
	testSp->setNumberAnimations(1);
	testSp->setAnimationSpeed(0, 0);
	testSp->addKeyframe(0, glm::vec2(0.f, 0.f));
	testSp->changeAnimation(0);
	testSp->setPosition(glm::vec2(0, -391));
	bg.bg = testSp;
	bg.time = 86.f;
	bg.duration = 17.f;
	bg.alpha = 0.f;
	bg.id = CASTLEVANIA_FAR;
	film.push(bg);
	renderBigBolt = false;
	renderBlood = false;
	boltDuration = BOLT_DURATION;
	stabVelocityFactor = 0.f;
	cameraY = -SCREEN_HEIGHT;
}

void Intro::filmUpdate(int deltaTime)
{
	int filmId = film.front().id;
	if ((filmId == CASTLE_OUTSIDE || filmId == CASTLE_INSIDE) && thunderCooldown <= 0.f)
	{
		if (film.front().bg->animation() != 2 && timeElapsed < 12) film.front().bg->changeAnimation(2);
		else if (film.front().bg->animation() != 1) film.front().bg->changeAnimation(1);
		boltDuration -= deltaTime;
		if (boltDuration < 0)
		{
			thunderCooldown = 5.f;
			boltDuration = BOLT_DURATION;
			renderBigBolt = !renderBigBolt;
		}
	}
	else if (filmId == SOLDIERS)
	{
		cameraX -= 1.f;
		film.front().bg->setPosition(glm::vec2(cameraX, 0.f));
	}
	else if (filmId == STAB)
	{
		stabVelocityFactor += deltaTime / 500.f;
		if (cameraY < 0 && !renderBlood) calcEaseIn(cameraY, -SCREEN_HEIGHT, 0, stabVelocityFactor);
		if (cameraY > 0 && !renderBlood)
		{
			cameraY = 0.f;
			renderBlood = true;
		}
		film.front().bg->setPosition(glm::vec2(0, cameraY));
	}
	else if (filmId == COFFIN_BREAK && film.front().bg->animation() != 0)
	{
		cinematicSprites[LIGHTS]->update(deltaTime);
		cinematicSprites[LIGHTS]->setPosition(glm::vec2(184.f, 0.f));
	}
	else if (filmId == DRACULA_APPEAR)
	{
		int timeMilisecs = int(timeElapsed * 1000);
		float colorValue = 1 - (((timeMilisecs / (deltaTime * 2)) % 2 == 0) * 0.5f);
		film.front().bg->setColor(glm::vec3(colorValue));
		if (cameraY > 0)
		{
			cameraY--;
			cinematicSprites[DRACULA]->setPosition(glm::vec2(0, cameraY));
		}
		else
		{
			cinematicSprites[DRACULA]->update(deltaTime);
			if (cinematicSprites[DRACULA]->animation() != 1 && timeElapsed < 65)
			{
				cinematicSprites[DRACULA]->changeAnimation(1);
			}
			else if (timeElapsed >= 65.5 && cinematicSprites[DRACULA]->animation() != 2)
			{
				cinematicSprites[DRACULA]->changeAnimation(2);
			}
			else if (timeElapsed >= 66.75f)
			{
				cameraY -= 1.25f;
				cinematicSprites[DRACULA]->setPosition(glm::vec2(0, cameraY+0.25f));
				film.front().bg->setPosition(glm::vec2(0, -SCREEN_HEIGHT * 2 - cameraY));
			}
		}
	}
	else if (filmId == CASTLEVANIA_CLOSE && timeElapsed < 74.5f)
	{
		cinematicSprites[BAT]->update(deltaTime);
		if (timeElapsed >= 74)
		{
			batPosition--;
			cinematicSprites[BAT]->setPosition(glm::vec2(0, batPosition));
		}
		cameraY = -391.f;
	}
	else if (filmId == CASTLEVANIA_FAR)
	{
		if (cameraY < 0)
		{
			cameraY += 0.75f;
			if (cameraY > 0) cameraY = 0.f;
			film.front().bg->setPosition(glm::vec2(0, cameraY));
			cinematicSprites[TITLE_BG]->setPosition(glm::vec2(0, cameraY));
			thunderCooldown = 1.f;
		}
		else
		{
			cinematicSprites[TITLE_BG]->update(deltaTime);
			if (timeElapsed >= 97)
			{
				if (!titleShowed)
				{
					titleShowed = true;
					cinematicSprites[TITLE_BG]->changeAnimation(0);
				}
				if (timeElapsed >= 98) cinematicSprites[TITLE]->update(deltaTime);
				if (timeElapsed >= 99.75f && cinematicSprites[TITLE]->animation() < 2) cinematicSprites[TITLE]->changeAnimation(2);
			}
			if (thunderCooldown <= 0 && !titleShowed)
			{
				if (cinematicSprites[TITLE_BG]->animation() != 3) cinematicSprites[TITLE_BG]->changeAnimation(3);
				boltDuration -= deltaTime;
				if (boltDuration < 0)
				{
					thunderCooldown = 2.f;
					boltDuration = BOLT_DURATION;
					renderBigBolt = !renderBigBolt;
				}
			}
		}
	}
	thunderCooldown -= deltaTime / 1000.f;
	renderBg = true;
}

float Intro::setEndTime() const
{
	return 103.0f;
}

void Intro::render()
{
	if (renderBg)
	{
		int filmId = film.front().id;
		if (filmId == CASTLE_OUTSIDE && thunderCooldown <= 0)
		{
			film.front().bg->render();
			bolts[renderBigBolt]->render();
		}
		else if (filmId == GIRL_ON_COFFIN)
		{
			cinematicQuads[COFFIN]->render();
			film.front().bg->render();
		}
		else if (filmId == SOLDIERS)
		{
			film.front().bg->render();
			film.front().bg->setPosition(glm::vec2(cameraX + SCREEN_WIDTH + 128, 0));
			shader->setUniform1f("frameWidth", 0.2f);
			shader->setUniform1i("flip", true);
			film.front().bg->render();
			shader->setUniform1i("flip", false);
		}
		else if (filmId == STAB)
		{
			film.front().bg->render();
			if (renderBlood)
			{
				if (timeElapsed < 55.80f) blood[0]->render();
				else blood[1]->render();
			}
		}
		else if (filmId == COFFIN_BREAK)
		{
			if (film.front().bg->animation() == 0)
			{
				cinematicQuads[COFFIN]->render();
				film.front().bg->render();
			}
			else
			{
				film.front().bg->render();
				cinematicSprites[LIGHTS]->render();
				cinematicSprites[LIGHTS]->setPosition(glm::vec2(9.f, 0.f));
				shader->setUniform1f("frameWidth", 0.25f);
				shader->setUniform1i("flip", true);
				cinematicSprites[LIGHTS]->render();
				shader->setUniform1i("flip", false);
			}
			cameraY = SCREEN_HEIGHT / 2;
		}
		else if (filmId == DRACULA_APPEAR)
		{
			film.front().bg->render();
			cinematicSprites[DRACULA]->render();
		}
		else if (filmId == CASTLEVANIA_CLOSE)
		{
			film.front().bg->render();
			if (timeElapsed < 74.5f) cinematicSprites[BAT]->render();
		}
		else if (filmId == CASTLEVANIA_FAR)
		{
			//cout << timeElapsed << endl;
			if (cameraY >= 0)
			{
				cinematicSprites[TITLE_BG]->render();
				if (thunderCooldown <= 0 && !titleShowed) bolts[2 + renderBigBolt]->render();
				if (timeElapsed >= 98) cinematicSprites[TITLE]->render();
			}
			else film.front().bg->render();
		}
		else film.front().bg->render();
		if (film.front().alpha < 1) blackScreen->render();
	}
	if (renderSubtitles) dialogueTxt->render(script.front().line, glm::vec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 32));
}

void Intro::calcEaseIn(float& valToInc, float startVal, float targetVal, float factor)
{
	factor = factor * factor;
	valToInc = startVal + (targetVal - startVal) * factor;
}