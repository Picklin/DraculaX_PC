#include "Intro.h"
#include "TextureManager.h"
#include "Game.h"
#include <iostream>

#define BOLT_DURATION 64	//milisecs

void Intro::init(ShaderProgram& shader)
{
	this->shader = &shader;
	Texture* introTexs1 = TextureManager::instance().getTexture("introTexs");
	blackScreen = TexturedQuad::createTexturedQuad(glm::vec2(0), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT), *TextureManager::instance().getTexture("pixel"), shader);
	//blackScreen = TexturedQuad::createTexturedQuad(glm::vec2(0), glm::vec2(0.2f, 0.2f), *introTexs1, shader);
	blackScreen->setColor(glm::vec3(0));
	blackScreen->setAlpha(0.f);
	//Sprite* testSp = Sprite::createSprite(glm::vec2(0.f, 0.f), glm::vec2(0.2f, 0.2f), introTexs1, &shader);
	glm::ivec2 fullScreen(SCREEN_WIDTH, SCREEN_HEIGHT);
	glm::vec2 offset(0.2f);
	bool arr = Game::instance().isArranged();
	coffin = Sprite::createSprite(fullScreen, offset, introTexs1, &shader);
	coffin->setNumberAnimations(2);
	coffin->setAnimationSpeed(0, 0);
	coffin->addKeyframe(0, glm::vec2(0.f, 0.2f));
	coffin->setAnimationSpeed(1, 0);
	coffin->addKeyframe(1, glm::vec2(0.f, 0.4f));
	coffin->changeAnimation(0);
	Texture* boltsTex = TextureManager::instance().getTexture("bolts");
	bolts[0] = TexturedQuad::createTexturedQuad(glm::vec2(0.5f, 0.f), glm::vec2(0.75f, 1.f), *boltsTex, shader);
	bolts[1] = TexturedQuad::createTexturedQuad(glm::vec2(0.75f, 0.f), glm::vec2(1.f, 1.f), *boltsTex, shader);
	bolts[0]->setPosition(glm::vec2(120, 0));
	bolts[1]->setPosition(glm::vec2(168, 0));
	Sprite* testSp = Sprite::createSprite(fullScreen, offset, introTexs1, &shader);
	testSp->setNumberAnimations(2);
	testSp->setAnimationSpeed(0, 0);
	testSp->addKeyframe(0, glm::vec2(0.f, 0.f));
	testSp->setAnimationSpeed(1, 20);
	testSp->addKeyframe(1, glm::vec2(0.2f, 0.f));
	testSp->addKeyframe(1, glm::vec2(0.f, 0.f));
	testSp->addKeyframe(1, glm::vec2(0.2f, 0.f));
	testSp->setTransition(1, 0);
	testSp->changeAnimation(0);
	Bg bg;
	bg.bg = testSp;
	bg.time = 11.75f;
	bg.duration = 8.25f + 0.45f * !arr;
	bg.id = CASTLE_OUTSIDE;
	film.push(bg);
	testSp = Sprite::createSprite(fullScreen, offset, introTexs1, &shader);
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
	testSp = Sprite::createSprite(fullScreen, offset, introTexs1, &shader);
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
	testSp = Sprite::createSprite(fullScreen, offset, introTexs1, &shader);
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
	testSp = Sprite::createSprite(glm::ivec2(SCREEN_WIDTH, SCREEN_HEIGHT*2), glm::vec2(0.2f,0.4f), introTexs1, &shader);
	testSp->setNumberAnimations(1);
	testSp->setAnimationSpeed(0, 0);
	testSp->addKeyframe(0, glm::vec2(0.4f, 0.2f));
	testSp->changeAnimation(0);
	testSp->setPosition(glm::vec2(0.f, -SCREEN_HEIGHT));
	bg.bg = testSp;
	bg.time = 55.25f;
	bg.duration = 2.75f;
	bg.alpha = 1.f;
	bg.id = STAB;
	film.push(bg);
	endTime = 104.f;
	renderBigBolt = false;
	boltDuration = BOLT_DURATION;
	stabVelocityFactor = 0.f;
	cameraY = -SCREEN_HEIGHT;
	shader.setUniformMatrix4f("projection", glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f));
}

void Intro::update(int deltaTime)
{
	timeElapsed += deltaTime / 1000.f;
	if (!script.empty())
	{
		if ((timeElapsed >= script.front().time) && (timeElapsed <= (script.front().time + script.front().duration)))
		{
			renderSubtitles = true;
		}
		else if (timeElapsed > (script.front().time + script.front().duration))
		{
			renderSubtitles = false;
			script.pop();
		}
	}
	if (!film.empty())
	{
		if ((timeElapsed >= film.front().time) && (timeElapsed <= (film.front().time + film.front().duration)))
		{
			film.front().bg->update(deltaTime);
			if ((film.front().id == CASTLE_OUTSIDE || film.front().id == CASTLE_INSIDE) && thunderCooldown <= 0.f)
			{
				if (film.front().bg->animation() != 1) film.front().bg->changeAnimation(1);
				boltDuration -= deltaTime;
				if (boltDuration < 0)
				{
					thunderCooldown = 5.f;
					boltDuration = BOLT_DURATION;
					renderBigBolt = !renderBigBolt;
				}
			}
			else if (film.front().id == SOLDIERS)
			{
				cameraX -= 1.f;
				film.front().bg->setPosition(glm::vec2(cameraX, 0.f));
			}
			else if (film.front().id == STAB)
			{
				stabVelocityFactor += deltaTime / 500.f;
				if (cameraY < 0) calcEaseIn(cameraY, -SCREEN_HEIGHT, 0, stabVelocityFactor);
				film.front().bg->setPosition(glm::vec2(0, cameraY));
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
		else if (timeElapsed > (film.front().time + film.front().duration))
		{
			renderBg = false;
			film.pop();
		}
	}
	if (Game::instance().getKey(GLFW_KEY_ENTER))
	{
		timeElapsed = endTime;
		Game::instance().keyReleased(GLFW_KEY_ENTER);
	}
}

void Intro::render()
{
	if (renderBg)
	{
		if (film.front().id == CASTLE_OUTSIDE && thunderCooldown <= 0)
		{
			film.front().bg->render();
			bolts[renderBigBolt]->render();
		}
		else if (film.front().id == GIRL_ON_COFFIN)
		{
			coffin->render();
			film.front().bg->render();
		}
		else if (film.front().id == SOLDIERS)
		{
			film.front().bg->render();
			film.front().bg->setPosition(glm::vec2(cameraX + SCREEN_WIDTH + 128, 0));
			shader->setUniform1f("frameWidth", 0.2f);
			shader->setUniform1i("flip", true);
			film.front().bg->render();
			shader->setUniform1i("flip", false);
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