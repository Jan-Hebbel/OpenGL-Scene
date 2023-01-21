#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <format>

#include "Debug.h"
#include "Shader.h"

#include <fstream>
#include <sstream>

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, 
	int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

int main() 
{
	// set up a window and create opengl context ------------
	if (!glfwInit())
	{
		std::cout << "Failed to init GLFW" << '\n';
		exit(-1);
	}

#ifdef _DEBUG
	glfwSetErrorCallback(glfwErrorCallback);
#endif

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwSwapInterval(1);
	// in the monitor parameter use glfwGetPrimaryMonitor() for fullscreen
	GLFWwindow* window = 
		glfwCreateWindow(600, 600, "Projekt", nullptr, nullptr);
	if (!window)
	{
		std::cout << std::format("Window creation failed") << '\n';
		exit(-1);
	}
	glfwMakeContextCurrent(window);
	// ------------------------------------------------------
	
	// glfw callback functions ------------------------------
	glfwSetKeyCallback(window, KeyCallback);
	// ------------------------------------------------------

	// load opengl functions
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

	Shader shader;
	shader.AddShader("res/shader/Basic.vert", GL_VERTEX_SHADER);
	shader.AddShader("res/shader/Basic.frag", GL_FRAGMENT_SHADER);
	shader.LinkShader();

	float quad[6 * 4] = {
		-0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f,
		0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f,
		-0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 1.0f
	};

	unsigned int indices[6] = {
		0, 1, 2,
		0, 2, 3
	};

	unsigned int vb;
	glGenBuffers(1, &vb);
	glBindBuffer(GL_ARRAY_BUFFER, vb);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);

	unsigned int ib;
	glGenBuffers(1, &ib);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
		GL_STATIC_DRAW);

	unsigned int va;
	glGenVertexArrays(1, &va);
	glBindVertexArray(va);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, false, sizeof(float) * 6, (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, false, sizeof(float) * 6, 
		(void*)(2 * sizeof(float)));

	glBindVertexArray(va);
	glBindBuffer(GL_ARRAY_BUFFER, vb);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.2f, 0.3f, 0.6f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shader.Bind();

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, indices);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
}
