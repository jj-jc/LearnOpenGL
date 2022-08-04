#version 330 core

in vec3 vertexColor;
in vec2 TextureCoords;

out vec4 FragColor;

// texture sampler
uniform sampler2D textureSampler1;

void main()
{
    FragColor = texture(textureSampler1, TextureCoords) * vec4(vertexColor, 1.0f);
}