#include <glad/glad.h>

#include <iostream>

#include "Debug.h"
#include "Window.h"
#include "Shader.h"

#include <fstream>
#include <sstream>

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
	glGenBuffers(1, &vb); // delete vb later
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

	// Game loop
	while (!glfwWindowShouldClose(window.m_Window))
	{
		glfwPollEvents();

		glClearColor(0.2f, 0.3f, 0.6f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shader.Bind();
		glBindVertexArray(va);
		glBindBuffer(GL_ARRAY_BUFFER, vb);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, indices);

		glfwSwapBuffers(window.m_Window);
	}
}
