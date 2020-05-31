#version 440 core

uniform struct
{
    mat4 projectionMatrix;
    mat4 viewMatrix;
    mat4 modelMatrix;
    mat3 normalMatrix;
} matrices;

layout(points) in;
layout(line_strip, max_vertices = 2) out;

in vec3 ioVertexPosition[];
in vec3 ioVertexNormal[];

uniform float normalLength;

void main()
{
    mat4 mpMatrix = matrices.projectionMatrix * matrices.viewMatrix;
    vec4 firstNormalPoint = matrices.modelMatrix*vec4(ioVertexPosition[0], 1.0);
    gl_Position = mpMatrix * firstNormalPoint;
    EmitVertex();
  
    vec3 transformedNormal = normalize(matrices.normalMatrix*ioVertexNormal[0]);
    vec3 normalWithLength = transformedNormal * normalLength;

    vec4 secondNormalPoint = firstNormalPoint + vec4(normalWithLength, 0.0);
    gl_Position = mpMatrix*secondNormalPoint;
    EmitVertex();

    EndPrimitive();  
}