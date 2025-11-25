#ifndef _SCENETESTING_INCLUDE
#define _SCENETESTING_INCLUDE

#include "Scene.h"

// Scene contains all the entities of our game.
// It is responsible for updating and render them.

#define INIT_PLAYER_X_TILES 8
#define INIT_PLAYER_Y_TILES 16


class SceneTesting : public Scene
{

public:
	void update(int deltaTime);

protected:
	TileMap* setTileMap();
	TileMap* setPlatformMap() { return nullptr; }
	TileMap* setStairsMap() { return nullptr; }
	void initItems();
	void initActors(Player *player);
	const pair<int, int> setNewLevelAndScene() const { return pair<int, int>(-1, -1); }

};


#endif // _SCENETESTING_INCLUDE

