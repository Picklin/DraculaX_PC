#include "CoolIntro.h"
#include "TextureManager.h"
#include "Game.h"
#define TIME_BETWEEN_CROSS_COLOR 64

namespace {
	const glm::vec2 crossOffsets[2] = {
		glm::vec2(128,-32), glm::vec2(192,48)
	};
	const float redColors[3] = { 72 / 255.f, 108 / 255.f, 144 / 255.f };
	const float amplitude = 16.f;
	const float frequency = 5.f;
	const int crossSpeeds[3] = { 8, 12, 16 };
	const int crossYlimits[3] = { -76, -128, -176 };
}

void CoolIntro::initChild()
{
	cinematicQuads.resize(COUNTQUADS);
	cinematicSprites.resize(COUNTSPRITES);
	Texture* bgTex = TextureManager::instance().getTexture("coolIntroBg");
	if (TextureManager::instance().exists("bgPalette"))
	{
		bgPalette = TextureManager::instance().getTexture("bgPalette");
	}
	else
	{
		bgPalette = new Texture();
		bgPalette->loadFromFile("images/cinematics/intro_cool/palettes.png", TEXTURE_PIXEL_FORMAT_RGBA);
		TextureManager::instance().addTexture("bgPalette", bgPalette);
	}
	glm::ivec2 fullScreen(SCREEN_WIDTH, SCREEN_HEIGHT);
	glm::vec2 offset(0.125f, 0.25f);
	Texture* lvl0Tex = TextureManager::instance().getTexture("prologueTexs");
	Texture* pixTex = TextureManager::instance().getTexture("pixel");
	blackBar48px = TexturedQuad::createTexturedQuad(glm::vec2(0.f, 0.f), glm::vec2(SCREEN_WIDTH, 48.f), *pixTex, *shader);
	blackBar32px = TexturedQuad::createTexturedQuad(glm::vec2(0.f, 0.f), glm::vec2(SCREEN_WIDTH, 32.f), *pixTex, *shader);
	colorBackground = TexturedQuad::createTexturedQuad(glm::vec2(0.f, 0.f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT), *pixTex, *shader);
	blackBar32px->setPosition(glm::vec2(0, SCREEN_HEIGHT - 32));
	blackBar48px->setColor(glm::vec3(0));
	blackBar32px->setColor(glm::vec3(0));
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
	Texture* handTex = new Texture();
	handTex->loadFromFile("images/cinematics/intro_cool/cool_intro_texs.png", TEXTURE_PIXEL_FORMAT_RGBA);
	TextureManager::instance().addTexture("hand", handTex);
	cinematicQuads[HAND] = TexturedQuad::createTexturedQuad(glm::vec2(0.f, 0.f), glm::vec2(0.5f, 0.35f), *handTex, *shader);
	cinematicQuads[HAND]->setPosition(glm::vec2(-72, -128));
	shakeAngleStep = 16;
	shakeDist = 16;
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
	cinematicQuads[RICHTER1] = TexturedQuad::createTexturedQuad(glm::vec2(0.125f, 0.f), glm::vec2(0.25f,0.25f), *bgTex, *shader);
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
	cinematicQuads[SKY] = TexturedQuad::createTexturedQuad(glm::vec2(0.f, 0.f), glm::vec2(8.f, 1.f), *skytex, *shader);
	cinematicQuads[TREES_FAR] = TexturedQuad::createTexturedQuad(glm::vec2(0.f, 0.f), glm::vec2(16.f, 1.f), *treestex, *shader);
	cinematicQuads[TREES_CLOSE] = TexturedQuad::createTexturedQuad(glm::vec2(0.f, 0.3125f), glm::vec2(1.f, 0.5f), *lvl0Tex, *shader);
	cinematicQuads[CAR_TOP] = TexturedQuad::createTexturedQuad(glm::vec2(0.0625f, 0.25f), glm::vec2(0.125f, 0.3125f), *lvl0Tex, *shader);
	cinematicQuads[CAR] = TexturedQuad::createTexturedQuad(glm::vec2(0.f, 0.5f), glm::vec2(0.40625f, 0.71875f), *lvl0Tex, *shader);
	cinematicQuads[GRASS] = TexturedQuad::createTexturedQuad(glm::vec2(0.f, 0.84375f), glm::vec2(1.f, 1.f), *lvl0Tex, *shader);
	cinematicSprites[WHEELS] = Sprite::createSprite(glm::ivec2(32), glm::vec2(0.0625f, 0.125f), lvl0Tex, shader);
	cinematicSprites[WHEELS]->setNumberAnimations(1);
	cinematicSprites[WHEELS]->setAnimationSpeed(0, 30);
	cinematicSprites[WHEELS]->animatorX(0, 4, 0.f, 0.0625f, 0.71875f);
	cinematicSprites[WHEELS]->changeAnimation(0);
	cinematicSprites[HORSES] = Sprite::createSprite(glm::ivec2(128, 80), glm::vec2(0.25f, 0.3125f), lvl0Tex, shader);
	cinematicSprites[HORSES]->setNumberAnimations(1);
	cinematicSprites[HORSES]->setAnimationSpeed(0, 12);
	cinematicSprites[HORSES]->animatorX(0, 3, 0.125f, 0.25f, 0.f);
	cinematicSprites[HORSES]->animatorX(0, 2, 0.5f, 0.25f, 0.5f);
	cinematicSprites[HORSES]->changeAnimation(0);
	cinematicQuads[TREES_FAR]->setPosition(glm::vec2(0, 64));
	cinematicQuads[TREES_CLOSE]->setPosition(glm::vec2(0, 80));
	cinematicQuads[CAR_TOP]->setPosition(glm::vec2(16, 112));
	cinematicQuads[CAR]->setPosition(glm::vec2(0, 128));
	cinematicQuads[GRASS]->setPosition(glm::vec2(0, 224 - 40));
	cinematicSprites[HORSES]->setPosition(glm::vec2(120, 120));
	sp->setPosition(glm::vec2(72, 134));
	bg.bg = sp;
	bg.time = 2.75f;
	bg.duration = 6.25f;
	bg.id = HORSES;
	film.push(bg);
	sp = Sprite::createSprite(fullScreen, glm::vec2(0.5f, 0.25f), TextureManager::instance().getTexture("titleBG"), shader);
	sp->setNumberAnimations(2);
	sp->setAnimationSpeed(0, 0);
	sp->addKeyframe(0, glm::vec2(0.f, 0.f));
	sp->setAnimationSpeed(1, 20);
	sp->addKeyframe(1, glm::vec2(0.5f, 0.f));
	sp->addKeyframe(1, glm::vec2(0.f, 0.0f));
	sp->addKeyframe(1, glm::vec2(0.5f, 0.f));
	sp->setTransition(1, 0);
	sp->changeAnimation(0);
	bg.bg = sp;
	bg.time = 9.f;
	bg.duration = 6.f;
	bg.id = CASTLEVANIA;
	boltDuration = 64;	//milisecs
	film.push(bg);
	Texture* skelTex = new Texture();
	skelTex->loadFromFile("images/cinematics/intro_cool/skeleton_cementery.png", TEXTURE_PIXEL_FORMAT_RGBA);
	TextureManager::instance().addTexture("skeleton_intro_cementery", skelTex);
	cinematicQuads[SKELETON_CEMENTERY] = TexturedQuad::createTexturedQuad(glm::vec2(0.f), glm::vec2(1.f), *skelTex, *shader);
	cinematicQuads[SKELETON_CEMENTERY]->setPosition(glm::vec2(32, SCREEN_HEIGHT));
	sp = Sprite::createSprite(fullScreen, offset, bgTex, shader);
	sp->setNumberAnimations(1);
	sp->setAnimationSpeed(0, 0);
	sp->addKeyframe(0, glm::vec2(0.25f, 0.f));
	sp->changeAnimation(0);
	sp->setColorPalette(bgPalette);
	sp->setNumberPaletteAnimations(3);
	sp->setPaletteSpeed(0, 0);
	sp->addPaletteKeyframe(0, 0.025f);
	sp->setPaletteSpeed(1, 8);
	sp->addPaletteKeyframe(1, 0.075f);
	sp->addPaletteKeyframe(1, 0.125f);
	sp->setPaletteSpeed(2, 0);
	sp->addPaletteKeyframe(2, 0.175f);
	sp->setPaletteTransition(1, 2);
	sp->changePaletteAnimation(0);
	bg.bg = sp;
	bg.time = 15.5f;
	bg.duration = 5.5f;
	bg.id = CEMENTERY;
	film.push(bg);
	sp = Sprite::createSprite(glm::ivec2(SCREEN_WIDTH, SCREEN_HEIGHT * 2), glm::vec2(0.125f, 0.5f), bgTex, shader);
	sp->setNumberAnimations(1);
	sp->setAnimationSpeed(0, 0);
	sp->addKeyframe(0, glm::vec2(0.875f, 0.f));
	sp->changeAnimation(0);
	sp->setPosition(glm::vec2(0, -SCREEN_HEIGHT));
	sp->setColorPalette(bgPalette);
	sp->setNumberPaletteAnimations(1);
	sp->setPaletteSpeed(0, 8);
	sp->addPaletteKeyframe(0, 0.225f);
	sp->addPaletteKeyframe(0, 0.275f);
	sp->addPaletteKeyframe(0, 0.325f);
	sp->changePaletteAnimation(0);
	bg.bg = sp;
	bg.time = 21.f;
	bg.duration = 3.75f;
	bg.id = GIANT;
	film.push(bg);
	sp = Sprite::createSprite(fullScreen, offset, bgTex, shader);
	sp->setNumberAnimations(3);
	sp->setAnimationSpeed(0, 0);
	sp->addKeyframe(0, glm::vec2(0.f, 0.25f));
	sp->setAnimationSpeed(1, 0);
	sp->addKeyframe(1, glm::vec2(0.125f, 0.25f));
	sp->setAnimationSpeed(2, 0);
	sp->addKeyframe(2, glm::vec2(0.25f, 0.25f));
	sp->changeAnimation(0);
	bg.bg = sp;
	bg.time = 25.f;
	bg.duration = 2.5f;
	bg.id = PEOPLE;
	film.push(bg);
	glm::vec2 annettePos(64, 64);
	sp = Sprite::createSprite(glm::ivec2(128, 160), glm::vec2(0.f, 0.35f), glm::vec2(0.4f, 0.85f), handTex, shader);
	sp->setPosition(annettePos);
	cinematicQuads[ANNETTE_EYES] = TexturedQuad::createTexturedQuad(glm::vec2(0.5f, 0.f), glm::vec2(0.65f, 0.05f), *handTex, *shader);
	cinematicQuads[ANNETTE_EYES]->setPosition(annettePos + glm::vec2(32 + 6, 48));
	cinematicQuads[ANNETTE_MOUTH] = TexturedQuad::createTexturedQuad(glm::vec2(0.5f, 0.1f), glm::vec2(0.6f, 0.15f), *handTex, *shader);
	cinematicQuads[ANNETTE_MOUTH]->setPosition(annettePos + glm::vec2(32 + 6, 48 + 32));
	bg.bg = sp;
	bg.time = 27.5f;
	bg.duration = 3.f;
	bg.id = ANNETTE;
	film.push(bg);
	sp = Sprite::createSprite(glm::ivec2(SCREEN_WIDTH * 2, SCREEN_HEIGHT), glm::vec2(0.25f, 0.25f), bgTex, shader);
	sp->setNumberAnimations(1);
	sp->setAnimationSpeed(0, 0);
	sp->addKeyframe(0, glm::vec2(0.375f, 0.25f));
	sp->changeAnimation(0);
	bg.bg = sp;
	bg.time = 30.5f;
	bg.duration = 2.5f;
	bg.id = GIANTS;
	film.push(bg);
	sp = Sprite::createSprite(fullScreen, offset, bgTex, shader);
	sp->setNumberAnimations(1);
	sp->setAnimationSpeed(0, 1);
	sp->addKeyframe(0, glm::vec2(0.625f, 0.25f));
	sp->changeAnimation(0);
	cinematicQuads[BOOT] = TexturedQuad::createTexturedQuad(glm::vec2(0.7f, 0.f), glm::vec2(1.f, 0.5f), *handTex, *shader);
	cinematicQuads[BOOT]->setPosition(glm::vec2(94, -160));
	bg.bg = sp;
	bg.time = 33.f;
	bg.duration = 1.f;
	bg.id = RICHTER_BOOT;
	film.push(bg);
	sp = Sprite::createSprite(fullScreen, offset, bgTex, shader);
	sp->setNumberAnimations(3);
	sp->setAnimationSpeed(0, 0);
	sp->addKeyframe(0, glm::vec2(0.f, 0.5f));
	sp->setAnimationSpeed(1, 0);
	sp->addKeyframe(1, glm::vec2(0.125f, 0.5f));
	sp->setAnimationSpeed(2, 0);
	sp->addKeyframe(2, glm::vec2(0.25f, 0.5f));
	sp->changeAnimation(0);
	bg.bg = sp;
	bg.time = 34.f;
	bg.duration = 1.5f;
	bg.id = RICHTER_ARRIVE;
	film.push(bg);
	sp = Sprite::createSprite(fullScreen, offset, bgTex, shader);
	sp->setNumberAnimations(4);
	sp->setAnimationSpeed(0, 0);
	sp->addKeyframe(0, glm::vec2(0.375f, 0.5f));
	sp->setAnimationSpeed(1, 0);
	sp->addKeyframe(1, glm::vec2(0.5f, 0.5f));
	sp->setAnimationSpeed(2, 0);
	sp->addKeyframe(2, glm::vec2(0.625f, 0.5f));
	sp->setAnimationSpeed(3, 0);
	sp->addKeyframe(3, glm::vec2(0.75f, 0.5f));
	sp->changeAnimation(0);
	Texture* richterParts = new Texture();
	richterParts->loadFromFile("images/cinematics/intro_cool/richter_parts.png", TEXTURE_PIXEL_FORMAT_RGBA);
	TextureManager::instance().addTexture("richterParts", richterParts);
	glm::vec2 offset2(0.25f, 0.25f);
	cinematicSprites[RICHT_HANDS] = Sprite::createSprite(fullScreen / 2, offset2, richterParts, shader);
	cinematicSprites[RICHT_HANDS]->setNumberAnimations(2);
	cinematicSprites[RICHT_HANDS]->setAnimationSpeed(0, 15);
	cinematicSprites[RICHT_HANDS]->animatorX(0, 3, 0.f, 0.25f, 0.f);
	cinematicSprites[RICHT_HANDS]->setAnimationSpeed(1, 15);
	cinematicSprites[RICHT_HANDS]->addKeyframe(1, glm::vec2(0.75f, 0.f));
	cinematicSprites[RICHT_HANDS]->animatorX(1, 2, 0.f, 0.25f, 0.25f);
	cinematicSprites[RICHT_HANDS]->changeAnimation(0);
	cinematicSprites[RICHT_DUST] = Sprite::createSprite(fullScreen / 2, offset2, richterParts, shader);
	cinematicSprites[RICHT_DUST]->setNumberAnimations(2);
	cinematicSprites[RICHT_DUST]->setAnimationSpeed(0, 15);
	cinematicSprites[RICHT_DUST]->animatorX(0, 2, 0.5f, 0.25f, 0.25f);
	cinematicSprites[RICHT_DUST]->addKeyframe(0, glm::vec2(0.f, 0.5f));
	cinematicSprites[RICHT_DUST]->setAnimationSpeed(1, 0);
	cinematicSprites[RICHT_DUST]->addKeyframe(1, glm::vec2(0.5f, 0.75f));
	cinematicSprites[RICHT_DUST]->setTransition(0, 1);
	cinematicSprites[RICHT_DUST]->changeAnimation(0);
	cinematicSprites[RICHT_DUST]->setPosition(glm::vec2(0, 112));
	cinematicSprites[RICHT_WHIP] = Sprite::createSprite(fullScreen / 2, offset2, richterParts, shader);
	cinematicSprites[RICHT_WHIP]->setNumberAnimations(2);
	cinematicSprites[RICHT_WHIP]->setAnimationSpeed(0, 0);
	cinematicSprites[RICHT_WHIP]->addKeyframe(0, glm::vec2(0.f, 0.75f));
	cinematicSprites[RICHT_WHIP]->setAnimationSpeed(1, 0);
	cinematicSprites[RICHT_WHIP]->addKeyframe(1, glm::vec2(0.25f, 0.75f));
	cinematicSprites[RICHT_WHIP]->changeAnimation(0);
	cinematicSprites[RICHT_WHIP]->setPosition(glm::vec2(128, 16));
	cinematicSprites[RICHT_FACE] = Sprite::createSprite(fullScreen / 2, offset2, richterParts, shader);
	cinematicSprites[RICHT_FACE]->setNumberAnimations(3);
	cinematicSprites[RICHT_FACE]->setAnimationSpeed(0, 0);
	cinematicSprites[RICHT_FACE]->addKeyframe(0, glm::vec2(0.25f, 0.5f));
	cinematicSprites[RICHT_FACE]->setAnimationSpeed(1, 10);
	cinematicSprites[RICHT_FACE]->addKeyframe(1, glm::vec2(0.5f, 0.5f));
	cinematicSprites[RICHT_FACE]->setAnimationSpeed(2, 0);
	cinematicSprites[RICHT_FACE]->addKeyframe(2, glm::vec2(0.75f, 0.5f));
	cinematicSprites[RICHT_FACE]->setTransition(1, 2);
	cinematicSprites[RICHT_FACE]->changeAnimation(0);
	cinematicSprites[RICHT_FACE]->setPosition(glm::vec2(128, 112));
	Texture* bootTex = new Texture();
	bootTex->loadFromFile("images/cinematics/intro_cool/boot.png", TEXTURE_PIXEL_FORMAT_RGBA);
	TextureManager::instance().addTexture("boot", bootTex);
	cinematicQuads[BOOT2] = TexturedQuad::createTexturedQuad(glm::vec2(0), glm::vec2(1.f), *bootTex, *shader);
	cinematicQuads[BOOT2]->setPosition(glm::vec2(-SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2));
	//blackBox128x112 = TexturedQuad::createTexturedQuad(glm::vec2(0.f, 0.f), glm::vec2(128, 112), *pixTex, *shader);
	//blackBox128x112->setColor(glm::vec3(0));
	bg.bg = sp;
	bg.time = 35.75f;
	bg.duration = 4.75f;
	bg.id = RICHTER_READY;
	film.push(bg);
	sp = Sprite::createSprite(glm::ivec2(SCREEN_WIDTH * 3, SCREEN_HEIGHT), glm::vec2(0.375f, 0.25f), bgTex, shader);
	sp->setNumberAnimations(1);
	sp->setAnimationSpeed(0, 0);
	sp->addKeyframe(0, glm::vec2(0.f, 0.75f));
	sp->changeAnimation(0);
	bg.bg = sp;
	bg.time = 41.f;
	bg.duration = 2.f;
	bg.id = RICHTER_WHIP1;
	film.push(bg);
	sp = Sprite::createSprite(glm::ivec2(SCREEN_WIDTH * 4, SCREEN_HEIGHT), glm::vec2(0.5f, 0.25f), bgTex, shader);
	sp->setNumberAnimations(1);
	sp->setAnimationSpeed(0, 0);
	sp->addKeyframe(0, glm::vec2(0.375f, 0.f));
	sp->changeAnimation(0);
	sp->setPosition(glm::vec2(-SCREEN_WIDTH * 3, 0));
	bg.bg = sp;
	bg.time = 43.f;
	bg.duration = 1.5f;
	bg.id = RICHTER_WHIP2;
	film.push(bg);
	sp = Sprite::createSprite(glm::ivec2(SCREEN_WIDTH/2, SCREEN_HEIGHT), glm::vec2(0.0625f, 0.25f), bgTex, shader);
	sp->setNumberAnimations(1);
	sp->setAnimationSpeed(0, 0);
	sp->addKeyframe(0, glm::vec2(0.375f, 0.75f));
	sp->changeAnimation(0);
	sp->setPosition(glm::vec2(-SCREEN_WIDTH, 0));
	Texture* pfire = new Texture();
	pfire->loadFromFile("images/cinematics/intro_cool/purple_fire.png", TEXTURE_PIXEL_FORMAT_RGBA);
	TextureManager::instance().addTexture("pfire", pfire);
	cinematicSprites[PURPLE_FIRE] = Sprite::createSprite(glm::ivec2(80), glm::vec2(0.2f, 1.f), pfire, shader);
	cinematicSprites[PURPLE_FIRE]->setNumberAnimations(1);
	cinematicSprites[PURPLE_FIRE]->setAnimationSpeed(0, 10);
	cinematicSprites[PURPLE_FIRE]->animatorX(0, 5, 0.f, 0.2f, 0.f);
	cinematicSprites[PURPLE_FIRE]->changeAnimation(0);
	cinematicSprites[PURPLE_FIRE]->setPosition(glm::vec2(49, 48));
	cinematicQuads[RICHTER2] = TexturedQuad::createTexturedQuad(glm::vec2(0.4375f, 0.75f), glm::vec2(0.5f, 1.f), *bgTex, *shader);
	cinematicQuads[RICHTER2]->setPosition(glm::vec2(SCREEN_WIDTH * 2 + SCREEN_WIDTH/2, 0));
	bg.bg = sp;
	bg.time = 44.75f;
	bg.duration = 3.25f;
	bg.id = RICHTER_WHIP3;
	film.push(bg);
	sp = Sprite::createSprite(fullScreen, offset, bgTex, shader);
	sp->setNumberAnimations(1);
	sp->setAnimationSpeed(0, 0);
	sp->addKeyframe(0, glm::vec2(0.5f, 0.75f));
	sp->changeAnimation(0);
	Texture* richtEyes = new Texture();
	richtEyes->loadFromFile("images/cinematics/intro_cool/richter_sus.png", TEXTURE_PIXEL_FORMAT_RGBA);
	TextureManager::instance().addTexture("richtEyes", richtEyes);
	cinematicSprites[RICHTER_EYES] = Sprite::createSprite(fullScreen, glm::vec2(0.25f, 1.f), richtEyes, shader);
	cinematicSprites[RICHTER_EYES]->setNumberAnimations(2);
	cinematicSprites[RICHTER_EYES]->setAnimationSpeed(0, 10);
	cinematicSprites[RICHTER_EYES]->animatorX(0, 2, 0.f, 0.25f, 0.f);
	cinematicSprites[RICHTER_EYES]->setAnimationSpeed(1, 0);
	cinematicSprites[RICHTER_EYES]->addKeyframe(1, glm::vec2(0.5f, 0.f));
	cinematicSprites[RICHTER_EYES]->setTransition(0, 1);
	cinematicSprites[RICHTER_EYES]->changeAnimation(0);
	bg.bg = sp;
	bg.time = 48.f;
	bg.duration = 2.f;
	bg.id = RICHTER_SUS;
	film.push(bg);
	sp = Sprite::createSprite(glm::ivec2(SCREEN_WIDTH * 2, SCREEN_HEIGHT), glm::vec2(0.25f, 0.25f), bgTex, shader);
	sp->setNumberAnimations(1);
	sp->setAnimationSpeed(0, 0);
	sp->addKeyframe(0, glm::vec2(0.625f, 0.75f));
	sp->changeAnimation(0);
	sp->setPosition(glm::vec2(-SCREEN_WIDTH, 0));
	Texture* richtAmbushed = new Texture();
	richtAmbushed->loadFromFile("images/cinematics/intro_cool/richter_ambushed.png", TEXTURE_PIXEL_FORMAT_RGBA);
	TextureManager::instance().addTexture("richtAmbushed", richtAmbushed);
	Texture* skelAmbush = new Texture();
	skelAmbush->loadFromFile("images/cinematics/intro_cool/skeleton.png", TEXTURE_PIXEL_FORMAT_RGBA);
	TextureManager::instance().addTexture("skelAmbush", skelAmbush);
	cinematicQuads[RICHTER3] = TexturedQuad::createTexturedQuad(glm::vec2(0), glm::vec2(1), *richtAmbushed, *shader);
	cinematicQuads[SKELETON_AMBUSH] = TexturedQuad::createTexturedQuad(glm::vec2(0), glm::vec2(1), *skelAmbush, *shader);
	cinematicQuads[RICHTER3]->setPosition(glm::vec2(79, 56));
	cinematicQuads[SKELETON_AMBUSH]->setPosition(glm::vec2(21, 109));
	bg.bg = sp;
	bg.time = 50.25f;
	bg.duration = 2.25f;
	bg.id = RICHTER_AMBUSHED;
	film.push(bg);
	sp = Sprite::createSprite(fullScreen, offset, bgTex, shader);
	sp->setNumberAnimations(1);
	sp->setAnimationSpeed(0, 0);
	sp->addKeyframe(0, glm::vec2(0.875f, 0.5f));
	sp->changeAnimation(0);
	sp->setColorPalette(bgPalette);
	sp->setNumberPaletteAnimations(1);
	sp->setPaletteSpeed(0, 15);
	sp->addPaletteKeyframe(0, 0.375f);
	sp->addPaletteKeyframe(0, 0.425f);
	sp->changePaletteAnimation(0);
	Texture* hands = new Texture();
	hands->loadFromFile("images/cinematics/intro_cool/hands.png", TEXTURE_PIXEL_FORMAT_RGBA);
	TextureManager::instance().addTexture("richtHands", hands);
	cinematicQuads[RIGHT_HAND] = TexturedQuad::createTexturedQuad(glm::vec2(0.f, 0.f), glm::vec2(0.5f, 1.f), *hands, *shader);
	cinematicQuads[LEFT_HAND] = TexturedQuad::createTexturedQuad(glm::vec2(0.5f, 0.f), glm::vec2(1.f, 1.f), *hands, *shader);
	cinematicQuads[RIGHT_HAND]->setPosition(glm::vec2(50, 142));
	cinematicQuads[LEFT_HAND]->setPosition(glm::vec2(142, 142));
	bg.bg = sp;
	bg.time = 52.75f;
	bg.duration = 1.f;
	bg.id = RICHTER_ULT1;
	film.push(bg);
	sp = Sprite::createSprite(fullScreen, offset, bgTex, shader);
	sp->setNumberAnimations(1);
	sp->setAnimationSpeed(0, 30);
	sp->addKeyframe(0, glm::vec2(0.75f, 0.25f));
	sp->changeAnimation(0);
	sp->setColorPalette(bgPalette);
	sp->setNumberPaletteAnimations(2);
	sp->setPaletteSpeed(0, 60);
	sp->addPaletteKeyframe(0, 0.475f);
	sp->setPaletteSpeed(1, 12);
	sp->addPaletteKeyframe(1, 0.525f);
	sp->addPaletteKeyframe(1, 0.575f);
	sp->setPaletteTransition(0, 1);
	sp->changePaletteAnimation(0);
	Texture* elems = new Texture();
	elems->loadFromFile("images/cinematics/intro_cool/ult_elements.png", TEXTURE_PIXEL_FORMAT_RGBA);
	TextureManager::instance().addTexture("ultElements", elems);
	cinematicSprites[PURPLE_FIRE2] = Sprite::createSprite(glm::ivec2(SCREEN_WIDTH, SCREEN_HEIGHT), glm::vec2(0.25f, 0.5f), elems, shader);
	cinematicSprites[PURPLE_FIRE2]->setNumberAnimations(1);
	cinematicSprites[PURPLE_FIRE2]->setAnimationSpeed(0, 10);
	cinematicSprites[PURPLE_FIRE2]->animatorX(0, 3, 0.f, 0.25f, 0.5f);
	cinematicSprites[PURPLE_FIRE2]->changeAnimation(0);
	cinematicSprites[WHIRLWIND] = Sprite::createSprite(glm::ivec2(SCREEN_WIDTH, SCREEN_HEIGHT), glm::vec2(0.25f, 0.5f), elems, shader);
	cinematicSprites[WHIRLWIND]->setNumberAnimations(1);
	cinematicSprites[WHIRLWIND]->setAnimationSpeed(0, 30);
	cinematicSprites[WHIRLWIND]->animatorX(0, 4, 0.25f, 0.25f, 0.f);
	cinematicSprites[WHIRLWIND]->changeAnimation(0);
	cinematicQuads[SMALL_CROSS] = TexturedQuad::createTexturedQuad(glm::vec2(0.75f, 0.5f), glm::vec2(0.8125f, 1.f), *elems, *shader);
	cinematicQuads[SMALL_CROSS]->setColorPalette(bgPalette);
	cinematicQuads[SMALL_CROSS]->setNumberPaletteAnimations(1);
	cinematicQuads[SMALL_CROSS]->setPaletteSpeed(0, 0);
	cinematicQuads[SMALL_CROSS]->addPaletteKeyframe(0, 0.475f);
	cinematicQuads[SMALL_CROSS]->changePaletteAnimation(0);
	cinematicQuads[SMALL_CROSS]->setPosition(glm::vec2(44, -16));
	cinematicQuads[CROSS] = TexturedQuad::createTexturedQuad(glm::vec2(0.8125f, 0.5f), glm::vec2(0.875f, 1.f), *elems, *shader);
	cinematicQuads[CROSS]->setColorPalette(bgPalette);
	cinematicQuads[CROSS]->setNumberPaletteAnimations(1);
	cinematicQuads[CROSS]->setPaletteSpeed(0, 0);
	cinematicQuads[CROSS]->addPaletteKeyframe(0, 0.475f);
	cinematicQuads[CROSS]->changePaletteAnimation(0);
	cinematicQuads[CROSS]->setPosition(glm::vec2(0, -48));
	cinematicQuads[BIG_CROSS] = TexturedQuad::createTexturedQuad(glm::vec2(0.875f, 0.5f), glm::vec2(1.f, 1.f), *elems, *shader);
	cinematicQuads[BIG_CROSS]->setColorPalette(bgPalette);
	cinematicQuads[BIG_CROSS]->setNumberPaletteAnimations(1);
	cinematicQuads[BIG_CROSS]->setPaletteSpeed(0, 0);
	cinematicQuads[BIG_CROSS]->addPaletteKeyframe(0, 0.475f);
	cinematicQuads[BIG_CROSS]->changePaletteAnimation(0);
	cinematicQuads[BIG_CROSS]->setPosition(glm::vec2(96, -48));
	crossColorTimer[0] = 64;
	crossColorTimer[1] = 16;
	crossColorTimer[2] = 0;
	bg.bg = sp;
	bg.time = 54.f;
	bg.duration = 7.f;
	bg.id = RICHTER_ULT2;
	film.push(bg);
}

