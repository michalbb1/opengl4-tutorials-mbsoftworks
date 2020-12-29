#version 440 core

layout (location = 1) in vec3 inPosition;
layout (location = 3) in float inLifetime;
layout (location = 4) in float inSize;

out float ioLifetime;
out float ioSize;

void main()
{
    gl_Position = vec4(inPosition, 1.0);
    ioLifetime = inLifetime;
    ioSize = inSize;
}