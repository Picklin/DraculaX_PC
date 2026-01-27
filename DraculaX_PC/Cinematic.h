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
	static Cinematic* createCinematic(ShaderProgram& shader, const string& scriptPath, int cinematicId);
	static Cinematic* createCinematic(ShaderProgram& shader, int cinematicId);
	virtual void update(int deltaTime) = 0;
	virtual void render() = 0;
	bool ended() const;

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
	struct Bg
	{
		Sprite* bg;
		int id;
		float time;
		float duration;
		float alpha = 1.f;
	};
	Text dialogueTxt;
	queue<Line> script;
	queue<Bg> film;
	vector<Sprite*> movingElems;
	vector<TexturedQuad*> staticElems;
	TexturedQuad* blackScreen;
	ShaderProgram* shader = nullptr;
	float timeElapsed = 0.f;
	float endTime;
	bool renderSubtitles = false;
	bool renderBg = false;
};

#endif // !_CINEMATIC_INCLUDE