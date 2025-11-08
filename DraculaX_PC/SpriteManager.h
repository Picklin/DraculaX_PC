#pragma once
#ifndef _SPRITEMANAGER_INCLUDE
#define _SPRITEMANAGER_INCLUDE

#include <unordered_map>
#include <string>
#include "AnimKeyframes.h"

class SpriteManager
{
private:
	SpriteManager() {}

public:
	static SpriteManager& instance();
	void addAnimations(const string& entityName, const vector<AnimKeyframes>& animations);
	void removeAnimations(const string& entityName);
	//precondition: entityName exists in animationsCollection
	vector<AnimKeyframes> getAnimations(const string& entityName);
	bool exists(const string& entityName);
	void free();

private:
	unordered_map<string, vector<AnimKeyframes>> animationsCollection;
};

#endif // !_SPRITEMANAGER_INCLUDE