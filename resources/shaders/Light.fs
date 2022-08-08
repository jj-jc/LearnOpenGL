#version 330 core

in vec3 fragmentPos;
in vec3 fragmentNorm;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

out vec4 fragmentColor;

void main()
{
    // ambient lightning
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor; 
    // diffuse lightning
    vec3 lightDir = normalize(lightPos - fragmentPos);
    vec3 diffuse = max(dot(fragmentNorm, lightDir), 0.0f) * lightColor; 
    // specular lightning
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - fragmentPos);
    vec3 reflectionDir = reflect(-lightDir, fragmentNorm);
    vec3 specular = specularStrength * pow(max(dot(viewDir, reflectionDir), 0), 256) * lightColor; // shininess of n = 32 higher -> brighter (2, 4, 8, 16, 32, 64, 128, 256);

    // apply the lightning result with the 3 components
    vec3 result = (specular + ambient + diffuse) * objectColor;
    fragmentColor = vec4(result, 1.0);
}