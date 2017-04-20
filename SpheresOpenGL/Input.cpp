#include "Input.h"

#include "OpenGL.h"

bool keys[1024];

void InitInput()
{
	glfwSetKeyCallback(GLWindow, KeyCallback);
}

void KeyCallback(GLFWwindow * window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(window, GL_TRUE);

	if (action == GLFW_PRESS) keys[key] = true;
	else if (action == GLFW_RELEASE) keys[key] = false;
}

void UpdateInput()
{
	glfwPollEvents();
}

