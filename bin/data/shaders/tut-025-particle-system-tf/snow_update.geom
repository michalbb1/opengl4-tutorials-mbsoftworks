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
in float ioSize[];
in float ioAlpha[];
in int ioSnowflakeIndex[];

// All that we send further and that is recorded by transform feedback
out int outType;
out vec3 outPosition;
out float outSize;
out float outAlpha;
flat out int outSnowflakeIndex;

// Position where new particles are generated
uniform vec3 generatedPositionMin;
uniform vec3 generatedPositionRange;

// Size of newly generated particles
uniform float generatedSizeMin;
uniform float generatedSizeRange;

// Alpha of newly generated particles
uniform float generatedAlphaMin;
uniform float generatedAlphaRange;

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
            outSize = randomFloatMinRange(generatedSizeMin, generatedSizeRange);
            outAlpha = randomFloatMinRange(generatedAlphaMin, generatedAlphaRange);
            outSnowflakeIndex = randomIntMinRange(0, 4);
            EmitVertex();
            EndPrimitive();
        }

        return;
    }

    // If we get here, this means we deal with normal snow particle
    // Update its Y position and emit the particle only if it's still above zero (ground)
    outPosition = ioPosition[0];
    outPosition.y -= 40.0f * ioSize[0] * deltaTime;
    if(outPosition.y > 0.0)
    {
        outSize = ioSize[0];
        outAlpha = ioAlpha[0];
        outSnowflakeIndex = ioSnowflakeIndex[0];
        EmitVertex();
        EndPrimitive();
    }
}