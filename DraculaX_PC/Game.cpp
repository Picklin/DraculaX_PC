#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Game.h"
#include "Level1Sc1.h"
#include "Level1Sc2.h"
#include "Level1Sc3.h"

enum VoiceLanguages
{
	JP_DUB, EN_DUB
};

enum TextLanguages
{
	EN_TXT, ES_TXT
};

void Game::init()
{
	/*
	Al iniciar el juego, habrá que preguntarle al jugador:
	- idioma doblaje (original o inglés)
	- idioma texto (inglés o español)
	- idioma subtítulos (inglés o español), si los quiere.
	- musica (original o arranged)
	Esto se podrá cambiar en la configuración del juego más adelante.
	*/
	bPlay = true;
	gameStarted = false;
	twoPlayerMode = false;
	arranged = false;
	initialConfigSet = true;
	playingCinematic = !gameStarted && initialConfigSet;
	currDubLang = JP_DUB;
	currTxtLang = ES_TXT;
	currentLevel = STAGE1;
	currentScene = 0;
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	initShaders();
	player.init(MAP_OFFSET, spriteShader);
	gui.init(guiShader, &player, false && twoPlayerMode);
	EnemyManager::instance().setPlayer(player.getPointerPos(), player.myCenter());
	spriteShader.use();
	cinematic = Cinematic::createCinematic(spriteShader, arranged ? "Dialogues/Scripts/[2007_ES] Intro.txt" : "Dialogues/Scripts/[1993_ES] Intro.txt", Cinematic::INTRO);
	currentMenu = Screen::createScreen(spriteShader, Screen::TITLE);
	SoundEngine::instance().setMusicMode(arranged);	//cargamos sfx y paths para la música y establecemos si es arranged
	SoundEngine::instance().loadCinematics();
	if (playingCinematic) SoundEngine::instance().playIntro();
	//start();		//comentar cuando se deje de testear
	//st.init(player, gui, spriteShader, basicShader);
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
			scene->init(player, gui, spriteShader, basicShader);
			currentLevel = nextLevel;
			currentScene = nextScene;
			next = false;
		}
		else if (restart)
		{
			delete scene;
			scene = scenesFactory[currentLevel][currentScene]();
			scene->init(player, gui, spriteShader, basicShader);
			restart = false;
		}

		bool startPressed = getKey(GLFW_KEY_ENTER);
		if (startPressed && !startPressedLastFrame)
		{
			paused = !paused;
			if (paused)
			{
				SoundEngine::instance().pauseAllSounds();
				SoundEngine::instance().playSFX(SoundEngine::PAUSE);
			}
			else
			{
				SoundEngine::instance().unpauseAllSounds();
			}
			gui.toggleBoardFrame();
		}
		startPressedLastFrame = startPressed;
		if (!paused)
		{
			scene->update(deltaTime);
			SoundEngine::instance().update();
			//st.update(deltaTime);
		}
		gui.update(deltaTime);
	}
	else if (playingCinematic)
	{
		cinematic->update(deltaTime);
		playingCinematic = !cinematic->ended();
		if (!playingCinematic) SoundEngine::instance().stopAllSounds();
	}
	else
	{
		currentMenu->update(deltaTime);
	}

	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (gameStarted)
	{
		scene->render();
		//st.render();
	}
	else if (playingCinematic)
	{
		cinematic->render();
	}
	else currentMenu->render();
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
	//currentMenu.win();
	gameStarted = false;
	reset();
}

void Game::start()
{
	gameStarted = true;
	scene = scenesFactory[currentLevel][currentScene]();
	scene->init(player, gui, spriteShader, basicShader);
	gui.reset();
}

void Game::keyPressed(int key)
{
	if (key == -1) return;
	if (key == GLFW_KEY_ESCAPE) // Escape code
		bPlay = false;
	keys[key].pressed = true;
}

void Game::keyReleased(int key)
{
	if (key == -1) return;
	keys[key].pressed = false;
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
	//scene->setScreenWidth(width);
}

void Game::setViewportOffset(int offset)
{
	//scene->setviewportOffset(offset);
}

int Game::getCurrentDubLang()
{
	return currDubLang;
}

int Game::getCurrentTxtLang()
{
	return currTxtLang;
}

