// this code is heavily inspired by learnopengl.com

#version 430 core

out vec4 FragColor;

in vec2 texCoord;
in vec2 specMapCoord;
in vec3 normal;
in vec3 fragPos;

struct Material {
    sampler2D textureAtlas;
    float shininess;
};

struct Light 
{
    vec3 color;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct DirLight {
    Light light;

    vec3 direction;
};

struct PointLight {
    Light light;

    vec3 position;

    float constant;
    float linear;
    float quadratic;
};

uniform vec3 viewPos;
uniform Material material;
uniform DirLight dirLight;
uniform PointLight pointLight;

vec3 CalcLight(Light light, vec3 normal, vec3 viewDir, vec3 lightDir) 
{
    // colors
    vec3 tex = vec3(texture(material.textureAtlas, texCoord));
    // replacing specMapCoord with or setting it to texCoord will result in specular lighting
    vec3 specMap = vec3(texture(material.textureAtlas, specMapCoord)); 

    // ambient
    vec3 ambient = light.ambient * tex;
  	
    // diffuse 
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * tex;
    
    // specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * specMap;
        
    return (ambient + diffuse + specular) * light.color;
}

vec3 CalcDirLight(DirLight dirLight, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-dirLight.direction);

    return CalcLight(dirLight.light, normal, viewDir, lightDir);
}

vec3 CalcPointLight(PointLight pLight, vec3 normal, vec3 viewDir)
{
    vec3 position = pointLight.position;

    vec3 lightDir = normalize(position - fragPos);

    // attenuation
    float dist = length(position - fragPos);
    float attenuation = 1.0 / 
        (pLight.constant + pLight.linear * dist + pLight.quadratic * (dist * dist));

    return attenuation * CalcLight(pointLight.light, normal, viewDir, lightDir);
}

void main()
{
    vec3 norm = normalize(normal);
    vec3 viewDir = normalize(viewPos - fragPos);

    vec3 result = vec3(0.0);
    result += CalcDirLight(dirLight, norm, viewDir);
    result += CalcPointLight(pointLight, norm, viewDir);
    //result += CalcSpotLight();
    FragColor = vec4(result, 1.0);
}