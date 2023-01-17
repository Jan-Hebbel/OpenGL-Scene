#pragma once

#include <glad/glad.h>

#define ASSERT(x) if (!(x)) __debugbreak();

void glfwErrorCallback(int errorCode, const char* description);

void GLAPIENTRY openglErrorCallback(
    unsigned int source,
    unsigned int type,
    unsigned int id,
    unsigned int severity,
    int length,
    const char* message,
    const void* userParam
);