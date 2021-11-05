#version 440 core

uniform struct
{
	mat4 projectionMatrix;
	mat4 viewMatrix;
	mat4 modelMatrix;
} matrices;

layout(location = 0) in vec3 vertexPosition;

void main()
{
    mat4 mvpMatrix = matrices.projectionMatrix * matrices.viewMatrix * matrices.modelMatrix;
    gl_Position = mvpMatrix * vec4(vertexPosition, 1.0);
}