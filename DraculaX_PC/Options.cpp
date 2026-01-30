#include "Options.h"
#include "TextureManager.h"

enum DynamicMenuElems
{
	FLAME, COUNT
};

void Options::init(ShaderProgram& program)
{
	Texture* menuBG = new Texture();
	menuBG->loadFromFile("images/menuses/menuBG.png", TEXTURE_PIXEL_FORMAT_RGBA);
	TextureManager::instance().addTexture("menuBG", menuBG);
	quads.reserve(1);
	quads.emplace_back(TexturedQuad::createTexturedQuad(glm::vec2(0.f, 0.f), glm::vec2(1.f, 32.f), *menuBG, program));

	sprites.reserve(COUNT);

}

void Options::update(int deltaTime)
{
}