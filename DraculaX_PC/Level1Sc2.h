#pragma once
#ifndef _LEVEL1SC2_INCLUDE
#define _LEVEL1SC2_INCLUDE

#include "Scene.h"
class Level1Sc2 : public Scene
{
public:
	~Level1Sc2() {
		bg1->free();
		delete bg1;
		bg2->free();
		delete bg2;
		bg3->free();
		delete bg3;
		bg4->free();
		delete bg4;
	}
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

private:
	TexturedQuad* bg1;
	TexturedQuad* bg2;
	TexturedQuad* bg3;
	TexturedQuad* bg4;
	TexturedQuad* fireEffect;

	int fireColorTimer = 0;
	int fireColorIndex = 0;
};

#endif // !_LEVEL1SC2_INCLUDE