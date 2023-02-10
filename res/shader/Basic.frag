#version 430 core

out vec4 FragColor;

in vec2 texCoord;
in vec2 specMapCoord;
in vec3 normal;
in vec3 fragPos;

uniform vec3 viewPos;

struct Material {
    sampler2D textureAtlas;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 color;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;
uniform Material material;

void main()
{
    vec3 tex = vec3(texture(material.textureAtlas, texCoord));
    vec3 specMap = vec3(texture(material.textureAtlas, specMapCoord));

    // ambient
    vec3 ambient = light.ambient * tex;
  	
    // diffuse 
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * tex;
    
    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * specMap;
        
    vec3 result = (ambient + diffuse + specular) * light.color;
    FragColor = vec4(result, 1.0);
}