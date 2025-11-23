#pragma once
#ifndef _LEVEL1SC1_INCLUDE
#define _LEVEL1SC1_INCLUDE

#include "Scene.h"

class Level1Sc1 : public Scene
{
public:
	void init(Player& player, ShaderProgram& spriteShader, ShaderProgram& basicShader);
	void update(int deltaTime);
	void render();

protected:
	TileMap* setTileMap();
	TileMap* setPlatformMap();
	void initItems();
	void initActors(Player* player);
	void updateCamera();
	const pair<int, int> setNewLevelAndScene() const;
};

#endif // !_LEVEL1SC1_INCLUDE