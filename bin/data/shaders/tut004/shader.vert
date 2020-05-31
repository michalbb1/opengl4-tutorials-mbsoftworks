#version 440 core

uniform struct
{
    mat4 projectionMatrix;
    mat4 viewMatrix;
    mat4 modelMatrix;
} matrices;

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexColor;

smooth out vec3 ioVertexColor;

void main()
{
    mat4 mvpMatrix = matrices.projectionMatrix * matrices.viewMatrix * matrices.modelMatrix;
    gl_Position = mvpMatrix * vec4(vertexPosition, 1.0);
    ioVertexColor = vertexColor;
}