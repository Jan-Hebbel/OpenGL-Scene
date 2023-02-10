#version 430 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec3 aNormal;

out vec2 texCoord;
out vec3 normal;
out vec3 fragPos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
	texCoord = aTexCoord;
	normal = mat3(transpose(inverse(model))) * aNormal;
	fragPos = vec3(model * vec4(aPosition, 1.0));
	gl_Position = projection * view * vec4(fragPos, 1.0);
}