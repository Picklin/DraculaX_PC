#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "GUI.h"
#include "SpawnPoint.h"
#include "EffectsManager.h"
#include "EnemyManager.h"
#include "ItemManager.h"
#include "ProjectileManager.h"
#include "Candle.h"
#include "Axe.h"

// Scene contains all the entities of our game.
// It is responsible for updating and render them.

#define MAP_OFFSET glm::ivec2(32, 16)
#define SCREEN_X 32
#define SCREEN_Y 16
#define CAMERA_X 48
#define CAMERA_Y 16

class Scene
{

public:
	virtual ~Scene();
	virtual void init(Player& player, GUI& gui, ShaderProgram& spriteShader, ShaderProgram& basicShader);
	virtual void update(int deltaTime);
	virtual void render();
	void setScreenWidth(int width);
	void setViewportOffset(int offset);

protected:
	virtual TileMap* setTileMap() = 0;
	virtual TileMap* setPlatformMap() = 0;
	virtual TileMap* setStairsMap() = 0;
	virtual void initItems() = 0;
	virtual void initActors(Player* player) = 0;
	void updateActors(int deltaTime);
	virtual void updateCamera();
	virtual const pair<int, int> setNewLevelAndScene() const = 0;
	virtual void doAction(int eventId) {};
	void renderTransition();
	bool collision(const Hitbox& hitbox1, const Hitbox& hitbox2);

private:
	bool isInArea(const Hitbox& area, const glm::vec2& pos);
	bool oobProjectile(const glm::vec2& pos) const;
	bool oobEnemy(const glm::vec2& pos) const;
	void updateSpawnPoints(int deltaTime);
	void updateTriggerAreas();
	void updateEffects(int deltaTime);
	void destroyCandle(Candle& candle);
	void initManagers();

protected:
	int screenWidth;
	int viewportOffset;
	int maxSubweaponInstances = 1;

protected:
	enum Actions
	{
		AUTO_ADVANCE, NEXT_SCENE, BOSS_APPEARS
	};
	glm::mat4 projection;
	vector<Item*> items;
	vector<Candle*> candles;
	vector<Subweapon*> subweapons;
	vector<Projectile*> projectiles;
	vector<Enemy*> enemies;
	vector<InfSpawnPoint> infSpawnPoints;
	vector<SpawnPoint*> spawnPoints;
	vector<TriggerArea*> triggerAreas;
	vector<Effect*> effects;
	vector<Texture> backgroundTexs;
	vector<Sprite*> backgroundSprites;
	//capas de background estático de más al fondo a menos (de 0 a n-1)
	vector<TileMap*> layers;

	//para el parallax
	vector<glm::mat4> projections;

	glm::vec2 cameraPos;
	TileMap* map;
	TileMap* platforms;
	TileMap* stairs;
	Player* player;
	GUI* gui;
	ShaderProgram* spriteShader;
	ShaderProgram* basicShader;				//seconds
	TexturedQuad* blackScreen;

	float minX;
	float minY;
	float maxX;
	float maxY;

	float timeElapsed;
	float blackScreenDuration;
	float fadeDuration;
	float fadeTime = 0.f;
	bool fadeIn = false;
	bool fadeOut = true;
};


#endif // _SCENE_INCLUDE

