#pragma once
#ifndef _LEVEL1SC1_INCLUDE
#define _LEVEL1SC1_INCLUDE

#include "Scene.h"
#include "Text.h"

class Level1Sc1 : public Scene
{
public:
	~Level1Sc1() {
		trunk->free();
		delete trunk;
	}
	void init(Player& player, GUI& gui, ShaderProgram& spriteShader, ShaderProgram& basicShader);
	void update(int deltaTime);
	void render();

protected:
	TileMap* setTileMap();
	TileMap* setPlatformMap();
	TileMap* setStairsMap();
	void initItems();
	void initActors(Player* player);
	void updateCamera();
	const pair<int, int> setNewLevelAndScene() const;
	Text text;
	Text text2;
	Text text3;

private:
	TexturedQuad* trunk;
};

#endif // !_LEVEL1SC1_INCLUDE