#version 440 core

layout(std140, binding = 0) uniform MatricesBlock
{
    mat4 projectionMatrix;
    mat4 viewMatrix;
} block_matrices;

uniform struct
{
    mat4 modelMatrix;
    mat3 normalMatrix;
} matrices;

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 vertexTexCoord;
layout(location = 2) in vec3 vertexNormal;

smooth out vec2 ioVertexTexCoord;
smooth out vec3 ioVertexNormal;
smooth out vec4 ioWorldPosition;
smooth out vec4 ioEyeSpacePosition;

void main()
{
    mat4 mvMatrix = block_matrices.viewMatrix * matrices.modelMatrix;
    mat4 mvpMatrix = block_matrices.projectionMatrix * mvMatrix;
    gl_Position = mvpMatrix * vec4(vertexPosition, 1.0);
    
    // Output all out variables
    ioVertexTexCoord = vertexTexCoord;
    ioVertexNormal = matrices.normalMatrix * vertexNormal;
    ioWorldPosition = matrices.modelMatrix * vec4(vertexPosition, 1.0);
    ioEyeSpacePosition = mvMatrix * vec4(vertexPosition, 1.0);
}