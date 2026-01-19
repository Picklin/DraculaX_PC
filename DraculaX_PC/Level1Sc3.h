#pragma once
#ifndef _LEVEL1SC3_INCLUDE
#define _LEVEL1SC3_INCLUDE

#include "Scene.h"
#include "Wyvern.h"

class Level1Sc3 : public Scene
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

private:
	Wyvern* wyvern = nullptr;
	TexturedQuad* cielo;
	TexturedQuad* castlevania;
	TexturedQuad* muralla2;
	TexturedQuad* muralla;
	bool bossAppeares = false;
	
};

#endif // !_LEVEL1SC3_INCLUDE
