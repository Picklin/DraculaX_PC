#ifndef _GAME_INCLUDE
#define _GAME_INCLUDE

#include <GLFW/glfw3.h>
#include "GUI.h"
#include "SceneTesting.h"
#include "Level1Sc1.h"


#define SCREEN_WIDTH 256
#define SCREEN_HEIGHT 224


// Game is a singleton (a class with a single instance) that represents our whole application


class Game
{

private:
	Game();

public:
	static Game& instance()
	{
		static Game G;

		return G;
	}

	enum Levels
	{
		STAGE1,
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

	bool getKey(int key) const;

private:
	void updateGameInputs();
	void initShaders();
	void reset();
	void gameOver();

private:
	Level1Sc1 st;
	Player player;
	GUI gui;
	ShaderProgram spriteShader, basicShader, guiShader;
	using SceneCreator = std::function<Scene* ()>;
	vector<vector<SceneCreator>> scenesFactory;
	Scene* scene;

	int currentLevel = 0;
	int currentScene = 0;
	int nextLevel;
	int nextScene;

	struct Key
	{
		bool pressed = false;
		bool released = true;
	};

	Key keys[GLFW_KEY_LAST + 1]; // Store key states so that we can have access at any time
	bool bPlay; // Continue to play game?
	bool gameStarted = true;
	bool paused = false;
	bool startPressedLastFrame = false;
	bool next = false;
	bool restart = false;
};


#endif // _GAME_INCLUDE