#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
// layout (location = 2) in vec3 aTextureCoords;

out vec4 vertexColor;

void main()
{
    gl_Position = vec4(aPos, 1.0f);
    vertexColor = vec4(aColor, 1.0f); // set the output variable to a dark-red color
}