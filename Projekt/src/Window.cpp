#include "Debug.h"

#include "Window.h"
#include "Shader.h"

#include <iostream>
#include <format>
#include <iomanip>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb/stb_image.h>

Window window;
Camera camera;
Shader shader;
Matrices matrices;

void keyCallback(GLFWwindow* pwindow, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(pwindow, true);
}

void mouseCallback(GLFWwindow* pwindow, double xpos, double ypos)
{
	float xposf = static_cast<float>(xpos);
	float yposf = static_cast<float>(ypos);

	if (window.firstMouse)
	{
		window.lastX = xposf;
		window.lastY = yposf;
		window.firstMouse = false;
	}

	float xOffset = xposf - window.lastX;
	float yOffset = window.lastY - yposf;
	window.lastX = xposf;
	window.lastY = yposf;

	xOffset *= camera.mouseSens;
	yOffset *= camera.mouseSens;

	camera.yaw += xOffset;
	camera.pitch += yOffset;

	if (camera.pitch > 89.0f)
		camera.pitch = 89.0f;
	if (camera.pitch < -89.0f)
		camera.pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
	front.y = sin(glm::radians(camera.pitch));
	front.z = sin(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
	camera.front = glm::normalize(front);

	//TODO: update up and right with the cross function; see learnopengl.com
}

void printFPSandFrameTime(double deltaTime)
{
	std::cout << "Frame time: ";
	std::cout << std::setprecision(3) << std::fixed;
	std::cout << deltaTime * 1000.0 << " ms";
	std::cout << " | " << "FPS: " << 1.0 / deltaTime << '\n';
}

void processInput()
{
	const float cameraSpeed = camera.movementSpeed * static_cast<float>(window.deltaTime);

	if (glfwGetKey(window.handle, GLFW_KEY_W) == GLFW_PRESS)
		camera.position += cameraSpeed * camera.front;
	if (glfwGetKey(window.handle, GLFW_KEY_S) == GLFW_PRESS)
		camera.position -= cameraSpeed * camera.front;
	if (glfwGetKey(window.handle, GLFW_KEY_A) == GLFW_PRESS)
		camera.position -= glm::normalize(glm::cross(camera.front, camera.up)) * cameraSpeed;
	if (glfwGetKey(window.handle, GLFW_KEY_D) == GLFW_PRESS)
		camera.position += glm::normalize(glm::cross(camera.front, camera.up)) * cameraSpeed;
}

void Init()
{
	//Shader shader;
	shader.Create();
	shader.AddShader("res/shader/Basic.vert", GL_VERTEX_SHADER);
	shader.AddShader("res/shader/Basic.frag", GL_FRAGMENT_SHADER);
	shader.LinkShader();

	float block[] = {
	   -0.5f, -0.5f,  0.5f,		0.0f,    0.0f,		// bottom
		0.5f, -0.5f,  0.5f,		0.0625f, 0.0f,
		0.5f, -0.5f, -0.5f,		0.0625f, 0.0625f,
	   -0.5f, -0.5f, -0.5f,		0.0f,    0.0625f,

	   -0.5f,  0.5f,  0.5f,		0.0f,    0.0f,		// top
		0.5f,  0.5f,  0.5f,		0.0625f, 0.0f,
		0.5f,  0.5f, -0.5f,		0.0625f, 0.0625f,
	   -0.5f,  0.5f, -0.5f,		0.0f,	 0.0625f,

	   -0.5f, -0.5f,  0.5f,		0.0f,    0.0f,		// front
		0.5f, -0.5f,  0.5f,		0.0625f, 0.0f,
		0.5f,  0.5f,  0.5f,		0.0625f, 0.0625f,
	   -0.5f,  0.5f,  0.5f,		0.0f,    0.0625f,

	   -0.5f, -0.5f, -0.5f,		0.0f,    0.0f,		// left
	   -0.5f, -0.5f,  0.5f,		0.0625f, 0.0f,
	   -0.5f,  0.5f,  0.5f,		0.0625f, 0.0625f,
	   -0.5f,  0.5f, -0.5f,		0.0f,    0.0625f,

		0.5f, -0.5f,  0.5f,		0.0f,    0.0f,		// right
		0.5f, -0.5f, -0.5f,		0.0625f, 0.0f,
		0.5f,  0.5f, -0.5f,		0.0625f, 0.0625f,
		0.5f,  0.5f,  0.5f,		0.0f,    0.0625f,

		0.5f, -0.5f, -0.5f,		0.0f,    0.0f,		// back
	   -0.5f, -0.5f, -0.5f,		0.0625f, 0.0f,
	   -0.5f,  0.5f, -0.5f,		0.0625f, 0.0625f,
		0.5f,  0.5f, -0.5f,		0.0f,    0.0625f
	};

	unsigned int blockIndices[] = {
		0,  1,  2,	// bottom 
		0,  2,  3,
		4,  5,  6,	// top 
		4,  6,  7,
		8,  9,  10,	// front
		8,  10, 11,
		12, 13, 14,	// left
		12, 14, 15,
		16, 17, 18,	// right
		16, 18, 19,
		20, 21, 22,	// back
		20, 22, 23,
	};

	unsigned int vb, ib, va;
	glGenVertexArrays(1, &va);
	glBindVertexArray(va);

	glGenBuffers(1, &vb);
	glBindBuffer(GL_ARRAY_BUFFER, vb);
	glBufferData(GL_ARRAY_BUFFER, sizeof(block), block, GL_STATIC_DRAW);

	glGenBuffers(1, &ib);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(blockIndices), blockIndices,
		GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(float) * 5, (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeof(float) * 5, (void*)(3 * sizeof(float)));

	unsigned int texture;
	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	stbi_set_flip_vertically_on_load(true);

	int width, height, nrChannels;
	unsigned char* data = stbi_load("res/textures/texture_atlas.png",
		&width, &height, &nrChannels, 0);

	if (data)
	{
		// set format to GL_RGBA if the image is a png
		// set format to GL_RGB if the image is a jpg
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}
	else
	{
		std::cout << "Failed to load texture" << '\n';
	}
	stbi_image_free(data);

	camera.position = glm::vec3(0.0f, 0.0f, 3.0f);
	camera.front = glm::vec3(0.0f, 0.0f, -1.0f);
	camera.up = glm::vec3(0.0f, 1.0f, 0.0f);
	camera.right = glm::vec3(1.0f, 0.0f, 0.0f);
	camera.worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	camera.movementSpeed = 2.5f;
	camera.mouseSens = 0.1f;

	// leave object in the middle
	matrices.model = glm::mat4(1.0f);
	matrices.view = glm::lookAt(camera.position, camera.position + camera.front, camera.up);
	matrices.projection = glm::perspective(glm::pi<float>() * 0.25f, 1.0f / 1.0f, 0.1f, 100.0f);

	matrices.mvp = matrices.projection * matrices.view * matrices.model;

	shader.Bind();
	// set sampler2D to GL_TEXTURE0, since current texture is on GL_TEXTURE0                                              
	shader.SetMat4(0, matrices.mvp);
	shader.SetInt(1, 0);
}

void Update()
{
	processInput();
}

void Render()
{
	glClearColor(0.2f, 0.3f, 0.6f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	matrices.view = glm::lookAt(camera.position, camera.position + camera.front, camera.up);
	matrices.mvp = matrices.projection * matrices.view * matrices.model;

	shader.Bind();
	shader.SetMat4(0, matrices.mvp);

	// last parameter can be 0 if the element array buffer is bound to 
	// the vertex array before the draw call
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void Destroy()
{
	glfwDestroyWindow(window.handle);
	glfwTerminate();
}

void Window::Create()
{
	// set up a window and create opengl context ------------
	if (!glfwInit())
	{
		std::cout << "Failed to init GLFW" << '\n';
		exit(-1);
	}

#ifdef _DEBUG
	glfwSetErrorCallback(glfwErrorCallback);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwSwapInterval(1);
	// in the monitor parameter use glfwGetPrimaryMonitor() for fullscreen

	window.handle = glfwCreateWindow(600, 600, "Projekt", nullptr, nullptr);
	if (!window.handle)
	{
		std::cout << std::format("Window creation failed") << '\n';
		glfwTerminate();
		exit(-1);
	}
	glfwMakeContextCurrent(window.handle);
	// ------------------------------------------------------

	// glfw callback functions ------------------------------
	glfwSetKeyCallback(window.handle, keyCallback);
	glfwSetCursorPosCallback(window.handle, mouseCallback);
	// ------------------------------------------------------

	glfwSetInputMode(window.handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// load opengl functions
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Error initializing glad" << '\n';
		glfwDestroyWindow(window.handle);
		glfwTerminate();
		exit(-1);
	}

	// debugging --------------------------------------------
	int flags;
	glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
	{
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(openglErrorCallback, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	}
	// ------------------------------------------------------

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Window::Loop()
{
	Init();

	while (!glfwWindowShouldClose(window.handle))
	{
		auto currentFrame = glfwGetTime();
		window.deltaTime = currentFrame - window.lastFrame;
		window.lastFrame = currentFrame;

		printFPSandFrameTime(window.deltaTime);

		Update();
		Render();

		glfwSwapBuffers(window.handle);
		glfwPollEvents();
	}

	Destroy();
}
