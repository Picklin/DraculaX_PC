#include "Options.h"
#include "TextureManager.h"
#include "Game.h"

const glm::vec2 flamePositions[10] =
{
	glm::vec2(1*8,6*8), glm::vec2(18*8,6*8),
	glm::vec2(1*8,9*8), glm::vec2(18*8,9*8),
	glm::vec2(1*8,12*8), glm::vec2(18*8,12*8),
	glm::vec2(1*8,15*8), glm::vec2(18*8,15*8),
	glm::vec2(11*8,22*8), glm::vec2(19*8,22*8),
};

const vector<wstring> TextLanguageOptions =
{
	L"ENGLISH", L"INGLES",
	L"SPANISH", L"ESPAÑOL"
};
const vector<wstring> VoiceLanguageOptions =
{
	L"ORIGINAL", L"ORIGINAL",
	L"ENGLISH", L"INGLES"
};
const vector<wstring> SubtitlesLanguageOptions =
{
	L"ENGLISH", L"INGLES",
	L"SPANISH", L"ESPAÑOL",
	L"NONE", L"NINGUNO"
};
const vector<wstring> MusicOptions =
{
	L"ORIGINAL", L"ORIGINAL",
	L"ARRANGED", L"ORUESTADO"
};

const vector<wstring> ConfigOptions[4] =
{
	TextLanguageOptions, VoiceLanguageOptions, SubtitlesLanguageOptions, MusicOptions
};

enum configOptionsIds
{
	OPTIONS, LANG = 2, DUB = 4, SUB = 6, MUSIC = 8, END = 10
};

const wstring ConfigOptionsTxt[12] =
{
	L"OPTIONS", L"OPCIONES",
	L"LANGUAGE", L"IDIOMA",
	L"DUBBING", L"DOBLAJE",
	L"SUBTITLES", L"SUBTITULOS",
	L"MUSIC", L"MUSICA",
	L"END", L"FIN"
};

void Options::init(ShaderProgram& program)
{
	Texture* menuBG = new Texture();
	menuBG->loadFromFile("images/menuses/menuBG.png", TEXTURE_PIXEL_FORMAT_RGBA);
	TextureManager::instance().addTexture("menuBG", menuBG);
	quads.reserve(1);
	quads.emplace_back(TexturedQuad::createTexturedQuad(glm::vec2(0.f, 0.f), glm::vec2(1.f, 28.f), *menuBG, program));

	sprites.reserve(2);
	Texture* menuFire = new Texture();
	menuFire->loadFromFile("images/menuses/fire.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprites.emplace_back(Sprite::createSprite(glm::ivec2(16, 32), glm::vec2(0.125f, .5f), menuFire, &program));
	sprites.emplace_back(Sprite::createSprite(glm::ivec2(16, 32), glm::vec2(0.125f, .5f), menuFire, &program));
	sprites[0]->setNumberAnimations(1);
	sprites[0]->setAnimationSpeed(0, 20);
	sprites[0]->animatorX(0, 8, 0.f, 0.125f, 0.f);
	sprites[0]->changeAnimation(0);
	sprites[1]->setNumberAnimations(1);
	sprites[1]->setAnimationSpeed(0, 20);
	sprites[1]->animatorX(0, 8, 0.f, 0.125f, 0.5f);
	sprites[1]->changeAnimation(0);

	ui = TileMap::createTileMap("levels/menuses/options.txt", glm::vec2(0), program);

	text = Text::CreateMenuText(program);

	Screen::init(program);
}

void Options::update(int deltaTime)
{
	bgYoffset -= 0.025f;
	if (bgYoffset < 0.f) bgYoffset = 0.95f;
	sprites[0]->update(deltaTime);
	sprites[1]->update(deltaTime);
	bool up, down, right, left;
	up = Game::instance().getKey(GLFW_KEY_UP);
	down = Game::instance().getKey(GLFW_KEY_DOWN);
	right = Game::instance().getKey(GLFW_KEY_RIGHT);
	left = Game::instance().getKey(GLFW_KEY_LEFT);
	if (down && !downPressed)
	{
		currentFlamePos += 2;
		currentFlamePos %= 12;
	}
	else if (up && !upPressed)
	{
		currentFlamePos -= 2;
		currentFlamePos %= 12;
	}
	else if (right && !rightPressed)
	{
		currentOptions[currentFlamePos / 2] += numLang;
		currentOptions[currentFlamePos / 2] %= ConfigOptions[currentFlamePos / 2].size();
	}
	else if (left && !leftPressed)
	{
		currentOptions[currentFlamePos / 2] -= numLang;
		currentOptions[currentFlamePos / 2] %= ConfigOptions[currentFlamePos / 2].size();
	}
	upPressed = up;
	downPressed = down;
	rightPressed = right;
	leftPressed = left;
}

void Options::render()
{
	texProgram->setUniform1f("yOffset", bgYoffset);
	quads[0]->render();
	texProgram->setUniform1f("yOffset", 0.f);
	ui->render();
	int tileSize = ui->getTileSize();
	currentLang = currentOptions[LANG / numLang - 1] / numLang;
	text->render(ConfigOptionsTxt[configOptionsIds::OPTIONS + currentLang], glm::vec2(16 * tileSize, 4 * tileSize + 4));
	text->render(ConfigOptionsTxt[LANG + currentLang], glm::vec2(10 * tileSize + 4, 9 * tileSize + 4));
	text->render(TextLanguageOptions[currentOptions[LANG/numLang - 1] + currentLang], glm::vec2(26 * tileSize, 9 * tileSize + 4));
	text->render(ConfigOptionsTxt[DUB + currentLang], glm::vec2(10 * tileSize + 4, 12 * tileSize + 4));
	text->render(VoiceLanguageOptions[currentOptions[DUB / numLang - 1] + currentLang], glm::vec2(26 * tileSize, 12 * tileSize + 4));
	text->render(ConfigOptionsTxt[SUB + currentLang], glm::vec2(10 * tileSize + 4, 15 * tileSize + 4));
	text->render(SubtitlesLanguageOptions[currentOptions[SUB / numLang - 1] + currentLang], glm::vec2(26 * tileSize, 15 * tileSize + 4));
	text->render(ConfigOptionsTxt[MUSIC + currentLang], glm::vec2(10 * tileSize + 4, 18 * tileSize + 4));
	text->render(MusicOptions[currentOptions[MUSIC / numLang - 1] + currentLang], glm::vec2(26 * tileSize, 18 * tileSize + 4));
	text->render(ConfigOptionsTxt[END + currentLang], glm::vec2(16 * tileSize, 25 * tileSize + 4));
	sprites[1]->setPosition(glm::vec2(9 * 8, 1 * 8));
	sprites[1]->render();
	sprites[1]->setPosition(glm::vec2(21 * 8, 1 * 8));
	sprites[1]->render();
	sprites[0]->setPosition(flamePositions[currentFlamePos]);
	sprites[0]->render();
	sprites[0]->setPosition(flamePositions[currentFlamePos+1]);
	sprites[0]->render();
}
