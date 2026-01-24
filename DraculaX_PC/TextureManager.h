#pragma once
#ifndef _TEXTUREMANAGER_INCLUDE
#define _TEXTUREMANAGER_INCLUDE

#include <unordered_map>
#include "Texture.h"

class TextureManager
{
private:
	TextureManager();

public:
	static TextureManager& instance();
	void addTexture(const string& entityName, Texture* tex);
	void removeTexture(const string& entityName);
	//precondition: entity name exists as a key in textures
	Texture* getTexture(const string& entityName);
	bool exists(const string& entityName);
	void free();

private:
	unordered_map<string, Texture*> textures;
};

#endif // !_TEXTUREMANAGER_INCLUDE