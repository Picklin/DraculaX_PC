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
	virtual ~Cinematic();
	void init(ShaderProgram& shader);
	void loadScript(const string& scriptPath);
	void update(int deltaTime);
	virtual void render() = 0;
	bool ended() const;

protected:
	virtual void initChild() = 0;
	virtual void filmUpdate(int deltaTime) = 0;
	virtual float setEndTime() const = 0;

public:
	enum CinematicType
	{
		INTRO, COOL_INTRO, COUNT
	};

protected:
	struct Line
	{
		wstring line;
		float time;
		float duration;
	};
	struct Bg
	{
		Sprite* bg;
		int id;
		float time;
		float duration;
		float alpha;
	};
	queue<Line> script;
	queue<Bg> film;
	vector<TexturedQuad*> cinematicQuads;
	vector<Sprite*> cinematicSprites;
	//vector<Sprite*> movingElems;
	//vector<TexturedQuad*> staticElems;
	TexturedQuad* blackScreen = nullptr;
	ShaderProgram* shader = nullptr;
	Text* dialogueTxt = nullptr;
	float timeElapsed = 0.f;
	float endTime = 0.f;
	bool renderSubtitles = false;
	bool renderBg = false;
};

#endif // !_CINEMATIC_INCLUDE