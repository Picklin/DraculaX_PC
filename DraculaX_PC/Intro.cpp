#include "Intro.h"
#include "TextureManager.h"

void Intro::init(ShaderProgram& shader)
{
	blackScreen = TexturedQuad::createTexturedQuad(glm::vec2(0), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT), *TextureManager::instance().getTexture("pixel"), shader);
	blackScreen->setColor(glm::vec3(0));
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
}

void Intro::render()
{
	blackScreen->render();
	if (renderSubtitles) dialogueTxt.render(script.front().line, glm::vec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 32));
}
