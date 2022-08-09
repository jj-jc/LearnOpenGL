#version 330 core
// stores properties specific of each surface
struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 
// stores the features of color in Light
struct Light
{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 fragmentPos;
in vec3 fragmentNorm;

uniform vec3 lightPos;
uniform vec3 viewPos;

out vec4 fragmentColor;


uniform Material material;
uniform Light light;

void main()
{
    // ambient
    vec3 ambient = light.ambient * material.ambient;
    // diffuse
    vec3 normal = normalize(fragmentNorm);
    vec3 lightDir = normalize(lightPos - fragmentPos);
    float diff = max(dot(normal, lightDir), 0);
    vec3 diffuse = light.diffuse * diff * material.diffuse;
    // specular
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 viewDir = normalize(viewPos - fragmentPos);
    float spec = pow(max(dot(viewDir, reflectDir), 0), material.shininess);
    vec3 specular = light.specular * spec * material.specular;

    vec3 result = ambient + diffuse + specular;
    fragmentColor = vec4(result, 1.0f);
}