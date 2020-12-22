#version 440 core

layout (location = 1) in vec3 inPosition;
layout (location = 2) in float inSize;
layout (location = 3) in float inAlpha;
layout (location = 4) in int inSnowflakeIndex;

out float ioSize;
out float ioAlpha;
out int ioSnowflakeIndex;

void main()
{
    gl_Position = vec4(inPosition, 1.0);
    ioSize = inSize;
    ioAlpha = inAlpha;
    ioSnowflakeIndex = inSnowflakeIndex;
}