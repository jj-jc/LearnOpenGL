#version 330 core
// stores properties specific of each surface
struct Material
{
    sampler2D diffuse;
    sampler2D specular;
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
// structs of different types of lightning sources
// Direction Light (sun)
struct DirLight
{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
// Point Light (light builb)
struct PointLight
{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    // attenuation
    float constant;
    float linear;
    float quadratic;
};
// Spot Light (lantern)
struct SpotLight
{
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};


#define NR_POINT_LIGHTS 2

in vec3 fragmentPos;
in vec3 fragmentNorm;
in vec2 TexCoords;

// uniform vec3 lightPos;
uniform vec3 viewPos;

uniform Material material;
uniform DirLight dirLight;
uniform PointLight pointLight[NR_POINT_LIGHTS];
uniform SpotLight spotLight;

out vec4 fragmentColor;

vec3 calcDirlight(DirLight light, vec3 fragmentNorm, vec3 viewDir);
vec3 calcPointLight(PointLight light, vec3 fragmentNorm, vec3 fragmentPos, vec3 viewDir);
vec3 calcSpotLight(SpotLight light, vec3 fragmentNorm, vec3 fragmentPos, vec3 viewDir);


void main()
{
    vec3 viewDir = normalize(viewPos - fragmentPos);
    vec3 result = vec3(0.0);
    // now there are three types of lightning sources with 3 components each. 
    // add the directional light's contribution to the result
    // result += calcDirlight(dirLight, fragmentNorm, viewDir);
    // add the point light's contribution to the result
    for (int i = 0; i < NR_POINT_LIGHTS; i++)
    {
        // result += calcPointLight(pointLight[i], fragmentNorm, fragmentPos, viewDir);
    }
    // add the flash light's contribution to the result ( this is a spotlight in the view position)
    result += calcSpotLight(spotLight, fragmentNorm, fragmentPos, viewDir); 
    
    fragmentColor = vec4(result, 1.0f);
}

vec3 calcDirlight(DirLight light, vec3 fragmentNorm, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse
    float diff = max(dot(fragmentNorm, lightDir), 0.0);
    //specular
    vec3 reflectDir = reflect(-lightDir, fragmentNorm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // calculate the contribution 
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

    return (ambient + diffuse + specular);
}

vec3 calcPointLight(PointLight light, vec3 fragmentNorm, vec3 fragmentPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragmentPos);
    float diff = max(dot(fragmentNorm, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, fragmentNorm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // attenuation 
    float distanceToFragment = length(light.position - fragmentPos);
    float attenuation = 1.0 / (light.constant + light.linear * distanceToFragment + light.quadratic * distanceToFragment * distanceToFragment);

    // contributions
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

    return attenuation * (ambient + diffuse + specular); 

}

vec3 calcSpotLight(SpotLight light, vec3 fragmentNorm, vec3 fragmentPos, vec3 viewDir)
{ // works all the time with cos of angles
//TODO: optimise calculate the intensity only when theta > outerOff
    vec3 fragment2LightDir = normalize(light.position -fragmentPos);
    vec3 lightDir = normalize(-light.direction);
    float theta = dot(fragment2LightDir, lightDir);
    float epsilon = light.cutOff - light.outerOff;
    float intensity = clamp((theta - light.outerOff)/epsilon, 0.0, 1.0);

    float diff = max(dot(fragmentNorm, lightDir), 0.0);
    vec3 reflectDir = reflect(-fragment2LightDir, fragmentNorm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // contributions
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec* vec3(texture(material.specular, TexCoords));

    return ambient + intensity * (diffuse + specular);

}