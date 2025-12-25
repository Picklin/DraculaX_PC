#include "TitleScreen.h"
#include "Game.h"
#include "TextureManager.h"
#include "SoundEngine.h"

void TitleScreen::init(ShaderProgram& program)
{
	this->texProgram = &program;
	
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
	sprites.emplace_back(Sprite::createSprite(glm::ivec2(SCREEN_WIDTH, SCREEN_HEIGHT), glm::vec2(1.f, 0.2f), &textures[0], texProgram));
	//title
	quads.emplace_back(TexturedQuad::createTexturedQuad(glm::vec2(0.f, 0.875f), glm::vec2(1.f, 1.f), textures[1], *texProgram));
	//bolts
	quads.emplace_back(TexturedQuad::createTexturedQuad(glm::vec2(0.f, 0.f), glm::vec2(0.5f, 1.f), textures[2], *texProgram));
	quads.emplace_back(TexturedQuad::createTexturedQuad(glm::vec2(0.5f, 0.f), glm::vec2(1.f, 1.f), textures[2], *texProgram));

	projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);
	texProgram->setUniformMatrix4f("projection", projection);

	pushRunButton.init(*texProgram, "images/fonts/Letters&Nums.png", glm::ivec2(8, 8), 40);
	pushRunButton.setColor(glm::vec4(252 / 255.f, 0.f, 0.f, 1.f));

	reset();
}

void TitleScreen::update(int deltaTime)
{
	timeElapsed += deltaTime / 1000.f;
	if (!startPressed && Game::instance().getKey(GLFW_KEY_ENTER))
	{
		startPressed = true;
		SoundEngine::instance().playStart();
	}
	else if (startPressed)
	{
		timeBeforeStart -= deltaTime / 1000.f;
		if (timeBeforeStart <= 0)
		{
			Game::instance().start();
			reset();
		}
		else if (timeBeforeStart > 3.f) renderText = fmod(timeBeforeStart, .5f) < .25f;
	}
}

void TitleScreen::render()
{
	//texProgram->setUniformMatrix4f("projection", projection); //solo cuando cambie la proyección
	if (timeBeforeStart > 1.f)
	{
		sprites[0]->render();
		quads[0]->render();
		if (timeBeforeStart <= 3.f)
		{
			float alpha = timeBeforeStart - 2;
			sprites[0]->setAlpha(alpha);
			quads[0]->setAlpha(alpha+1.25f);
		}
		else if (renderText) pushRunButton.render(" PUSH  RUN  BUTTON!", glm::vec2(128.f, 144.f));
		
	}
}

void TitleScreen::reset()
{
	timeElapsed = 0.f;
	timeBeforeStart = 6.f;
	startPressed = false;
	renderText = true;
}
