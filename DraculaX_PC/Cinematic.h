#pragma once
#ifndef _CINEMATIC_INCLUDE
#define _CINEMATIC_INCLUDE

#include "queue"
#include "Sprite.h"
#include "TexturedQuad.h"
#include "Text.h"

class Cinematic
{
public:
	virtual void init(ShaderProgram& shader) = 0;
	virtual void update(int deltaTime) = 0;
	virtual void render() = 0;

protected:
	struct Line
	{
		string line;
		float time;
		float duration;
	};
	struct CurrentBg
	{
		int bgId;
		float time;
		float duration;
	};
	Text dialogue;
	const vector<Line> dialogueStr;
	queue<Sprite*> movingBgs;
	queue<TexturedQuad*> staticBgs;
	vector<Sprite*> movingElems;
	vector<TexturedQuad*> staticElems;
	ShaderProgram* shader;
	float timeElapsed = 0.f;
};

#endif // !_CINEMATIC_INCLUDE