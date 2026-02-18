#pragma once
#ifndef _CINEMATICFACTORY_INCLUDE
#define _CINEMATICFACTORY_INCLUDE

#include "Cinematic.h"

class CinematicFactory
{
private:
	CinematicFactory() {};

public:
	static CinematicFactory& instance()
	{
		static CinematicFactory CF;
		return CF;
	}
	Cinematic* createCinematic(ShaderProgram& shader, const string& scriptPath, int cinematicId);
	Cinematic* createCinematic(ShaderProgram& shader, int cinematicId);

};

#endif // !_CINEMATICFACTORY_INCLUDE
