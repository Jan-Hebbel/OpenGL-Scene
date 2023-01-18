#include "Debug.h"

#include <iostream>
#include <format>

void glfwErrorCallback(int errorCode, const char* description)
{
	std::cout << std::format("[GLFW] {}: {}\n", errorCode, description);
}

void GLAPIENTRY openglErrorCallback(
    unsigned int source,
    unsigned int type,
    unsigned int id,
    unsigned int severity,
    int length,
    const char* message,
    const void* userParam
)
{
    switch (severity)
    {
        std::cout << "[OpenGL] ";
        case GL_DEBUG_SEVERITY_HIGH:
        {
            fprintf(stderr, 
                "ERROR: type = 0x%x\n\tseverity: HIGH\n\tmessage = %s\n", 
                type, message);
            exit(-1);
            break;
        }
        case GL_DEBUG_SEVERITY_MEDIUM:
        {
            fprintf(stderr, 
                "WARNING: type = 0x%x\n\tseverity: MEDIUM\n\tmessage = %s\n", 
                type, message);
            break;
        }
        case GL_DEBUG_SEVERITY_LOW:
        {
            fprintf(stderr, 
                "WARNING: type = 0x%x\n\tseverity: LOW\n\tmessage = %s\n", 
                type, message);
            break;
        }
        case GL_DEBUG_SEVERITY_NOTIFICATION:
        {
            break;
        }
    }
}