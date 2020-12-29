#version 440 core

#include "../common/random.glsl"

const int PARTICLE_TYPE_GENERATOR = 0;
const int PARTICLE_TYPE_NORMAL = 1;

layout(points) in;
layout(points) out;
layout(max_vertices = 64) out;

// All that we get from vertex shader
in int ioType[];
in vec3 ioPosition[];
in vec3 ioVelocity[];
in float ioLifetime[];
in float ioSize[];

// All that we send further and that is recorded by transform feedback
out int outType;
out vec3 outPosition;
out vec3 outVelocity;
out vec3 outColor;
out float outLifetime;
out float outSize;

// Position where new particles are generated
uniform vec3 generatedPositionMin;
uniform vec3 generatedPositionRange;

// Velocity of newly generated particles
uniform vec3 generatedVelocityMin;
uniform vec3 generatedVelocityRange;

// Size of newly generated particles
uniform float generatedSizeMin;
uniform float generatedSizeRange;

// Lifetime of newly generated particles
uniform float generatedLifetimeMin;
uniform float generatedLifetimeRange;

// Time passed since last frame (in seconds)
uniform float deltaTime;

// How many particles should be generated during this pass - if greater than zero, then particles are generated
uniform int numParticlesToGenerate;

void main()
{
    initializeRandomNumberGeneratorSeed();

    // First, check if the incoming type of particle is generator
    outType = ioType[0];
    if(outType == PARTICLE_TYPE_GENERATOR)
    {
        // If it's the case, always emit generator particle further
        EmitVertex();
        EndPrimitive();

        // And now generate random particles, if numParticlesToGenerate is greater than zero
        for(int i = 0; i < numParticlesToGenerate; i++)
        {
            outType = PARTICLE_TYPE_NORMAL;
            outPosition = randomVectorMinRange(generatedPositionMin, generatedPositionRange);
            outVelocity = randomVectorMinRange(generatedVelocityMin, generatedVelocityRange);
            outLifetime = randomFloatMinRange(generatedLifetimeMin, generatedLifetimeRange);
            outSize = randomFloatMinRange(generatedSizeMin, generatedSizeRange);
            EmitVertex();
            EndPrimitive();
        }

        return;
    }

    // If we get here, this means we deal with normal fire particle
    // Update its lifetime first and if it survives, emit the particle
    outLifetime = ioLifetime[0] - deltaTime;
    if(outLifetime > 0.0)
    {
        outVelocity = ioVelocity[0];
        outPosition = ioPosition[0] + outVelocity * deltaTime;
        outSize = ioSize[0];
        EmitVertex();
        EndPrimitive(); 
    }
}