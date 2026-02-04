#pragma once
#ifndef _COOLINTRO_INCLUDE
#define _COOLINTRO_INCLUDE

#include "Cinematic.h"
class CoolIntro : public Cinematic
{
public:
	void render() override;

protected:
	void initChild() override;
	void filmUpdate(int deltaTime) override;
	float setEndTime() const override;

private:
	int getColorIndex(int range, float freq);

private:
	enum BGIDs
	{
		MAP, RIDING, CASTLEVANIA, CEMENTERY, GIANT, PEOPLE, ANNETTE, GIANTS,
		RICHTER_BOOT, RICHTER_ARRIVE, RICHTER_READY, RICHTER_WHIP1, RICHTER_WHIP2, RICHTER_WHIP3, RICHTER_LOOK, RICHTER_AMBUSH, RICHTER_ULT1, RICHTER_ULT2
	};
	enum Quads
	{
		HAND, RICHTER1, SKY, TREES_FAR, TREES_CLOSE, CAR_TOP, CAR, GRASS, SKELETON_CEMENTERY, GIANT_QUAD, ANNETTE_EYES, ANNETTE_MOUTH, BOOT, BOOT2, COUNT
	};
	enum Sprites
	{
		WHEELS, HORSES, RICHT_HANDS, RICHT_DUST, RICHT_WHIP, RICHT_FACE, COUNT2
	};
	TexturedQuad* blackBar48px;
	TexturedQuad* blackBar32px;
	//TexturedQuad* blackBox128x112;
	TexturedQuad* redBackground;
	TexturedQuad* bolts[2];
	TexturedQuad* introQuads[COUNT];
	Sprite* introSprites[COUNT2];

	float bgXScroll = 0.f;
	float boltTimer = 2.f;
	float richter1Alpha = 1.f;
	float startY;
	float cameraX = 0.f;
	int boltDuration;
	int annetteEyesDuration;
	int shakeAngleStep;
	int shakeDist;
	int shakeAngle = 0;
	bool shaking = false;
	bool shaked = false;
	bool renderBigBolt = false;
	bool annetteBlinked = false;
};

#endif // !_COOLINTRO_INCLUDE