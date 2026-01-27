#pragma once
#ifndef _INTRO_INCLUDE
#define _INTRO_INCLUDE

#include "Cinematic.h"

class Intro : public Cinematic
{
public:
	void update(int deltaTime) override;
	void render() override;

protected:
	void init(ShaderProgram& shader) override;

private:
	void calcEaseIn(float& valToInc, float startVal, float targetVal, float factor);

private:
	enum BGIDs
	{
		CASTLE_OUTSIDE, CASTLE_INSIDE, GIRL_ON_COFFIN, SOLDIERS, STAB, COFFIN_BREAK, DRACULA_APPEAR, CASTLEVANIA_CLOSE, CASTLEVANIA_FAR
	};
	Sprite* coffin;
	TexturedQuad* bolts[2];
	float cameraX = 0.f;
	float cameraY = 0.f;
	float thunderCooldown = 0.f;
	float boltDuration;
	float stabVelocityFactor;
	bool renderBigBolt;
};

#endif // !_INTRO_INCLUDE