void CoolIntro::filmUpdate(int deltaTime)
{
	int filmId = film.front().id;
	if (filmId == MAP)
	{
		if (cinematicQuads[HAND]->getPosition().x < 16) cinematicQuads[HAND]->incPosition(glm::vec2(4, 8));
		else if (shaking)
		{
			shakeAngle += shakeAngleStep;
			if (shakeAngle >= 180)
			{
				shaking = false;
				shakeAngle = 0;
			}
			else
			{
				float positionY = startY + shakeDist * sin(glm::radians((float)shakeAngle));
				film.front().bg->setPosition(glm::vec2(0, positionY));
				cinematicQuads[HAND]->setPosition(glm::vec2(cinematicQuads[HAND]->getPosition().x, positionY + 48));
			}
		}
		else if (!shaked)
		{
			shaking = true;
			shaked = true;
			startY = 0.f;
		}
	}
	else if (filmId == HORSES)
	{
		if (timeElapsed >= 3.75f && richter1Alpha > 0)
		{
			richter1Alpha -= deltaTime / 3000.f;
			cinematicQuads[RICHTER1]->setAlpha(richter1Alpha);
		}
		bgXScroll += 0.0125f;
		cinematicSprites[WHEELS]->update(deltaTime);
		cinematicSprites[HORSES]->update(deltaTime);
	}
	else if (filmId == CASTLEVANIA && boltTimer <= 0)
	{
		if (film.front().bg->animation() != 1) film.front().bg->changeAnimation(1);
		boltDuration -= deltaTime;
		if (boltDuration < 0)
		{
			boltTimer = 2.f;
			boltDuration = 64;
			renderBigBolt = !renderBigBolt;
		}
	}
	else if (filmId == CEMENTERY)
	{
		if (timeElapsed >= 17 && film.front().bg->paletteAnimation() < 1) film.front().bg->changePaletteAnimation(1);
		else if (timeElapsed >= 19 && cinematicQuads[SKELETON_CEMENTERY]->getPosition().y > 80) cinematicQuads[SKELETON_CEMENTERY]->incPosition(glm::vec2(0, -2));
	}
	else if (filmId == GIANT && film.front().bg->getPosition().y < 0)
	{
		film.front().bg->incPosition(glm::vec2(0, 1));
	}
	else if (filmId == PEOPLE)
	{
		Sprite* s = film.front().bg;
		if (timeElapsed >= 26.6f) s->changeAnimation(2);
		else if (timeElapsed >= 25.75f) s->changeAnimation(1);
		else s->changeAnimation(0);
		colorBackground->setColor(glm::vec3(redColors[getColorIndex(3, timeElapsed * 10)], 0, 0));
	}
	else if (filmId == ANNETTE)
	{
		if (cinematicQuads[ANNETTE_EYES]->getPosition().y > 80)
		{
			glm::vec2 inc(0, -.5f);
			cinematicQuads[ANNETTE_EYES]->incPosition(inc);
			cinematicQuads[ANNETTE_MOUTH]->incPosition(inc);
			film.front().bg->incPosition(inc);
		}
		else
		{
			if (timeElapsed >= 29.5f && !annetteBlinked)
			{
				annetteEyesDuration = 128;
				annetteBlinked = true;
			}
			else if (annetteEyesDuration > 0) annetteEyesDuration -= deltaTime;
		}
		colorBackground->setColor(glm::vec3(redColors[getColorIndex(3, timeElapsed*10)], 0, 0));
	}
	else if (filmId == GIANTS && cameraX > -SCREEN_WIDTH / 2.f)
	{
		film.front().bg->setPosition(glm::vec2(cameraX--, 0));
	}
	else if (filmId == RICHTER_BOOT && cinematicQuads[BOOT]->getPosition().y < 32)
	{
		cinematicQuads[BOOT]->incPosition(glm::vec2(0, 8));
	}
	else if (filmId == RICHTER_ARRIVE)
	{
		Sprite* sp = film.front().bg;
		if (timeElapsed > 34.5f && sp->animation() == 0) sp->changeAnimation(1);
		else if (timeElapsed > 35.f && sp->animation() == 1) sp->changeAnimation(2);
	}
	else if (filmId == RICHTER_READY)
	{
		cinematicSprites[RICHT_HANDS]->update(deltaTime);
		int anim = film.front().bg->animation();
		float bootPosX = cinematicQuads[BOOT2]->getPosition().x;
		if (timeElapsed >= 37.f && bootPosX < 0)
		{
			if (cinematicSprites[RICHT_HANDS]->animation() == 0)
			{
				//cout << "cambio de animacion de las manos" << endl;
				cinematicSprites[RICHT_HANDS]->changeAnimation(1);
				cameraX = 0;
				colorBackground->setColor(glm::vec3(0, 0, 36/255.f));
			}
			else
			{
				//cout << "apareciendo la bota" << endl;
				cinematicQuads[BOOT2]->incPosition(glm::vec2(4, 0));	
			}
		}
		else if (timeElapsed >= 39.f)
		{
			//cout << "cara" << endl;
			if (timeElapsed >= 39.5f && cinematicSprites[RICHT_FACE]->animation() == 0) cinematicSprites[RICHT_FACE]->changeAnimation(1);
			if (anim != 3) film.front().bg->changeAnimation(3);
			else cinematicSprites[RICHT_FACE]->update(deltaTime);
		}	
		else if (timeElapsed >= 38.f)
		{
			//cout << "whip ready" << endl;
			if (cinematicSprites[RICHT_WHIP]->getPosition().y > 0) cinematicSprites[RICHT_WHIP]->incPosition(glm::vec2(0, -2));
			else if (cinematicSprites[RICHT_WHIP]->animation() != 1) cinematicSprites[RICHT_WHIP]->changeAnimation(1);
			if (anim != 2) film.front().bg->changeAnimation(2);
		}
		else if (bootPosX == 0)
		{
			//cout << "entro, aparece la bota, momento: " << timeElapsed << endl;
			if (anim != 1) film.front().bg->changeAnimation(1);
			cinematicSprites[RICHT_DUST]->update(deltaTime);
		}
	}
	else if (filmId == RICHTER_WHIP1 && cameraX > -SCREEN_WIDTH * 2)
	{
		cameraX -= 4;
		film.front().bg->setPosition(glm::vec2(cameraX, 0));
	}
	else if (filmId == RICHTER_WHIP2 && film.front().bg->getPosition().x < 0)
	{
		film.front().bg->incPosition(glm::vec2(8, 0));
	}
	else if (filmId == RICHTER_WHIP3)
	{
		Sprite* sp = film.front().bg;
		if (sp->getPosition().x < 0) sp->incPosition(glm::vec2(2, 0));
		if (cinematicQuads[RICHTER2]->getPosition().x > 128) cinematicQuads[RICHTER2]->incPosition(glm::vec2(-4, 0));
		else if (timeElapsed >= 47.25f && !cinematicSprites[PURPLE_FIRE]->animationEnded()) cinematicSprites[PURPLE_FIRE]->update(deltaTime);
	}
	else if (filmId == RICHTER_SUS && timeElapsed >= 48.75f)
	{
		cinematicSprites[RICHTER_EYES]->update(deltaTime);
	}
	else if (filmId == RICHTER_AMBUSHED && film.front().bg->getPosition().x < 0)
	{
		cinematicQuads[SKELETON_AMBUSH]->incPosition(glm::vec2(-1, 0));
		film.front().bg->incPosition(glm::vec2(2, 0));
	}
	else if (filmId == RICHTER_ULT1 && cinematicQuads[RIGHT_HAND]->getPosition().y > (SCREEN_HEIGHT - 32 - 64))
	{
		cinematicQuads[RIGHT_HAND]->incPosition(glm::vec2(.5, -.5));
		cinematicQuads[LEFT_HAND]->incPosition(glm::vec2(-.5, -.5));
	}
	else if (filmId == RICHTER_ULT2)
	{
		if (timeElapsed >= 57.f && film.front().alpha > 0)
		{
			blackScreenAlpha += deltaTime / 3000.f;
			blackScreen->setAlpha(blackScreenAlpha);
		}
		cinematicSprites[PURPLE_FIRE2]->update(deltaTime);
		cinematicSprites[WHIRLWIND]->update(deltaTime);
		for (int i = 0; i < 3; i++)
		{
			updateCross(deltaTime, i);
		}
	}
	boltTimer -= deltaTime / 1000.f;
}

