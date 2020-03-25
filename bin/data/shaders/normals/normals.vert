#version 440 core

layout (location = 0) in vec3 vertexPosition;
layout (location = 2) in vec3 vertexNormal;

out vec3 ioVertexPosition;
out vec3 ioVertexNormal;

void main()
{
    ioVertexPosition = vertexPosition;
    ioVertexNormal = vertexNormal;
}