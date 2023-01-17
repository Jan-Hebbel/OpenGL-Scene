#pragma once

#include <GLFW/glfw3.h>

class Window
{
public:
	GLFWwindow* m_Window;

	Window();
	~Window();

	void SetWindowHint(int hint, int value);
	void CreateWindow(int width, int height, const char *title, 
		bool fullscreen);
	void EnableVSync();
};

void KeyCallback(GLFWwindow* window, int key, int scancode, int action,
	int mods);