#version 440 core

layout (location = 0) in int inType;
layout (location = 1) in vec3 inPosition;
layout (location = 2) in float inSize;
layout (location = 3) in float inAlpha;
layout (location = 4) in int inSnowflakeIndex;

out int ioType;
out vec3 ioPosition;
out float ioSize;
out float ioAlpha;
out int ioSnowflakeIndex;

void main()
{
    ioType = inType;
    ioPosition = inPosition;
    ioSize = inSize;
    ioAlpha = inAlpha;
    ioSnowflakeIndex = inSnowflakeIndex;
}