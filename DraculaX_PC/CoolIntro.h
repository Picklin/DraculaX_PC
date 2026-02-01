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
	enum BGIDs
	{
		MAP, HORSES, CEMENTERY, GIANT, PEOPLE, ANNETTE, GIANTS,
		RICHTER_BOOT, RICHTER_ARRIVE, RICHTER_READY, RICHTER_WHIP1, RICHTER_WHIP2, RICHTER_WHIP3, RICHTER_LOOK, RICHTER_AMBUSH, RICHTER_ULT1, RICHTER_ULT2
	};
	TexturedQuad* bolts[2];

};

#endif // !_COOLINTRO_INCLUDE