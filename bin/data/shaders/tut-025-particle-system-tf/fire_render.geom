#version 440 core

layout(std140, binding = 0) uniform MatricesBlock
{
    mat4 projectionMatrix;
    mat4 viewMatrix;
} block_matrices;

layout(points) in;
layout(triangle_strip) out;
layout(max_vertices = 4) out;

// All that we get from vertex shader
in float ioLifetime[];
in float ioSize[];

// Color of particles
uniform vec3 particlesColor;

// Vectors to render particles as billboards facing camera
uniform vec3 billboardHorizontalVector;
uniform vec3 billboardVerticalVector;

// Output variables
smooth out vec2 ioTexCoord;
smooth out vec4 ioEyeSpacePosition;
flat out vec4 ioColor;

void main()
{
    vec3 particlePosition = gl_in[0].gl_Position.xyz;
    float size = ioSize[0];
    mat4 mVP = block_matrices.projectionMatrix * block_matrices.viewMatrix;

    // Mix particles color with alpha calculated from particle lifetime
    ioColor = vec4(particlesColor, min(1.0, ioLifetime[0]));

    // Emit bottom left vertex
    vec4 bottomLeft = vec4(particlePosition+(-billboardHorizontalVector-billboardVerticalVector)*size, 1.0);
    ioTexCoord = vec2(0.0, 0.0);
    gl_Position = mVP * bottomLeft;
    ioEyeSpacePosition = block_matrices.viewMatrix * bottomLeft;
    EmitVertex();

    // Emit top left vertex
    vec4 topLeft = vec4(particlePosition+(-billboardHorizontalVector+billboardVerticalVector)*size, 1.0);
    ioTexCoord = vec2(0.0, 1.0);
    gl_Position = mVP * topLeft;
    ioEyeSpacePosition = block_matrices.viewMatrix * topLeft;
    EmitVertex();

    // Emit bottom right vertex
    vec4 bottomRight = vec4(particlePosition+(billboardHorizontalVector-billboardVerticalVector)*size, 1.0);
    ioTexCoord = vec2(1.0, 0.0);
    gl_Position = mVP * bottomRight;
    ioEyeSpacePosition = block_matrices.viewMatrix * bottomRight;
    EmitVertex();

    // Emit top right vertex
    vec4 topRight = vec4(particlePosition+(billboardHorizontalVector+billboardVerticalVector)*size, 1.0);
    ioTexCoord = vec2(1.0, 1.0);
    gl_Position = mVP * topRight;
    ioEyeSpacePosition = block_matrices.viewMatrix * topRight;
    EmitVertex();

    // And finally end the primitive
    EndPrimitive();
}