int Game::getCurrentLevel()
{
	return currentLevel;
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
		keys[GLFW_KEY_UP].pressed = axisY < -axisThreshold || gamepadState.buttons[GLFW_GAMEPAD_BUTTON_DPAD_UP];
		keys[GLFW_KEY_DOWN].pressed = axisY > axisThreshold || gamepadState.buttons[GLFW_GAMEPAD_BUTTON_DPAD_DOWN];
		keys[GLFW_KEY_RIGHT].pressed = axisX > axisThreshold || gamepadState.buttons[GLFW_GAMEPAD_BUTTON_DPAD_RIGHT];
		keys[GLFW_KEY_LEFT].pressed = axisX < -axisThreshold || gamepadState.buttons[GLFW_GAMEPAD_BUTTON_DPAD_LEFT];
		keys[GLFW_KEY_A].pressed = gamepadState.buttons[GLFW_GAMEPAD_BUTTON_LEFT_BUMPER] == GLFW_PRESS;
		keys[GLFW_KEY_S].pressed = gamepadState.buttons[GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER] == GLFW_PRESS;
		keys[GLFW_KEY_Z].pressed = gamepadState.buttons[GLFW_GAMEPAD_BUTTON_A] == GLFW_PRESS && keys[GLFW_KEY_Z].released;
		keys[GLFW_KEY_Z].released = gamepadState.buttons[GLFW_GAMEPAD_BUTTON_A] == GLFW_RELEASE;
		keys[GLFW_KEY_X].pressed = gamepadState.buttons[GLFW_GAMEPAD_BUTTON_X] == GLFW_PRESS && keys[GLFW_KEY_X].released;
		keys[GLFW_KEY_X].released = gamepadState.buttons[GLFW_GAMEPAD_BUTTON_X] == GLFW_RELEASE;
		keys[GLFW_KEY_C].pressed = gamepadState.buttons[GLFW_GAMEPAD_BUTTON_Y] == GLFW_PRESS;
		keys[GLFW_KEY_SPACE].pressed = gamepadState.buttons[GLFW_GAMEPAD_BUTTON_B] == GLFW_PRESS;
		keys[GLFW_KEY_TAB].pressed = gamepadState.buttons[GLFW_GAMEPAD_BUTTON_BACK] == GLFW_PRESS && keys[GLFW_KEY_TAB].released;
		keys[GLFW_KEY_TAB].released = gamepadState.buttons[GLFW_GAMEPAD_BUTTON_BACK] == GLFW_RELEASE;
		keys[GLFW_KEY_ENTER].pressed = gamepadState.buttons[GLFW_GAMEPAD_BUTTON_START] == GLFW_PRESS && keys[GLFW_KEY_ENTER].released;
		keys[GLFW_KEY_ENTER].released = gamepadState.buttons[GLFW_GAMEPAD_BUTTON_START] == GLFW_RELEASE;
	}
}

void Game::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture_sprites.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture_sprites.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	spriteShader.init();
	spriteShader.addShader(vShader);
	spriteShader.addShader(fShader);
	spriteShader.link();
	if (!spriteShader.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << spriteShader.log() << endl << endl;
	}
	spriteShader.bindFragmentOutput("outColor");

	vShader.free();
	fShader.free();
	vShader.initFromFile(VERTEX_SHADER, "shaders/texture_basic.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture_backgrounds.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	basicShader.init();
	basicShader.addShader(vShader);
	basicShader.addShader(fShader);
	basicShader.link();
	if (!basicShader.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << basicShader.log() << endl << endl;
	}
	basicShader.bindFragmentOutput("outColor");
	fShader.free();
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/gui.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	guiShader.init();
	guiShader.addShader(vShader);
	guiShader.addShader(fShader);
	guiShader.link();
	if (!guiShader.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << guiShader.log() << endl << endl;
	}
	guiShader.bindFragmentOutput("outColor");
	fShader.free();
	vShader.free();
}

void Game::reset()
{
	currentLevel = 0;
	currentScene = 0;
	player.init(MAP_OFFSET, basicShader);
}

void Game::gameOver()
{
	//currentMenu.gameOver();
	gameStarted = false;
	reset();
}

bool Game::isTwoPlayerMode()
{
	return twoPlayerMode;
}

bool Game::isArranged()
{
	return arranged;
}

bool Game::getKey(int key) const
{
	return keys[key].pressed;
}

Game::Game()
{
	scenesFactory.resize(2);
	vector<SceneCreator> lvl1SC;
	lvl1SC.reserve(3);
	lvl1SC.emplace_back([this]() { return new Level1Sc1(); });
	lvl1SC.emplace_back([this]() { return new Level1Sc2(); });
	lvl1SC.emplace_back([this]() { return new Level1Sc3(); });
	scenesFactory[1] = lvl1SC;
	scene = nullptr;
	cinematic = nullptr;
}



