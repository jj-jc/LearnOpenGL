#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;

uniform mat4 model;
uniform mat3 normal; // normal matrix to transform the normal vectors into the world coordinates, this avoid translation and problems with non-uniform scale
uniform mat4 view;
uniform mat4 projection;

out vec3 fragmentNorm;
out vec3 fragmentPos;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    fragmentPos = vec3(model * vec4(aPos, 1.0));
    fragmentNorm = normal * normalize(aNorm);
}