CoolIntro::~CoolIntro()
{
	blackBar32px->free();
	blackBar48px->free();
	colorBackground->free();
	delete blackBar32px;
	delete blackBar48px;
	delete colorBackground;
	for (TexturedQuad* bolts : bolts)
	{
		bolts->free();
		delete bolts;
	}
}

void CoolIntro::render()
{
	if (renderBg)
	{
		int filmId = film.front().id;
		if (filmId == MAP)
		{
			film.front().bg->render();
			cinematicQuads[HAND]->render();
			blackBar48px->render();
		}
		else if (filmId == HORSES)
		{
			shader->setUniform1f("xOffset", bgXScroll * 0.25f);
			cinematicQuads[SKY]->render();
			shader->setUniform1f("xOffset", bgXScroll);
			cinematicQuads[TREES_FAR]->render();
			shader->setUniform1f("xOffset", bgXScroll * 0.5f);
			cinematicQuads[TREES_CLOSE]->render();
			shader->setUniform1f("xOffset", 0.f);
			cinematicQuads[CAR_TOP]->render();
			cinematicQuads[CAR]->render();
			cinematicSprites[WHEELS]->setPosition(glm::vec2(16, 168));
			cinematicSprites[WHEELS]->render();
			cinematicSprites[WHEELS]->setPosition(glm::vec2(78, 168));
			cinematicSprites[WHEELS]->render();
			cinematicSprites[HORSES]->render();
			shader->setUniform1f("xOffset", bgXScroll);
			cinematicQuads[GRASS]->render();
			shader->setUniform1f("xOffset", 0.f);
			film.front().bg->render();
			cinematicQuads[RICHTER1]->render();
		}
		else if (filmId == CASTLEVANIA)
		{
			film.front().bg->render();
			if (boltTimer <= 0) bolts[renderBigBolt]->render();
		}
		else if (filmId == CEMENTERY)
		{
			film.front().bg->render();
			cinematicQuads[SKELETON_CEMENTERY]->render();
			blackBar32px->render();
		}
		else if (filmId == PEOPLE)
		{
			colorBackground->render();
			film.front().bg->render();
		}
		else if (filmId == ANNETTE)
		{
			colorBackground->render();
			film.front().bg->render();
			if (cinematicQuads[ANNETTE_EYES]->getPosition().y > 80)
			{
				cinematicQuads[ANNETTE_EYES]->render();
				cinematicQuads[ANNETTE_MOUTH]->render();
			}
			else if (annetteEyesDuration > 0) cinematicQuads[ANNETTE_EYES]->render();
			blackBar32px->setPosition(glm::vec2(0, 0));
			blackBar32px->render();
			blackBar32px->setPosition(glm::vec2(0, SCREEN_HEIGHT - 32));
			blackBar32px->render();
		}
		else if (filmId == RICHTER_BOOT)
		{
			film.front().bg->render();
			cinematicQuads[BOOT]->render();
			blackBar32px->setPosition(glm::vec2(0, 0));
			blackBar32px->render();
			blackBar32px->setPosition(glm::vec2(0, SCREEN_HEIGHT - 32));
			blackBar32px->render();
		}
		else if (filmId == RICHTER_READY)
		{
			film.front().bg->render();
			cinematicSprites[RICHT_HANDS]->render();
			if (cinematicQuads[BOOT2]->getPosition().x < 0) cinematicQuads[BOOT2]->render();
			else if (timeElapsed < 38.f) cinematicSprites[RICHT_DUST]->render();
			if (timeElapsed >= 38.f) cinematicSprites[RICHT_WHIP]->render();
			if (film.front().bg->animation() == 3) cinematicSprites[RICHT_FACE]->render();
		}
		else if (filmId == RICHTER_WHIP3)
		{
			colorBackground->render();
			film.front().bg->render();
			if (timeElapsed >= 47.25f && !cinematicSprites[PURPLE_FIRE]->animationEnded()) cinematicSprites[PURPLE_FIRE]->render();
			cinematicQuads[RICHTER2]->render();
			blackBar48px->setPosition(glm::vec2(0));
			blackBar48px->render();
			blackBar48px->setPosition(glm::vec2(0, SCREEN_HEIGHT - 48));
			blackBar48px->render();
		}
		else if (filmId == RICHTER_SUS)
		{
			film.front().bg->render();
			cinematicSprites[RICHTER_EYES]->render();
		}
		else if (filmId == RICHTER_AMBUSHED)
		{
			cinematicQuads[SKELETON_AMBUSH]->render();
			cinematicQuads[SKELETON_AMBUSH]->incPosition(glm::vec2(164, 0));
			shader->setUniform1f("frameWidth", 1.f);
			shader->setUniform1i("flip", true);
			cinematicQuads[SKELETON_AMBUSH]->render();
			cinematicQuads[SKELETON_AMBUSH]->incPosition(glm::vec2(-164, 0));
			shader->setUniform1i("flip", false);
			cinematicQuads[RICHTER3]->render();
			film.front().bg->render();
		}
		else if (filmId == RICHTER_ULT1)
		{
			film.front().bg->render();
			cinematicQuads[RIGHT_HAND]->render();
			cinematicQuads[LEFT_HAND]->render();
			blackBar32px->render();
		}
		else if (filmId == RICHTER_ULT2)
		{
			renderCross(0);
			film.front().bg->render();
			cinematicSprites[WHIRLWIND]->render();
			cinematicSprites[PURPLE_FIRE2]->render();
			renderCross(1);
			shader->setUniform1f("paletteIndexOffset", 0.f + crossBlack[2] * 0.5f);
			cinematicQuads[BIG_CROSS]->render();
			shader->setUniform1f("paletteIndexOffset", 0.f);
		}
		else film.front().bg->render();
		if (blackScreenAlpha > 0) blackScreen->render();
	}
}

