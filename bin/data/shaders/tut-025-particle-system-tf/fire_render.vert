#version 440 core

layout (location = 1) in vec3 vertexPosition;
layout (location = 3) in float lifeTime;
layout (location = 4) in float size;

out float ioLifeTime;
out float ioSize;

void main()
{
    gl_Position = vec4(vertexPosition, 1.0);
    ioLifeTime = lifeTime;
    ioSize = size;
}