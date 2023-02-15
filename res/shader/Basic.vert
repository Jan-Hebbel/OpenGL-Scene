#version 430 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec3 aNormal;
layout(location = 3) in vec2 aSpecMapCoord;

out vec2 texCoord;
out vec2 specMapCoord;
out vec3 normal;
out vec3 fragPos;
out vec4 lightFragPos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 lightVP;

void main()
{
	texCoord = aTexCoord;
	specMapCoord = aSpecMapCoord;
	normal = mat3(transpose(inverse(model))) * aNormal;
	fragPos = vec3(model * vec4(aPosition, 1.0));
	lightFragPos = lightVP * vec4(fragPos, 1.0);
	gl_Position = projection * view * vec4(fragPos, 1.0);
}