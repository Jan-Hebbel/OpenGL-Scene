#version 430 core

out vec4 FragColor;

in vec3 texCoords;

layout(location = 2) uniform samplerCube skybox;

void main()
{    
    FragColor = texture(skybox, texCoords);
}