#include <glad/glad.h>

#include <iostream>

#include "Debug.h"
#include "Window.h"

int main() 
{
	Window window;
	window.SetWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	window.SetWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	window.SetWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//window.EnableVSync(); // "turn on vsync"
	window.CreateWindow(600, 600, "Projekt", false);

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	// debugging --------------------------------------------
	int flags;
	glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
	{
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(openglErrorCallback, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, 
			nullptr, GL_TRUE);
	}
	// ------------------------------------------------------

	// Game loop
	while (!glfwWindowShouldClose(window.m_Window))
	{
		glfwPollEvents();

		glClearColor(0.2f, 0.3f, 0.6f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window.m_Window);
	}
}
