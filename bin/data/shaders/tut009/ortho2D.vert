#version 440 core

uniform struct
{
    mat4 projectionMatrix;
    mat4 modelMatrix;
} matrices;

layout(location = 0) in vec2 vertexPosition;
layout(location = 1) in vec2 vertexTexCoord;

smooth out vec2 ioVertexTexCoord;

void main()
{
    mat4 mvpMatrix = matrices.projectionMatrix * matrices.modelMatrix;
    gl_Position = mvpMatrix * vec4(vertexPosition, 0.0, 1.0);
    ioVertexTexCoord = vertexTexCoord;
}