float CoolIntro::setEndTime() const
{
	return 64.f;
}

void CoolIntro::updateCross(int deltaTime, int crossNum)
{
	cinematicQuads[SMALL_CROSS + crossNum]->incPosition(glm::vec2(0, -crossSpeeds[crossNum]));
	if (cinematicQuads[SMALL_CROSS + crossNum]->getPosition().y <= crossYlimits[crossNum])
	{
		cinematicQuads[SMALL_CROSS + crossNum]->incPosition(glm::vec2(0, SCREEN_HEIGHT * 2));
	}
	crossColorTimer[crossNum] += deltaTime;
	if (crossColorTimer[crossNum] > TIME_BETWEEN_CROSS_COLOR)
	{
		crossColorTimer[crossNum] = 0;
		crossBlack[crossNum] = !crossBlack[crossNum];
	}
}

void CoolIntro::renderCross(int crossNum)
{
	shader->setUniform1f("paletteIndexOffset", 0.f + crossBlack[crossNum] * 0.5f);
	cinematicQuads[SMALL_CROSS + crossNum]->render();
	cinematicQuads[SMALL_CROSS + crossNum]->incPosition(crossOffsets[crossNum]);
	shader->setUniform1f("frameWidth", 0.0625f);
	shader->setUniform2f("texCoordDispl", 0.5f + 0.125f*crossNum, 0.f);
	shader->setUniform1i("flip", true);
	if (crossColorTimer[crossNum] * 2 > TIME_BETWEEN_CROSS_COLOR) shader->setUniform1f("paletteIndexOffset", 0.f + !crossBlack[crossNum] * 0.5f);
	cinematicQuads[SMALL_CROSS + crossNum]->render();
	cinematicQuads[SMALL_CROSS + crossNum]->incPosition(-crossOffsets[crossNum]);
	shader->setUniform2f("texCoordDispl",0.f, 0.f);
	shader->setUniform1i("flip", false);
	shader->setUniform1f("paletteIndexOffset", 0.f);
}

int CoolIntro::getColorIndex(int range, float freq)
{
	return int((sin(freq) + 1) / 2 * range);
}
