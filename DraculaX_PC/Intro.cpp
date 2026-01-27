#include "Intro.h"
#include "TextureManager.h"
#include "Game.h"
#include <iostream>

#define BOLT_DURATION 64	//milisecs

void Intro::initChild()
{
	Texture* introTexs1 = TextureManager::instance().getTexture("introTexs");
	blackScreen->setAlpha(0.f);
	//Sprite* testSp = Sprite::createSprite(glm::vec2(0.f, 0.f), glm::vec2(0.2f, 0.2f), introTexs1, &shader);
	glm::ivec2 fullScreen(SCREEN_WIDTH, SCREEN_HEIGHT);
	glm::vec2 offset(0.2f);
	bool arr = Game::instance().isArranged();
	coffin = TexturedQuad::createTexturedQuad(glm::vec2(0.f, 0.2f), glm::vec2(0.2f, 0.4f), *introTexs1, *shader);
	Texture* boltsTex = TextureManager::instance().getTexture("bolts");
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
	lights = Sprite::createSprite(glm::ivec2(64, 128), glm::vec2(0.25f, 0.5f), TextureManager::instance().getTexture("introTexs2"), shader);
	lights->setNumberAnimations(2);
	lights->setAnimationSpeed(0, 8);
	lights->addKeyframe(0, glm::vec2(0.75f, 0.5f));
	lights->setAnimationSpeed(1, 0);
	lights->addKeyframe(1, glm::vec2(0.5f, 0.5f));
	lights->setTransition(0, 1);
	lights->changeAnimation(0);
	dracula = Sprite::createSprite(fullScreen, glm::vec2(1.f, 0.25f), TextureManager::instance().getTexture("dracula_intro"), shader);
	dracula->setNumberAnimations(3);
	dracula->setAnimationSpeed(0, 0);
	dracula->addKeyframe(0, glm::vec2(0.f));
	dracula->setAnimationSpeed(1, 30);
	dracula->animatorY(1, 2, 0.25f, 0.25f, 0.f);
	dracula->setAnimationSpeed(2, 0);
	dracula->addKeyframe(2, glm::vec2(0.f, 0.75f));
	dracula->changeAnimation(0);
	dracula->setPosition(glm::vec2(0, SCREEN_HEIGHT / 2));
	bat = Sprite::createSprite(fullScreen, glm::vec2(1.f, 0.125f), TextureManager::instance().getTexture("bat_intro"), shader);
	bat->setNumberAnimations(2);
	bat->setAnimationSpeed(0, 15);
	bat->animatorY(0, 6, 0.f, 0.125f, 0.f);
	bat->setAnimationSpeed(1, 15);
	bat->animatorY(1, 2, 0.75f, 0.125f, 0.f);
	bat->setTransition(0, 1);
	bat->changeAnimation(0);
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
	Texture* titleBgTex = TextureManager::instance().getTexture("titleBG");
	titleBg = Sprite::createSprite(fullScreen, glm::vec2(0.5f, 0.25f), titleBgTex, shader);
	titleBg->setNumberAnimations(4);
	titleBg->setAnimationSpeed(0, 10);
	titleBg->animatorY(0, 4, 0.f, 0.25f, 0.5f);
	titleBg->setAnimationSpeed(1, 2);
	titleBg->addKeyframe(1, glm::vec2(0.5f, 0.75f));
	titleBg->setAnimationSpeed(2, 0);
	titleBg->addKeyframe(2, glm::vec2(0.f, 0.f));
	titleBg->setAnimationSpeed(3, 20);
	titleBg->addKeyframe(3, glm::vec2(0.5f, 0.f));
	titleBg->addKeyframe(3, glm::vec2(0.f, 0.f));
	titleBg->addKeyframe(3, glm::vec2(0.5f, 0.f));
	titleBg->setTransition(0, 1);
	titleBg->setTransition(1, 2);
	titleBg->setTransition(3, 2);
	titleBg->changeAnimation(2);
	title = Sprite::createSprite(fullScreen, glm::vec2(1.f, 0.125f), TextureManager::instance().getTexture("titleOriginal"), shader);
	title->setNumberAnimations(4);
	title->setAnimationSpeed(0, 5);
	title->animatorY(0, 2, 0.f, 0.125f, 0.f);
	title->setAnimationSpeed(1, 0);
	title->addKeyframe(1, glm::vec2(0.f, 0.25f));
	title->setAnimationSpeed(2, 3);
	title->animatorY(2, 3, 0.375f, 0.125f, 0.f);
	title->setAnimationSpeed(3, 0);
	title->addKeyframe(3, glm::vec2(0.f, 0.75f));
	title->setTransition(0, 1);
	title->setTransition(2, 3);
	title->changeAnimation(0);
	testSp = Sprite::createSprite(glm::ivec2(SCREEN_WIDTH, 672), glm::vec2(0.5f, 0.75f), titleBgTex, shader);
	testSp->setNumberAnimations(1);
	testSp->setAnimationSpeed(0, 0);
	testSp->addKeyframe(0, glm::vec2(0.f, 0.f));
	testSp->changeAnimation(0);
	testSp->setPosition(glm::vec2(0, -391));
	bg.bg = testSp;
	bg.time = 86.f;
	bg.duration = 16.f;
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
		lights->update(deltaTime);
		lights->setPosition(glm::vec2(184.f, 0.f));
	}
	else if (filmId == DRACULA_APPEAR)
	{
		int timeMilisecs = int(timeElapsed * 1000);
		float colorValue = 1 - (((timeMilisecs / (deltaTime * 2)) % 2 == 0) * 0.5f);
		film.front().bg->setColor(glm::vec4(glm::vec3(colorValue), 1.f));
		if (cameraY > 0)
		{
			cameraY--;
			dracula->setPosition(glm::vec2(0, cameraY));
		}
		else
		{
			dracula->update(deltaTime);
			if (dracula->animation() != 1 && timeElapsed < 65)
			{
				dracula->changeAnimation(1);
			}
			else if (timeElapsed >= 65.5 && dracula->animation() != 2)
			{
				dracula->changeAnimation(2);
			}
			else if (timeElapsed >= 66.75f)
			{
				cameraY -= 1.25f;
				dracula->setPosition(glm::vec2(0, cameraY+0.25f));
				film.front().bg->setPosition(glm::vec2(0, -SCREEN_HEIGHT * 2 - cameraY));
			}
		}
	}
	else if (filmId == CASTLEVANIA_CLOSE && timeElapsed < 74.5f)
	{
		bat->update(deltaTime);
		if (timeElapsed >= 74)
		{
			batPosition--;
			bat->setPosition(glm::vec2(0, batPosition));
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
			titleBg->setPosition(glm::vec2(0, cameraY));
			thunderCooldown = 1.f;
		}
		else
		{
			titleBg->update(deltaTime);
			if (timeElapsed >= 97)
			{
				if (!titleShowed)
				{
					titleShowed = true;
					titleBg->changeAnimation(0);
				}
				if (timeElapsed >= 98) title->update(deltaTime);
				if (timeElapsed >= 99.75f && title->animation() < 2) title->changeAnimation(2);
			}
			if (thunderCooldown <= 0)
			{
				if (titleBg->animation() != 3) titleBg->changeAnimation(3);
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
	float alpha = film.front().alpha;
	if (alpha < 1.f)
	{
		alpha += deltaTime / 1000.f;
		blackScreen->setAlpha(1-alpha);
		film.front().alpha = alpha;
	}
	thunderCooldown -= deltaTime / 1000.f;
	renderBg = true;
}

float Intro::setEndTime() const
{
	return 102.0f;
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
			coffin->render();
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
				coffin->render();
				film.front().bg->render();
			}
			else
			{
				film.front().bg->render();
				lights->render();
				lights->setPosition(glm::vec2(9.f, 0.f));
				shader->setUniform1f("frameWidth", 0.25f);
				shader->setUniform1i("flip", true);
				lights->render();
				shader->setUniform1i("flip", false);
			}
			cameraY = SCREEN_HEIGHT / 2;
		}
		else if (filmId == DRACULA_APPEAR)
		{
			film.front().bg->render();
			dracula->render();
		}
		else if (filmId == CASTLEVANIA_CLOSE)
		{
			film.front().bg->render();
			if (timeElapsed < 74.5f) bat->render();
		}
		else if (filmId == CASTLEVANIA_FAR)
		{
			//cout << timeElapsed << endl;
			if (cameraY >= 0)
			{
				titleBg->render();
				if (thunderCooldown <= 0) bolts[2 + renderBigBolt]->render();
				if (timeElapsed >= 98) title->render();
			}
			else film.front().bg->render();
		}
		else film.front().bg->render();
		if (film.front().alpha < 1) blackScreen->render();
	}
	if (renderSubtitles) dialogueTxt.render(script.front().line, glm::vec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 32));
}

void Intro::calcEaseIn(float& valToInc, float startVal, float targetVal, float factor)
{
	factor = factor * factor;
	valToInc = startVal + (targetVal - startVal) * factor;
}