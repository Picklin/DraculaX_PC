#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Game.h"
#include "SoundEngine.h"

enum Levels
{
	STAGE1,
};

void Game::init()
{
	bPlay = true;
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	initShaders();
	player.init(MAP_OFFSET, texProgram);
	EnemyManager::instance().setPlayer(player.getPointerPos(), player.myCenter());
	//titScreen.init(texProgram);
	st.init(player, texProgram);
}

bool Game::update(int deltaTime)
{
	updateGameInputs();

	if (gameStarted)
	{
		if (next)
		{
			delete scene;
			scene = scenesFactory[nextLevel][nextScene]();
			scene->init(player, texProgram);
			currentLevel = nextLevel;
			currentScene = nextScene;
			next = false;
		}
		else if (restart)
		{
			delete scene;
			scene = scenesFactory[currentLevel][currentScene]();
			scene->init(player, texProgram);
			restart = false;
		}

		bool startPressed = getKey(GLFW_KEY_ENTER);
		if (startPressed && !startPressedLastFrame)
		{
			paused = !paused;
			if (paused)
			{
				SoundEngine::instance().pauseAllSounds();
				//SoundEngine::instance().playPause();
			}
			else
			{
				SoundEngine::instance().unpauseAllSounds();
			}
		}
		startPressedLastFrame = startPressed;
		if (!paused)
		{
			//scene->update(deltaTime);
			SoundEngine::instance().update();
			st.update(deltaTime);
		}
	}
	else
	{
		if (scene != NULL)
		{
			delete scene;
			scene = nullptr;
		}
		//titScreen.update(deltaTime);
	}

	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (gameStarted)
	{
		//scene->render();
		st.render();
	}
	//else titScreen.render();
}

void Game::changeScene(int level, int scene)
{
	this->nextLevel = level;
	this->nextScene = scene;
	next = true;
}

void Game::restartScene()
{
	SoundEngine::instance().stopAllSounds();
	//player.respawn();
	//if (player.getLifes() == 0) gameOver();
	//else restart = true;
}

void Game::win()
{
	//titScreen.win();
	gameStarted = false;
	reset();
}

void Game::start()
{
	gameStarted = true;
	scene = scenesFactory[currentLevel][currentScene]();
	scene->init(player, texProgram);
}

void Game::keyPressed(int key)
{
	if (key == -1) return;
	if (key == GLFW_KEY_ESCAPE) // Escape code
		bPlay = false;
	keys[key] = true;
}

void Game::keyReleased(int key)
{
	if (key == -1) return;
	keys[key] = false;
}

void Game::mouseMove(int x, int y)
{
}

void Game::mousePress(int button)
{
}

void Game::mouseRelease(int button)
{
}

void Game::setScreenWidth(int width)
{
	st.setScreenWidth(width);
	//scene->setScreenWidth(width);
}

void Game::setViewportOffset(int offset)
{
	st.setViewportOffset(offset);
	//scene->setviewportOffset(offset);
}

void Game::updateGameInputs()
{
	if (!glfwJoystickIsGamepad(GLFW_JOYSTICK_1)) return;
	GLFWgamepadstate gamepadState;
	if (glfwGetGamepadState(GLFW_JOYSTICK_1, &gamepadState))
	{
		const float axisThreshold = .5f;
		float axisX = gamepadState.axes[GLFW_GAMEPAD_AXIS_LEFT_X];
		float axisY = gamepadState.axes[GLFW_GAMEPAD_AXIS_LEFT_Y];
		keys[GLFW_KEY_UP] = axisY < -axisThreshold || gamepadState.buttons[GLFW_GAMEPAD_BUTTON_DPAD_UP];
		keys[GLFW_KEY_DOWN] = axisY > axisThreshold || gamepadState.buttons[GLFW_GAMEPAD_BUTTON_DPAD_DOWN];
		keys[GLFW_KEY_RIGHT] = axisX > axisThreshold || gamepadState.buttons[GLFW_GAMEPAD_BUTTON_DPAD_RIGHT];
		keys[GLFW_KEY_LEFT] = axisX < -axisThreshold || gamepadState.buttons[GLFW_GAMEPAD_BUTTON_DPAD_LEFT];
		keys[GLFW_KEY_A] = gamepadState.buttons[GLFW_GAMEPAD_BUTTON_LEFT_BUMPER] == GLFW_PRESS;
		keys[GLFW_KEY_S] = gamepadState.buttons[GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER] == GLFW_PRESS;
		keys[GLFW_KEY_Z] = gamepadState.buttons[GLFW_GAMEPAD_BUTTON_A] == GLFW_PRESS;
		keys[GLFW_KEY_X] = gamepadState.buttons[GLFW_GAMEPAD_BUTTON_X] == GLFW_PRESS;
		keys[GLFW_KEY_C] = gamepadState.buttons[GLFW_GAMEPAD_BUTTON_Y] == GLFW_PRESS;
		keys[GLFW_KEY_SPACE] = gamepadState.buttons[GLFW_GAMEPAD_BUTTON_B] == GLFW_PRESS;
		keys[GLFW_KEY_TAB] = gamepadState.buttons[GLFW_GAMEPAD_BUTTON_BACK] == GLFW_PRESS;
		keys[GLFW_KEY_ENTER] = gamepadState.buttons[GLFW_GAMEPAD_BUTTON_START] == GLFW_PRESS;
	}
}

void Game::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if (!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
}

void Game::reset()
{
	currentLevel = 0;
	currentScene = 0;
	player.init(MAP_OFFSET, texProgram);
}

void Game::gameOver()
{
	//titScreen.gameOver();
	gameStarted = false;
	reset();
}

bool Game::getKey(int key) const
{
	return keys[key];
}

Game::Game()
{
	scene = nullptr;
}


