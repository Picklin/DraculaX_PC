#ifndef _GAME_INCLUDE
#define _GAME_INCLUDE

#include <GLFW/glfw3.h>
#include "GUI.h"
#include "SceneTesting.h"
#include "Screen.h"
#include "Cinematic.h"

#define SCREEN_WIDTH 256
#define SCREEN_HEIGHT 224


// Game is a singleton (a class with a single instance) that represents our whole application


class Game
{

private:
	Game();
	//~Game();

public:
	static Game& instance()
	{
		static Game G;

		return G;
	}

	enum Levels
	{
		STAGE0, STAGE1, STAGE2, STAGE2_ALT, STAGE2_ALT2, STAGE3, STAGE3_ALT, STAGE4, STAGE4_ALT, STAGE5, STAGE5_ALT, STAGE6, STAGE7, STAGE8
	};

	void init();
	bool update(int deltaTime);
	void render();
	void changeScene(int level, int scene);
	void restartScene();
	void win();
	void start();

	// Input callback methods
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMove(int x, int y);
	void mousePress(int button);
	void mouseRelease(int button);
	void setScreenWidth(int width);
	void setViewportOffset(int offset);
	void setInitialConfig();

	int getCurrentDubLang();
	int getCurrentTxtLang();
	int getCurrentLevel();

	bool isTwoPlayerMode();
	bool isArranged();
	bool getKey(int key) const;

private:
	void updateGameInputs();
	void initShaders();
	void reset();
	void gameOver();

private:
	Player player;
	GUI gui;
	ShaderProgram spriteShader, basicShader, guiShader;
	using SceneCreator = std::function<Scene* ()>;
	vector<vector<SceneCreator>> scenesFactory;
	Scene* scene;
	Screen* currentMenu;
	Cinematic* cinematic;

	int currDubLang;
	int currTxtLang;
	int currentLevel;
	int currentScene;
	int nextLevel;
	int nextScene;

	struct Key
	{
		bool pressed = false;
		bool released = true;
	};

	Key keys[GLFW_KEY_LAST + 1]; // Store key states so that we can have access at any time
	bool bPlay; // Continue to play game?
	bool gameStarted = false;
	bool paused = false;
	bool startPressedLastFrame = false;
	bool next = false;
	bool restart = false;
	bool playingCinematic;
	bool twoPlayerMode;
	bool arranged;
	bool initialConfigSet = false;
};


#endif // _GAME_INCLUDE