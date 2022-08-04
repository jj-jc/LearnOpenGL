#version 330 core

in vec4 vertexColor;
in vec2 TextureCoords;

out vec4 FragColor;

// texture sampler
uniform sampler2D textureSampler;
uniform sampler2D textureSampler2;

void main()
{
    // FragColor = texture(textureSampler1, TextureCoords) * vec4(vertexColor);
    FragColor = mix(texture(textureSampler, TextureCoords), texture(textureSampler2, TextureCoords), 0.5);
}