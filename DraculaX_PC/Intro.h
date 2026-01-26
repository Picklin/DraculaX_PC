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
	enum BGIDs
	{
		CASTLE_OUTSIDE, CASTLE_INSIDE, GIRL_ON_COFFIN, SOLDIERS, STAB, COFFIN_BREAK, DRACULA_APPEAR, CASTLEVANIA_CLOSE, CASTLEVANIA_FAR
	};

};

#endif // !_INTRO_INCLUDE