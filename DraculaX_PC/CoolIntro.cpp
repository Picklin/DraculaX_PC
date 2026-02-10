#include "CoolIntro.h"
#include "TextureManager.h"
#include "Game.h"

namespace {
	const float redColors[3] = { 72 / 255.f, 108 / 255.f, 144 / 255.f };
	const float amplitude = 16.f;
	const float frequency = 5.f;
}

void CoolIntro::initChild()
{
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
	introQuads[HAND] = TexturedQuad::createTexturedQuad(glm::vec2(0.f, 0.f), glm::vec2(0.5f, 0.35f), *handTex, *shader);
	introQuads[HAND]->setPosition(glm::vec2(-72, -128));
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
	introQuads[RICHTER1] = TexturedQuad::createTexturedQuad(glm::vec2(0.125f, 0.f), glm::vec2(0.25f,0.25f), *bgTex, *shader);
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
	introQuads[SKELETON_CEMENTERY] = TexturedQuad::createTexturedQuad(glm::vec2(0.f), glm::vec2(1.f), *skelTex, *shader);
	introQuads[SKELETON_CEMENTERY]->setPosition(glm::vec2(32, SCREEN_HEIGHT));
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
	introQuads[ANNETTE_EYES] = TexturedQuad::createTexturedQuad(glm::vec2(0.5f, 0.f), glm::vec2(0.65f, 0.05f), *handTex, *shader);
	introQuads[ANNETTE_EYES]->setPosition(annettePos + glm::vec2(32 + 6, 48));
	introQuads[ANNETTE_MOUTH] = TexturedQuad::createTexturedQuad(glm::vec2(0.5f, 0.1f), glm::vec2(0.6f, 0.15f), *handTex, *shader);
	introQuads[ANNETTE_MOUTH]->setPosition(annettePos + glm::vec2(32 + 6, 48 + 32));
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
	introQuads[BOOT] = TexturedQuad::createTexturedQuad(glm::vec2(0.7f, 0.f), glm::vec2(1.f, 0.5f), *handTex, *shader);
	introQuads[BOOT]->setPosition(glm::vec2(94, -160));
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
	introSprites[RICHT_HANDS] = Sprite::createSprite(fullScreen / 2, offset2, richterParts, shader);
	introSprites[RICHT_HANDS]->setNumberAnimations(2);
	introSprites[RICHT_HANDS]->setAnimationSpeed(0, 15);
	introSprites[RICHT_HANDS]->animatorX(0, 3, 0.f, 0.25f, 0.f);
	introSprites[RICHT_HANDS]->setAnimationSpeed(1, 15);
	introSprites[RICHT_HANDS]->addKeyframe(1, glm::vec2(0.75f, 0.f));
	introSprites[RICHT_HANDS]->animatorX(1, 2, 0.f, 0.25f, 0.25f);
	introSprites[RICHT_HANDS]->changeAnimation(0);
	introSprites[RICHT_DUST] = Sprite::createSprite(fullScreen / 2, offset2, richterParts, shader);
	introSprites[RICHT_DUST]->setNumberAnimations(2);
	introSprites[RICHT_DUST]->setAnimationSpeed(0, 15);
	introSprites[RICHT_DUST]->animatorX(0, 2, 0.5f, 0.25f, 0.25f);
	introSprites[RICHT_DUST]->addKeyframe(0, glm::vec2(0.f, 0.5f));
	introSprites[RICHT_DUST]->setAnimationSpeed(1, 0);
	introSprites[RICHT_DUST]->addKeyframe(1, glm::vec2(0.5f, 0.75f));
	introSprites[RICHT_DUST]->setTransition(0, 1);
	introSprites[RICHT_DUST]->changeAnimation(0);
	introSprites[RICHT_DUST]->setPosition(glm::vec2(0, 112));
	introSprites[RICHT_WHIP] = Sprite::createSprite(fullScreen / 2, offset2, richterParts, shader);
	introSprites[RICHT_WHIP]->setNumberAnimations(2);
	introSprites[RICHT_WHIP]->setAnimationSpeed(0, 0);
	introSprites[RICHT_WHIP]->addKeyframe(0, glm::vec2(0.f, 0.75f));
	introSprites[RICHT_WHIP]->setAnimationSpeed(1, 0);
	introSprites[RICHT_WHIP]->addKeyframe(1, glm::vec2(0.25f, 0.75f));
	introSprites[RICHT_WHIP]->changeAnimation(0);
	introSprites[RICHT_WHIP]->setPosition(glm::vec2(128, 16));
	introSprites[RICHT_FACE] = Sprite::createSprite(fullScreen / 2, offset2, richterParts, shader);
	introSprites[RICHT_FACE]->setNumberAnimations(3);
	introSprites[RICHT_FACE]->setAnimationSpeed(0, 0);
	introSprites[RICHT_FACE]->addKeyframe(0, glm::vec2(0.25f, 0.5f));
	introSprites[RICHT_FACE]->setAnimationSpeed(1, 10);
	introSprites[RICHT_FACE]->addKeyframe(1, glm::vec2(0.5f, 0.5f));
	introSprites[RICHT_FACE]->setAnimationSpeed(2, 0);
	introSprites[RICHT_FACE]->addKeyframe(2, glm::vec2(0.75f, 0.5f));
	introSprites[RICHT_FACE]->setTransition(1, 2);
	introSprites[RICHT_FACE]->changeAnimation(0);
	introSprites[RICHT_FACE]->setPosition(glm::vec2(128, 112));
	Texture* bootTex = new Texture();
	bootTex->loadFromFile("images/cinematics/intro_cool/boot.png", TEXTURE_PIXEL_FORMAT_RGBA);
	TextureManager::instance().addTexture("boot", bootTex);
	introQuads[BOOT2] = TexturedQuad::createTexturedQuad(glm::vec2(0), glm::vec2(1.f), *bootTex, *shader);
	introQuads[BOOT2]->setPosition(glm::vec2(-SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2));
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
	introSprites[PURPLE_FIRE] = Sprite::createSprite(glm::ivec2(80), glm::vec2(0.2f, 1.f), pfire, shader);
	introSprites[PURPLE_FIRE]->setNumberAnimations(1);
	introSprites[PURPLE_FIRE]->setAnimationSpeed(0, 10);
	introSprites[PURPLE_FIRE]->animatorX(0, 5, 0.f, 0.2f, 0.f);
	introSprites[PURPLE_FIRE]->changeAnimation(0);
	introSprites[PURPLE_FIRE]->setPosition(glm::vec2(49, 48));
	introQuads[RICHTER2] = TexturedQuad::createTexturedQuad(glm::vec2(0.4375f, 0.75f), glm::vec2(0.5f, 1.f), *bgTex, *shader);
	introQuads[RICHTER2]->setPosition(glm::vec2(SCREEN_WIDTH * 2 + SCREEN_WIDTH/2, 0));
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
	introSprites[RICHTER_EYES] = Sprite::createSprite(fullScreen, glm::vec2(0.25f, 1.f), richtEyes, shader);
	introSprites[RICHTER_EYES]->setNumberAnimations(2);
	introSprites[RICHTER_EYES]->setAnimationSpeed(0, 10);
	introSprites[RICHTER_EYES]->animatorX(0, 2, 0.f, 0.25f, 0.f);
	introSprites[RICHTER_EYES]->setAnimationSpeed(1, 0);
	introSprites[RICHTER_EYES]->addKeyframe(1, glm::vec2(0.5f, 0.f));
	introSprites[RICHTER_EYES]->setTransition(0, 1);
	introSprites[RICHTER_EYES]->changeAnimation(0);
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
	introQuads[RICHTER3] = TexturedQuad::createTexturedQuad(glm::vec2(0), glm::vec2(1), *richtAmbushed, *shader);
	introQuads[SKELETON_AMBUSH] = TexturedQuad::createTexturedQuad(glm::vec2(0), glm::vec2(1), *skelAmbush, *shader);
	introQuads[RICHTER3]->setPosition(glm::vec2(79, 56));
	introQuads[SKELETON_AMBUSH]->setPosition(glm::vec2(21, 109));
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
	introQuads[RIGHT_HAND] = TexturedQuad::createTexturedQuad(glm::vec2(0.f, 0.f), glm::vec2(0.5f, 1.f), *hands, *shader);
	introQuads[LEFT_HAND] = TexturedQuad::createTexturedQuad(glm::vec2(0.5f, 0.f), glm::vec2(1.f, 1.f), *hands, *shader);
	introQuads[RIGHT_HAND]->setPosition(glm::vec2(50, 142));
	introQuads[LEFT_HAND]->setPosition(glm::vec2(142, 142));
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
	introSprites[PURPLE_FIRE2] = Sprite::createSprite(glm::ivec2(SCREEN_WIDTH, SCREEN_HEIGHT), glm::vec2(0.25f, 0.5f), elems, shader);
	introSprites[PURPLE_FIRE2]->setNumberAnimations(1);
	introSprites[PURPLE_FIRE2]->setAnimationSpeed(0, 10);
	introSprites[PURPLE_FIRE2]->animatorX(0, 3, 0.f, 0.25f, 0.5f);
	introSprites[PURPLE_FIRE2]->changeAnimation(0);
	introSprites[WHIRLWIND] = Sprite::createSprite(glm::ivec2(SCREEN_WIDTH, SCREEN_HEIGHT), glm::vec2(0.25f, 0.5f), elems, shader);
	introSprites[WHIRLWIND]->setNumberAnimations(1);
	introSprites[WHIRLWIND]->setAnimationSpeed(0, 30);
	introSprites[WHIRLWIND]->animatorX(0, 4, 0.25f, 0.25f, 0.f);
	introSprites[WHIRLWIND]->changeAnimation(0);
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
		if (introQuads[HAND]->getPosition().x < 16) introQuads[HAND]->incPosition(glm::vec2(4, 8));
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
				introQuads[HAND]->setPosition(glm::vec2(introQuads[HAND]->getPosition().x, positionY + 48));
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
			introQuads[RICHTER1]->setAlpha(richter1Alpha);
		}
		bgXScroll += 0.0125f;
		introSprites[WHEELS]->update(deltaTime);
		introSprites[HORSES]->update(deltaTime);
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
		else if (timeElapsed >= 19 && introQuads[SKELETON_CEMENTERY]->getPosition().y > 80) introQuads[SKELETON_CEMENTERY]->incPosition(glm::vec2(0, -2));
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
		if (introQuads[ANNETTE_EYES]->getPosition().y > 80)
		{
			glm::vec2 inc(0, -.5f);
			introQuads[ANNETTE_EYES]->incPosition(inc);
			introQuads[ANNETTE_MOUTH]->incPosition(inc);
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
	else if (filmId == RICHTER_BOOT && introQuads[BOOT]->getPosition().y < 32)
	{
		introQuads[BOOT]->incPosition(glm::vec2(0, 8));
	}
	else if (filmId == RICHTER_ARRIVE)
	{
		Sprite* sp = film.front().bg;
		if (timeElapsed > 34.5f && sp->animation() == 0) sp->changeAnimation(1);
		else if (timeElapsed > 35.f && sp->animation() == 1) sp->changeAnimation(2);
	}
	else if (filmId == RICHTER_READY)
	{
		introSprites[RICHT_HANDS]->update(deltaTime);
		int anim = film.front().bg->animation();
		float bootPosX = introQuads[BOOT2]->getPosition().x;
		if (timeElapsed >= 37.f && bootPosX < 0)
		{
			if (introSprites[RICHT_HANDS]->animation() == 0)
			{
				//cout << "cambio de animacion de las manos" << endl;
				introSprites[RICHT_HANDS]->changeAnimation(1);
				cameraX = 0;
				colorBackground->setColor(glm::vec3(0, 0, 36/255.f));
			}
			else
			{
				//cout << "apareciendo la bota" << endl;
				introQuads[BOOT2]->incPosition(glm::vec2(4, 0));	
			}
		}
		else if (timeElapsed >= 39.f)
		{
			//cout << "cara" << endl;
			if (timeElapsed >= 39.5f && introSprites[RICHT_FACE]->animation() == 0) introSprites[RICHT_FACE]->changeAnimation(1);
			if (anim != 3) film.front().bg->changeAnimation(3);
			else introSprites[RICHT_FACE]->update(deltaTime);
		}	
		else if (timeElapsed >= 38.f)
		{
			//cout << "whip ready" << endl;
			if (introSprites[RICHT_WHIP]->getPosition().y > 0) introSprites[RICHT_WHIP]->incPosition(glm::vec2(0, -2));
			else if (introSprites[RICHT_WHIP]->animation() != 1) introSprites[RICHT_WHIP]->changeAnimation(1);
			if (anim != 2) film.front().bg->changeAnimation(2);
		}
		else if (bootPosX == 0)
		{
			//cout << "entro, aparece la bota, momento: " << timeElapsed << endl;
			if (anim != 1) film.front().bg->changeAnimation(1);
			introSprites[RICHT_DUST]->update(deltaTime);
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
		if (introQuads[RICHTER2]->getPosition().x > 128) introQuads[RICHTER2]->incPosition(glm::vec2(-4, 0));
		else if (timeElapsed >= 47.25f && !introSprites[PURPLE_FIRE]->animationEnded()) introSprites[PURPLE_FIRE]->update(deltaTime);
	}
	else if (filmId == RICHTER_SUS && timeElapsed >= 48.75f)
	{
		introSprites[RICHTER_EYES]->update(deltaTime);
	}
	else if (filmId == RICHTER_AMBUSHED && film.front().bg->getPosition().x < 0)
	{
		introQuads[SKELETON_AMBUSH]->incPosition(glm::vec2(-1, 0));
		film.front().bg->incPosition(glm::vec2(2, 0));
	}
	else if (filmId == RICHTER_ULT1 && introQuads[RIGHT_HAND]->getPosition().y > (SCREEN_HEIGHT - 32 - 64))
	{
		introQuads[RIGHT_HAND]->incPosition(glm::vec2(.5, -.5));
		introQuads[LEFT_HAND]->incPosition(glm::vec2(-.5, -.5));
	}
	else if (filmId == RICHTER_ULT2)
	{
		if (timeElapsed >= 57.f && film.front().alpha > 0)
		{
			blackScreenAlpha += deltaTime / 3000.f;
			blackScreen->setAlpha(blackScreenAlpha);
		}
		introSprites[PURPLE_FIRE2]->update(deltaTime);
		introSprites[WHIRLWIND]->update(deltaTime);
	}
	boltTimer -= deltaTime / 1000.f;
}

void CoolIntro::render()
{
	if (renderBg)
	{
		int filmId = film.front().id;
		if (filmId == MAP)
		{
			film.front().bg->render();
			introQuads[HAND]->render();
			blackBar48px->render();
		}
		else if (filmId == HORSES)
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
			introQuads[RICHTER1]->render();
		}
		else if (filmId == CASTLEVANIA)
		{
			film.front().bg->render();
			if (boltTimer <= 0) bolts[renderBigBolt]->render();
		}
		else if (filmId == CEMENTERY)
		{
			film.front().bg->render();
			introQuads[SKELETON_CEMENTERY]->render();
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
			if (introQuads[ANNETTE_EYES]->getPosition().y > 80)
			{
				introQuads[ANNETTE_EYES]->render();
				introQuads[ANNETTE_MOUTH]->render();
			}
			else if (annetteEyesDuration > 0) introQuads[ANNETTE_EYES]->render();
			blackBar32px->setPosition(glm::vec2(0, 0));
			blackBar32px->render();
			blackBar32px->setPosition(glm::vec2(0, SCREEN_HEIGHT - 32));
			blackBar32px->render();
		}
		else if (filmId == RICHTER_BOOT)
		{
			film.front().bg->render();
			introQuads[BOOT]->render();
			blackBar32px->setPosition(glm::vec2(0, 0));
			blackBar32px->render();
			blackBar32px->setPosition(glm::vec2(0, SCREEN_HEIGHT - 32));
			blackBar32px->render();
		}
		else if (filmId == RICHTER_READY)
		{
			film.front().bg->render();
			introSprites[RICHT_HANDS]->render();
			if (introQuads[BOOT2]->getPosition().x < 0) introQuads[BOOT2]->render();
			else if (timeElapsed < 38.f) introSprites[RICHT_DUST]->render();
			if (timeElapsed >= 38.f) introSprites[RICHT_WHIP]->render();
			if (film.front().bg->animation() == 3) introSprites[RICHT_FACE]->render();
		}
		else if (filmId == RICHTER_WHIP3)
		{
			colorBackground->render();
			film.front().bg->render();
			if (timeElapsed >= 47.25f && !introSprites[PURPLE_FIRE]->animationEnded()) introSprites[PURPLE_FIRE]->render();
			introQuads[RICHTER2]->render();
			blackBar48px->setPosition(glm::vec2(0));
			blackBar48px->render();
			blackBar48px->setPosition(glm::vec2(0, SCREEN_HEIGHT - 48));
			blackBar48px->render();
		}
		else if (filmId == RICHTER_SUS)
		{
			film.front().bg->render();
			introSprites[RICHTER_EYES]->render();
		}
		else if (filmId == RICHTER_AMBUSHED)
		{
			introQuads[SKELETON_AMBUSH]->render();
			introQuads[SKELETON_AMBUSH]->incPosition(glm::vec2(164, 0));
			shader->setUniform1f("frameWidth", 1.f);
			shader->setUniform1i("flip", true);
			introQuads[SKELETON_AMBUSH]->render();
			introQuads[SKELETON_AMBUSH]->incPosition(glm::vec2(-164, 0));
			shader->setUniform1i("flip", false);
			introQuads[RICHTER3]->render();
			film.front().bg->render();
		}
		else if (filmId == RICHTER_ULT1)
		{
			film.front().bg->render();
			introQuads[RIGHT_HAND]->render();
			introQuads[LEFT_HAND]->render();
			blackBar32px->render();
		}
		else if (filmId == RICHTER_ULT2)
		{
			film.front().bg->render();
			introSprites[WHIRLWIND]->render();
			introSprites[PURPLE_FIRE2]->render();
		}
		else film.front().bg->render();
		if (blackScreenAlpha > 0) blackScreen->render();
	}
}

float CoolIntro::setEndTime() const
{
	return 64.f;
}

int CoolIntro::getColorIndex(int range, float freq)
{
	return int((sin(freq) + 1) / 2 * range);
}
