#version 440 core

layout(location = 0) out vec4 outputColor;

uniform vec4 color;

void main()
{
    outputColor = color;
}