#version 440 core

uniform struct Matrices
{
    mat4 mProj;
    mat4 mView;
} matrices;

uniform vec3 billboardHorizontalVector;
uniform vec3 billboardVerticalVector;
uniform vec3 particlesColor;

layout(points) in;
layout(triangle_strip) out;
layout(max_vertices = 4) out;

in float ioLifeTime[];
in float ioSize[];

smooth out vec2 ioTexCoord;
smooth out vec4 ioEyeSpacePosition;
flat out vec4 ioColor;

void main()
{
    vec3 particlePosition = gl_in[0].gl_Position.xyz;
    float size = ioSize[0];
    mat4 mVP = matrices.mProj*matrices.mView;

    ioColor = vec4(particlesColor, min(1.0, ioLifeTime[0]));

    vec4 bottomLeft = vec4(particlePosition+(-billboardHorizontalVector-billboardVerticalVector)*size, 1.0);
    ioTexCoord = vec2(0.0, 0.0);
    gl_Position = mVP * bottomLeft;
    ioEyeSpacePosition = matrices.mView * bottomLeft;
    EmitVertex();

    vec4 topLeft = vec4(particlePosition+(-billboardHorizontalVector+billboardVerticalVector)*size, 1.0);
    ioTexCoord = vec2(0.0, 1.0);
    gl_Position = mVP * topLeft;
    ioEyeSpacePosition = matrices.mView * topLeft;
    EmitVertex();

    vec4 bottomRight = vec4(particlePosition+(billboardHorizontalVector-billboardVerticalVector)*size, 1.0);
    ioTexCoord = vec2(1.0, 0.0);
    gl_Position = mVP * bottomRight;
    ioEyeSpacePosition = matrices.mView * bottomRight;
    EmitVertex();

    vec4 topRight = vec4(particlePosition+(billboardHorizontalVector+billboardVerticalVector)*size, 1.0);
    ioTexCoord = vec2(1.0, 1.0);
    gl_Position = mVP * topRight;
    ioEyeSpacePosition = matrices.mView * topRight;
    EmitVertex();
      
    EndPrimitive();
}