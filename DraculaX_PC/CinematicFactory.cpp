
#include "CinematicFactory.h"
#include "Intro.h"
#include "CoolIntro.h"

Cinematic* CinematicFactory::createCinematic(ShaderProgram& shader, const string& scriptPath, int cinematicId)
{
	Cinematic* c;
	 switch (cinematicId)
	 {
	 case Cinematic::INTRO:
		 c = new Intro();
		 break;
	 case Cinematic::COOL_INTRO:
		 c = new CoolIntro();
		 break;
	 default:
		 return nullptr;
	 }
	 c->init(shader);
	 c->loadScript(scriptPath);
	 return c;
}

Cinematic* CinematicFactory::createCinematic(ShaderProgram& shader, int cinematicId)
{
	Cinematic* c;
	switch (cinematicId)
	{
	case Cinematic::INTRO:
		c = new Intro();
		break;
	case Cinematic::COOL_INTRO:
		c = new CoolIntro();
		break;
	default:
		return nullptr;
	}
	c->init(shader);
	return c;
}
