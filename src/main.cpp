#include "gl_headers.h"

#include <stdlib.h>
#include <stdio.h>

#include "App.h"

const bool bFullscreen = false;

extern App* GetApp();

App* app;

static void error_callback(int error, const char* description)
{
	printf(description);
	fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scanCode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	app->KeyEvent(key, scanCode, action, mods);
}

void char_callback(GLFWwindow * window, unsigned int code)
{
	app->CharEvent(code);
}

void mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
	app->MouseClick(button, action, mods);
}

void cursor_callback(GLFWwindow* window, double x, double y)
{
	app->MouseDrag((int)x, (int)y);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	app->WindowResized(width, height);
	glViewport(0, 0, width, height);
}

int main(int argc, char** argv)
{
	app = GetApp();

	glfwSetErrorCallback(error_callback);

	if (!glfwInit())
		exit(EXIT_FAILURE);

	const GLFWvidmode* videoMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	if (!videoMode)
	{
		printf("Error: Primary Monitor VideoMode is NULL");
		exit(EXIT_FAILURE);
	}
	const int desktopWidth = videoMode->width;
	const int desktopHeight = videoMode->height;

	GLFWwindow* window = nullptr;
	if (bFullscreen)
	{
		window = glfwCreateWindow(desktopWidth, desktopHeight, app->GetTitle(), glfwGetPrimaryMonitor(), nullptr);
	}
	else
	{
		window = glfwCreateWindow(1000, 500, app->GetTitle(), nullptr, nullptr);
	}

	if (!window)
	{
		printf("Error: Failed to create window\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	int windowWidth, windowHeight;
	glfwGetWindowSize(window, &windowWidth, &windowHeight);

	glfwSetWindowPos(window, desktopWidth / 2 - windowWidth / 2 + 1000, desktopHeight / 2 - windowHeight / 2);
	
	int displayWidth, displayHeight;
	glfwGetFramebufferSize(window, &displayWidth, &displayHeight);
	
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	//Perform initial call to set app width and height
	framebuffer_size_callback(window, displayWidth, displayHeight);

	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, key_callback);
	glfwSetCharCallback(window, char_callback);
	glfwSetMouseButtonCallback(window, mouse_callback);
	glfwSetCursorPosCallback(window, cursor_callback);

	app->Initialize();

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		app->Update();
		glfwPollEvents();
		app->Render();
		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);

	glfwTerminate();

	app->Shutdown();
	delete app;

	exit(EXIT_SUCCESS);
}