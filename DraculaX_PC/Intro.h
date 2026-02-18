#pragma once
#ifndef _INTRO_INCLUDE
#define _INTRO_INCLUDE

#include "Cinematic.h"

class Intro : public Cinematic
{
public:
	~Intro();
	void render() override;

protected:
	void initChild() override;
	void filmUpdate(int deltaTime) override;
	float setEndTime() const override;

private:
	void calcEaseIn(float& valToInc, float startVal, float targetVal, float factor);

private:
	enum BGIDs
	{
		CASTLE_OUTSIDE, CASTLE_INSIDE, GIRL_ON_COFFIN, SOLDIERS, STAB, COFFIN_BREAK, DRACULA_APPEAR, CASTLEVANIA_CLOSE, CASTLEVANIA_FAR
	};
	enum Quads
	{
		COFFIN, COUNTQUADS
	};
	enum Sprites
	{
		DRACULA, BAT, LIGHTS, TITLE_BG, TITLE, COUNTSPRITES
	};
	TexturedQuad* bolts[4];
	TexturedQuad* blood[2];
	float cameraX = 0.f;
	float cameraY = 0.f;
	float thunderCooldown = 0.f;
	float boltDuration;
	float stabVelocityFactor;
	float batPosition = 0.f;
	bool renderBigBolt;
	bool renderBlood;
	bool titleShowed = false;
};

#endif // !_INTRO_INCLUDE