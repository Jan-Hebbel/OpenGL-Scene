#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

struct Window
{
	GLFWwindow* handle;
	int width;
	int height;
	float aspectRatio;
	float lastX;
	float lastY;
	bool firstMouse = true;
	double deltaTime = 0.0f;
	double lastFrame = 0.0f;

	static void Create(int width, int height);
	static void Loop();
};

struct Camera
{
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	float yaw = -90.0f;
	float pitch = 0.0f;

	float movementSpeed;
	float mouseSens;
};

//TODO: Refactor
struct Matrices
{
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;
};
