#version 430 core

out vec4 FragColor;

in vec2 texCoord;
in vec3 normal;
in vec3 fragPos;

uniform sampler2D textureAtlas;

uniform vec3 lightColor;
uniform vec3 viewPos;

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;

void main()
{
    // ambient
    vec3 ambient = light.ambient;
  	
    // diffuse 
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff;
    
    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = light.specular * spec;  
        
    vec3 result = (ambient + diffuse + specular) * lightColor * vec3(texture(textureAtlas, texCoord));
    FragColor = vec4(result, 1.0);
}