#version 440 core

uniform struct
{
    mat4 projectionMatrix;
    mat4 viewMatrix;
    mat4 modelMatrix;
    mat3 normalMatrix;
} matrices;

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec2 vertexTexCoord;
layout (location = 2) in vec3 vertexNormal;
layout (location = 3) in vec3 nextVertexPosition;
layout (location = 4) in vec3 nextVertexNormal;

smooth out vec2 ioVertexTexCoord;
smooth out vec3 ioVertexNormal;
smooth out vec4 ioWorldPosition;
smooth out vec4 ioEyeSpacePosition;

uniform float interpolationFactor;

void main()
{
    mat4 mvMatrix = matrices.viewMatrix * matrices.modelMatrix;
    mat4 mvpMatrix = matrices.projectionMatrix * mvMatrix;

    vec4 interpolatedPosition = vec4(vertexPosition + (nextVertexPosition - vertexPosition)*interpolationFactor, 1.0);
    vec3 interpolatedNormal = vertexNormal + (nextVertexNormal - vertexNormal)*interpolationFactor;
    gl_Position = mvpMatrix*interpolatedPosition;
    ioVertexTexCoord = vertexTexCoord;
    ioEyeSpacePosition = mvMatrix * interpolatedPosition;
    ioVertexNormal = matrices.normalMatrix * interpolatedNormal;
    ioWorldPosition = matrices.modelMatrix * interpolatedPosition;
}