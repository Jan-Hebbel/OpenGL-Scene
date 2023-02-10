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
Shader skyboxShader;
Matrices matrices;
unsigned int blockVA;
unsigned int skyboxVA;
unsigned int textureBlock;
unsigned int textureSkybox;

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
	camera.right = glm::normalize(glm::cross(camera.front, camera.worldUp));
	camera.up = glm::normalize(glm::cross(camera.right, camera.front));
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
	// basic shader
	shader.Create();
	shader.AddShader("res/shader/Basic.vert", GL_VERTEX_SHADER);
	shader.AddShader("res/shader/Basic.frag", GL_FRAGMENT_SHADER);
	shader.LinkShader();

	float block[] = {
		// bottom
	    -0.5f, -0.5f, -0.5f,		0.125f,  0.0f,		 0.0f, -1.0f,  0.0f,	0.9375f, 0.0f,
	 	 0.5f, -0.5f, -0.5f,		0.1875f, 0.0f,		 0.0f, -1.0f,  0.0f,	1.0f,    0.0f,
		 0.5f, -0.5f,  0.5f,		0.1875f, 0.0625f,	 0.0f, -1.0f,  0.0f,    1.0f,    0.0625f,
	    -0.5f, -0.5f,  0.5f,		0.125f,  0.0625f,	 0.0f, -1.0f,  0.0f,    0.9375f, 0.0625f,
		// top
	    -0.5f,  0.5f,  0.5f,		0.0f,    0.0f,	 	 0.0f,  1.0f,  0.0f,	0.9375f, 0.0f,
		 0.5f,  0.5f,  0.5f,		0.0625f, 0.0f,		 0.0f,  1.0f,  0.0f,	1.0f,    0.0f,
		 0.5f,  0.5f, -0.5f,		0.0625f, 0.0625f,	 0.0f,  1.0f,  0.0f,	1.0f,    0.0625f,
		-0.5f,  0.5f, -0.5f,		0.0f,	 0.0625f,	 0.0f,  1.0f,  0.0f,	0.9375f, 0.0625f,
		// front
	    -0.5f, -0.5f,  0.5f,		0.0625f, 0.0f,		 0.0f,  0.0f,  1.0f,	0.9375f, 0.0f,
		 0.5f, -0.5f,  0.5f,		0.125f,  0.0f,		 0.0f,  0.0f,  1.0f,	1.0f,    0.0f,
		 0.5f,  0.5f,  0.5f,		0.125f,  0.0625f,	 0.0f,  0.0f,  1.0f,	1.0f,    0.0625f,
		-0.5f,  0.5f,  0.5f,		0.0625f, 0.0625f,	 0.0f,  0.0f,  1.0f,	0.9375f, 0.0625f,
		// left
	    -0.5f, -0.5f, -0.5f,		0.0625f, 0.0f,		-1.0f,  0.0f,  0.0f,	0.9375f, 0.0f,
		-0.5f, -0.5f,  0.5f,		0.125f,  0.0f,		-1.0f,  0.0f,  0.0f,	1.0f,    0.0f,
		-0.5f,  0.5f,  0.5f,		0.125f,  0.0625f,	-1.0f,  0.0f,  0.0f,	1.0f,    0.0625f,
		-0.5f,  0.5f, -0.5f,		0.0625f, 0.0625f,	-1.0f,  0.0f,  0.0f,	0.9375f, 0.0625f,
		// right
		 0.5f, -0.5f,  0.5f,		0.0625f, 0.0f,		 1.0f,  0.0f,  0.0f,	0.9375f, 0.0f,
		 0.5f, -0.5f, -0.5f,		0.125f,  0.0f,		 1.0f,  0.0f,  0.0f,	1.0f,    0.0f,
		 0.5f,  0.5f, -0.5f,		0.125f,  0.0625f,	 1.0f,  0.0f,  0.0f,	1.0f,    0.0625f,
		 0.5f,  0.5f,  0.5f,		0.0625f, 0.0625f,	 1.0f,  0.0f,  0.0f,	0.9375f, 0.0625f,
		 // back
		 0.5f, -0.5f, -0.5f,		0.0625f, 0.0f,		 0.0f,  0.0f, -1.0f,	0.9375f, 0.0f,
		-0.5f, -0.5f, -0.5f,		0.125f,  0.0f,		 0.0f,  0.0f, -1.0f,	1.0f,    0.0f,
		-0.5f,  0.5f, -0.5f,		0.125f,  0.0625f,	 0.0f,  0.0f, -1.0f,	1.0f,    0.0625f,
		 0.5f,  0.5f, -0.5f,		0.0625f, 0.0625f,	 0.0f,  0.0f, -1.0f,	0.9375f, 0.0625f,
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

	unsigned int vb, ib;
	glGenVertexArrays(1, &blockVA);
	glBindVertexArray(blockVA);

	glGenBuffers(1, &vb);
	glBindBuffer(GL_ARRAY_BUFFER, vb);
	glBufferData(GL_ARRAY_BUFFER, sizeof(block), block, GL_STATIC_DRAW);

	glGenBuffers(1, &ib);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(blockIndices), blockIndices,
		GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(float) * 10, (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeof(float) * 10, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, false, sizeof(float) * 10, (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 2, GL_FLOAT, false, sizeof(float) * 10, (void*)(8 * sizeof(float)));

	glGenTextures(1, &textureBlock);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureBlock);

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
	stbi_set_flip_vertically_on_load(false);

	// skybox shader
	skyboxShader.Create();
	skyboxShader.AddShader("res/shader/Skybox.vert", GL_VERTEX_SHADER);
	skyboxShader.AddShader("res/shader/Skybox.frag", GL_FRAGMENT_SHADER);
	skyboxShader.LinkShader();

	// cube vbo, vao and data for skybox
	float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	unsigned int vb_skybox;
	glGenVertexArrays(1, &skyboxVA);
	glBindVertexArray(skyboxVA);

	glGenBuffers(1, &vb_skybox);
	glBindBuffer(GL_ARRAY_BUFFER, vb_skybox);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(float) * 3, (const void*)0);

	// load cube map for skybox
	glGenTextures(1, &textureSkybox);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureSkybox);

	const std::string faces[] = {
		"res/textures/skybox_right.png",
		"res/textures/skybox_left.png",
		"res/textures/skybox_top.png",
		"res/textures/skybox_bottom.png",
		"res/textures/skybox_front.png",
		"res/textures/skybox_back.png"
	};

	for (unsigned int i = 0; i < 6; ++i)
	{
		unsigned char* data = 
			stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);

		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGBA,
				GL_UNSIGNED_BYTE, data);
		}
		else
		{
			std::cout << "Failed to load skybox texture " << i << '\n';
		}
		stbi_image_free(data);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	camera.position = glm::vec3(0.0f, 0.0f, 3.0f);
	camera.front = glm::vec3(0.0f, 0.0f, -1.0f);
	camera.up = glm::vec3(0.0f, 1.0f, 0.0f);
	camera.right = glm::vec3(1.0f, 0.0f, 0.0f);
	camera.worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	camera.movementSpeed = 5.0f;
	camera.mouseSens = 0.1f;

	// leave object in the middle
	matrices.model = glm::mat4(1.0f);
	matrices.view = glm::lookAt(camera.position, camera.position + camera.front, camera.up);
	matrices.projection = 
		glm::perspective(glm::pi<float>() * 0.25f, window.aspectRatio, 0.1f, 100.0f);

	shader.Bind();
	// vertex shader
	shader.SetMat4("projection", matrices.projection);
	shader.SetMat4("view", matrices.view);
	shader.SetMat4("model", matrices.model);

	// fragment shader
	shader.SetVec3("viewPos", camera.position);
	// material
	shader.SetInt("material.textureAtlas", 0);
	shader.SetFloat("material.shininess", 1);
	// light
	shader.SetVec3("light.position", glm::vec3(8.0f, 2.0f, 8.0f));
	shader.SetVec3("light.color", glm::vec3(1.0f, 0.8f, 0.8f));
	shader.SetVec3("light.ambient", glm::vec3(0.1f, 0.1f, 0.1f));
	shader.SetVec3("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
	shader.SetVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));

	skyboxShader.Bind();
	skyboxShader.SetMat4("projection", matrices.projection);
	skyboxShader.SetMat4("view", glm::mat4(glm::mat3(matrices.view)));
	skyboxShader.SetInt("skybox", 0);
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

	// draw 16 x 16 grass blocks
	shader.Bind();
	shader.SetMat4("projection", matrices.projection);
	shader.SetMat4("view", matrices.view);
	shader.SetMat4("model", matrices.model);
	shader.SetVec3("viewPos", camera.position);
	glBindVertexArray(blockVA);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureBlock);
	for (float i = 0.0f; i < 16.0f; ++i)
	{
		for (float j = 0.0f; j < 16.0f; ++j)
		{
			/*if (i > 0 || j > 0) continue;*/
			matrices.model = glm::mat4(1.0f);
			matrices.model = glm::translate(matrices.model, glm::vec3(i, 0.0f, j));
			shader.SetMat4("model", matrices.model);
			//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
			//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}
	matrices.model = glm::mat4(1.0f);
	matrices.model = glm::translate(matrices.model, glm::vec3(4.0f, 1.0f, 4.0f));
	shader.SetMat4("model", matrices.model);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// draw skybox
	glDepthFunc(GL_LEQUAL);
	skyboxShader.Bind();
	skyboxShader.SetMat4("projection", matrices.projection);
	skyboxShader.SetMat4("view", glm::mat4(glm::mat3(matrices.view)));
	glBindVertexArray(skyboxVA);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureSkybox);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS);
}

void Destroy()
{
	glfwDestroyWindow(window.handle);
	glfwTerminate();
}

void Window::Create(int width, int height)
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

	window.width = width;
	window.height = height;
	window.aspectRatio = static_cast<float>(width) / static_cast<float>(height);

	window.handle = glfwCreateWindow(window.width, window.height, "Projekt", nullptr, nullptr);
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

	glViewport(0, 0, window.width, window.height);

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
	glEnable(GL_CULL_FACE);
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
