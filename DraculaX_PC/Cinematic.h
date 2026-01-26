#pragma once
#ifndef _CINEMATIC_INCLUDE
#define _CINEMATIC_INCLUDE

#include "queue"
#include "Sprite.h"
#include "TexturedQuad.h"
#include "Text.h"

#define SCREEN_WIDTH	256
#define SCREEN_HEIGHT	224

class Cinematic
{
public:
	static Cinematic* createCinematic(ShaderProgram& shader, const string& scriptPath, int cinematicId);
	static Cinematic* createCinematic(ShaderProgram& shader, int cinematicId);
	virtual void update(int deltaTime) = 0;
	virtual void render() = 0;

protected:
	virtual void init(ShaderProgram& shader) = 0;
	Cinematic() {}

private:
	void loadScript(const string& scriptPath);

public:
	enum CinematicType
	{
		INTRO, COUNT
	};

protected:
	struct Line
	{
		wstring line;
		float time;
		float duration;
	};
	struct CurrentBg
	{
		int bgId;
		float time;
		float duration;
	};
	Text dialogueTxt;
	queue<Line> script;
	queue<Sprite*> movingBgs;
	queue<TexturedQuad*> staticBgs;
	vector<Sprite*> movingElems;
	vector<TexturedQuad*> staticElems;
	TexturedQuad* blackScreen;
	ShaderProgram* shader = nullptr;
	float timeElapsed = 0.f;
	bool renderSubtitles = false;
};

#endif // !_CINEMATIC_INCLUDE