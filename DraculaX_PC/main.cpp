#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Game.h"
#include <iostream>


#define TARGET_FRAMERATE 60.0f

bool isFullscreen = false;
double lastClickTime = 0.0;

int windowedPosX, windowedPosY;
int windowedWidth, windowedHeight;

void joystick_callback(int joystickId, int action)
{
	if (action == GLFW_CONNECTED)
	{
		std::cout << "Mando conectado" << endl;
	}
	else if (action == GLFW_DISCONNECTED)
	{
		std::cout << "Mando desconectado" << endl;
	}
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
		Game::instance().keyPressed(key);
	else if (action == GLFW_RELEASE)
		Game::instance().keyReleased(key);
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	Game::instance().mouseMove(int(xpos), int(ypos));
}
void toggleFullscreen(GLFWwindow* window)
{
	if (!isFullscreen)
	{
		glfwGetWindowPos(window, &windowedPosX, &windowedPosY);
		glfwGetWindowSize(window, &windowedWidth, &windowedHeight);

		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);

		glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
	}
	else
	{
		glfwSetWindowMonitor(window, nullptr, windowedPosX, windowedPosY, windowedWidth, windowedHeight, 0);
	}

	// Cambiamos el estado
	isFullscreen = !isFullscreen;
}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_PRESS)
		Game::instance().mousePress(button);
	else if (action == GLFW_RELEASE)
		Game::instance().mouseRelease(button);
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		double currentTime = glfwGetTime();

		if (currentTime - lastClickTime < 0.3)
		{
			toggleFullscreen(window);
		}

		lastClickTime = currentTime;
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// Actualizar la proyección para mantener la relación de aspecto
	float targetAspectRatio = float(SCREEN_WIDTH) / float(SCREEN_HEIGHT);

	float windowAspectRatio = float(width) / float(height);
	int viewportX, viewportY, viewportWidth, viewportHeight;

	if (windowAspectRatio > targetAspectRatio)
	{
		viewportHeight = height;
		viewportWidth = (int)(height * targetAspectRatio);
		viewportX = (width - viewportWidth) / 2;
		viewportY = 0;
	}
	else
	{
		viewportWidth = width;
		viewportHeight = (int)(width / targetAspectRatio);
		viewportX = 0;
		viewportY = (height - viewportHeight) / 2;
	}
	Game::instance().setScreenWidth(viewportWidth);
	Game::instance().setViewportOffset(viewportX);
	glViewport(viewportX, viewportY, viewportWidth, viewportHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
}

int main(void)
{
	GLFWwindow* window;
	double timePerFrame = 1.f / TARGET_FRAMERATE, timePreviousFrame, currentTime;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Set window initial position */
	glfwSetWindowPos(window, 100, 100);
	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	/* Set callbacks */
	glfwSetJoystickCallback(joystick_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	/* Init glew to have access to GL extensions */
	glewExperimental = GL_TRUE;
	glewInit();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	/* Init step of the game loop */
	Game::instance().init();
	timePreviousFrame = glfwGetTime();

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		currentTime = glfwGetTime();
		if (currentTime - timePreviousFrame >= timePerFrame)
		{
			/* Update & render steps of the game loop */
			if (!Game::instance().update(int(1000.0f * (currentTime - timePreviousFrame))))
				glfwSetWindowShouldClose(window, GLFW_TRUE);
			Game::instance().render();
			timePreviousFrame = currentTime;

			/* Swap front and back buffers */
			glfwSwapBuffers(window);
		}

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

