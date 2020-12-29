#version 440 core

layout (location = 0) in int inType;
layout (location = 1) in vec3 inPosition;
layout (location = 2) in vec3 inVelocity;
layout (location = 3) in float inLifetime;
layout (location = 4) in float inSize;

out int ioType;
out vec3 ioPosition;
out vec3 ioVelocity;
out float ioLifetime;
out float ioSize;

void main()
{
    ioType = inType;
    ioPosition = inPosition;
    ioVelocity = inVelocity;
    ioLifetime = inLifetime;
    ioSize = inSize;
}