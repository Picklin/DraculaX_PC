#pragma once
#ifndef _LEVEL1SC2_INCLUDE
#define _LEVEL1SC2_INCLUDE

#include "Scene.h"
class Level1Sc2 : public Scene
{
public:
	void init(Player& player, GUI& gui, ShaderProgram& spriteShader, ShaderProgram& basicShader) override;
	void update(int deltaTime) override;
	void render() override;

protected:
	TileMap* setTileMap() override;
	TileMap* setPlatformMap() override;
	TileMap* setStairsMap() override;
	void initItems() override;
	void initActors(Player* player) override;
	void updateCamera() override;
	const pair<int, int> setNewLevelAndScene() const override;
};

#endif // !_LEVEL1SC2_INCLUDE