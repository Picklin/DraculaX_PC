#pragma once
#ifndef _LEVEL1SC1_INCLUDE
#define _LEVEL1SC1_INCLUDE

#include "Scene.h"
#include "Text.h"

class Level1Sc1 : public Scene
{
public:
	void init(Player& player, ShaderProgram& spriteShader, ShaderProgram& basicShader);
	void update(int deltaTime);
	void render();

protected:
	TileMap* setTileMap();
	TileMap* setPlatformMap();
	TileMap* setStairsMap();
	void setStairsInfo();
	void initItems();
	void initActors(Player* player);
	void updateCamera();
	const pair<int, int> setNewLevelAndScene() const;
	Text text;
};

#endif // !_LEVEL1SC1_INCLUDE