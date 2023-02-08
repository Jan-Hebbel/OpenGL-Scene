#version 430 core

out vec4 FragColor;

in vec2 texCoord;
in vec3 normal;
in vec3 fragPos;

layout(location = 3) uniform sampler2D textureAtlas;

layout(location = 4) uniform vec3 lightPos;
layout(location = 5) uniform vec3 lightColor;

void main()
{
	// ambient lighting
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;

	// diffuse lighting
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(lightPos - fragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	vec3 result = (ambient + diffuse) * vec3(texture(textureAtlas, texCoord));
	FragColor = vec4(result, 1.0);
}