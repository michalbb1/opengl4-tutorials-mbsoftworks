#version 440 core

precision highp float;

layout(location = 0) out vec4 outputColor;

smooth in vec3 ioVertexColor;

void main()
{
	outputColor = vec4(ioVertexColor, 1.0);
}