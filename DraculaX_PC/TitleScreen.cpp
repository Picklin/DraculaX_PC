#include "TitleScreen.h"
#include "Game.h"
#include "TextureManager.h"
#include "SoundEngine.h"

#define VANISH_TIME 3.F
#define BOLT_FREQ 2.F
#define BOLT_DURATION 64	//milisecs

void TitleScreen::init(ShaderProgram& program)
{
	this->texProgram = &program;
	this->lang = Game::instance().getCurrentTxtLang();

	textures.resize(3);
	sprites.reserve(1);
	quads.reserve(3);

	textures[0].loadFromFile("images/screens/titleBG.png", TEXTURE_PIXEL_FORMAT_RGBA);
	textures[0].setMagFilter(GL_NEAREST);
	TextureManager::instance().addTexture("titleBG", &textures[0]);
	textures[1].loadFromFile("images/screens/titleOriginal.png", TEXTURE_PIXEL_FORMAT_RGBA);
	textures[1].setMagFilter(GL_NEAREST);
	TextureManager::instance().addTexture("titleOriginal", &textures[1]);
	textures[2].loadFromFile("images/screens/bolts.png", TEXTURE_PIXEL_FORMAT_RGBA);
	textures[2].setMagFilter(GL_NEAREST);
	TextureManager::instance().addTexture("bolts", &textures[2]);
	//background
	sprites.emplace_back(Sprite::createSprite(glm::ivec2(SCREEN_WIDTH, SCREEN_HEIGHT), glm::vec2(0.5f, 0.25f), &textures[0], texProgram));
	sprites[0]->setNumberAnimations(2);
	sprites[0]->setAnimationSpeed(0, 0);
	sprites[0]->addKeyframe(0, glm::vec2(0.f, 0.f));
	sprites[0]->setAnimationSpeed(1, 20);
	sprites[0]->addKeyframe(1, glm::vec2(0.5f, 0.f));
	sprites[0]->addKeyframe(1, glm::vec2(0.f, 0.0f));
	sprites[0]->addKeyframe(1, glm::vec2(0.5f, 0.f));
	sprites[0]->setTransition(1, 0);
	sprites[0]->changeAnimation(0);
	//title
	quads.emplace_back(TexturedQuad::createTexturedQuad(glm::vec2(0.f, 0.875f), glm::vec2(1.f, 1.f), textures[1], *texProgram));
	//bolts
	quads.emplace_back(TexturedQuad::createTexturedQuad(glm::vec2(0.25f, 0.f), glm::vec2(0.5f, 1.f), textures[2], *texProgram));
	quads.emplace_back(TexturedQuad::createTexturedQuad(glm::vec2(0.f, 0.f), glm::vec2(0.25f, 1.f), textures[2], *texProgram));

	quads[1]->setPosition(glm::vec2(145, 55));
	quads[2]->setPosition(glm::vec2(88, 15));

	//projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);
	//texProgram->setUniformMatrix4f("projection", projection);

	pushRunButton = Text::CreateLettersAndNumsText(program);
	pushRunButton.setColor(glm::vec4(252 / 255.f, 0.f, 0.f, 1.f));

	boltTimer = BOLT_FREQ;
	boltDuration = BOLT_DURATION;
	renderBigBolt = true;

	reset();
}

void TitleScreen::update(int deltaTime)
{
	boltTimer -= deltaTime / 1000.f;
	if (!startPressed && boltTimer <= 0)
	{
		if (sprites[0]->animation() != 1) sprites[0]->changeAnimation(1);
		boltDuration -= deltaTime;
		if (boltDuration < 0)
		{
			boltTimer = BOLT_FREQ;
			boltDuration = BOLT_DURATION;
			renderBigBolt = !renderBigBolt;
		}
	}
	if (!startPressed && Game::instance().getKey(GLFW_KEY_ENTER))
	{
		startPressed = true;
		SoundEngine::instance().playSFX(SoundEngine::START);
	}
	else if (startPressed)
	{
		timeBeforeStart -= deltaTime / 1000.f;
		if (timeBeforeStart <= 0)
		{
			Game::instance().start();
			reset();
		}
		else if (timeBeforeStart > VANISH_TIME) renderText = fmod(timeBeforeStart, .5f) < .25f;
	}
	sprites[0]->update(deltaTime);
}

void TitleScreen::render()
{
	//texProgram->setUniformMatrix4f("projection", projection); //solo cuando cambie la proyección
	if (timeBeforeStart > 1.f)
	{
		sprites[0]->render();
		if (!startPressed && boltTimer <= 0) quads[1 + renderBigBolt]->render();
		quads[0]->render();
		if (timeBeforeStart <= VANISH_TIME)
		{
			float alpha = timeBeforeStart - (VANISH_TIME - 1);
			sprites[0]->setAlpha(alpha);
			quads[0]->setAlpha(alpha+1.25f);
		}
		else if (renderText) pushRunButton.render(msg[lang], glm::vec2(128.f, 144.f));
	}
}

void TitleScreen::reset()
{
	timeBeforeStart = 5.f;
	startPressed = false;
	renderText = true;
}
