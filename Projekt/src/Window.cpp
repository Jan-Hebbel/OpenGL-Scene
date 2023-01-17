#include "Debug.h"
#include "Window.h"

#include <iostream>
#include <format>

Window::Window()
	: m_Window(nullptr)
{
	if (!glfwInit())
	{
		std::cout << "Failed to init GLFW" << '\n';
		exit(-1);
	}

#ifdef _DEBUG
	glfwSetErrorCallback(glfwErrorCallback);
#endif
}

Window::~Window()
{
	if (m_Window)
		glfwDestroyWindow(m_Window);
	glfwTerminate();
}

void Window::SetWindowHint(int hint, int value)
{
	glfwWindowHint(hint, value);
}

void Window::CreateWindow(int width, int height, const char* title, bool fullscreen)
{
	// no need to check for NULL, an ErrorCallback should be called
	m_Window = glfwCreateWindow(width, height, title, 
		(fullscreen ? glfwGetPrimaryMonitor() : nullptr), nullptr);
	
	glfwSetKeyCallback(m_Window, KeyCallback);

	glfwMakeContextCurrent(m_Window);
}

void Window::EnableVSync()
{
	glfwSwapInterval(-1);
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action,
	int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}
