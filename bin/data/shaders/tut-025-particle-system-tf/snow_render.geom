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

in float ioSize[];
in float ioAlpha[];
in int ioSnowflakeIndex[];

smooth out vec2 ioTexCoord;
smooth out vec4 ioEyeSpacePosition;
flat out vec4 ioColor;

void main()
{
    vec3 particlePosition = gl_in[0].gl_Position.xyz;
    particlePosition.x += 0.5 * sin(particlePosition.y * 0.3);

    int snowflakeIndex = ioSnowflakeIndex[0];
    int row = snowflakeIndex / 2;
    int col = snowflakeIndex % 2;
    vec2 texCoordBase = vec2(0.5 * col, 0.5 - row * 0.5);

    float size = ioSize[0];
    mat4 mVP = matrices.mProj*matrices.mView;

    ioColor = vec4(particlesColor, ioAlpha[0]);

    vec4 bottomLeft = vec4(particlePosition+(-billboardHorizontalVector-billboardVerticalVector)*size, 1.0);
    ioTexCoord = texCoordBase;
    gl_Position = mVP * bottomLeft;
    ioEyeSpacePosition = matrices.mView * bottomLeft;
    EmitVertex();

    vec4 topLeft = vec4(particlePosition+(-billboardHorizontalVector+billboardVerticalVector)*size, 1.0);
    ioTexCoord = texCoordBase + vec2(0.0, 0.5);
    gl_Position = mVP * topLeft;
    ioEyeSpacePosition = matrices.mView * topLeft;
    EmitVertex();

    vec4 bottomRight = vec4(particlePosition+(billboardHorizontalVector-billboardVerticalVector)*size, 1.0);
    ioTexCoord = texCoordBase + vec2(0.5, 0.0);
    gl_Position = mVP * bottomRight;
    ioEyeSpacePosition = matrices.mView * bottomRight;
    EmitVertex();

    vec4 topRight = vec4(particlePosition+(billboardHorizontalVector+billboardVerticalVector)*size, 1.0);
    ioTexCoord = texCoordBase + vec2(0.5, 0.5);
    gl_Position = mVP * topRight;
    ioEyeSpacePosition = matrices.mView * topRight;
    EmitVertex();
      
    EndPrimitive();
}