#version 440 core

layout(points) in;
layout(points) out;
layout(max_vertices = 40) out;

// All that we get from vertex shader
in int ioType[];
in vec3 ioPosition[];
in vec3 ioVelocity[];
in float ioLifeTime[];
in float ioSize[];

// All that we send further
out int outType;
out vec3 outPosition;
out vec3 outVelocity;
out vec3 outColor;
out float outLifeTime;
out float outSize;

uniform vec3 randomGeneratorSeed; // Seed number for our random number function
vec3 localRandomGeneratorSeed;

uniform vec3 generatedPositionMin; // Position where new particles are spawned
uniform vec3 generatedPositionRange;
uniform vec3 generatedVelocityMin; // Velocity of new particle - from min to (min+range)
uniform vec3 generatedVelocityRange;

uniform float generatedSizeMin;
uniform float generatedSizeRange;

uniform float generatedLifeTimeMin;
uniform float generatedLifeTimeRange; // Life of new particle - from min to (min+range)
uniform float deltaTime; // Time passed since last frame

uniform int numParticlesToGenerate; // How many particles will be generated next time, if greater than zero, particles are generated

// This function returns random number from zero to one
float randomFloat()
{
    uint n = floatBitsToUint(localRandomGeneratorSeed.y * 214013.0 + localRandomGeneratorSeed.x * 2531011.0 + localRandomGeneratorSeed.z * 141251.0);
    n = n * (n * n * 15731u + 789221u);
    n = (n >> 9u) | 0x3F800000u;

    float result =  2.0 - uintBitsToFloat(n);
    localRandomGeneratorSeed = vec3(localRandomGeneratorSeed.x + 147158.0 * result,
        localRandomGeneratorSeed.y * result  + 415161.0 * result,
        localRandomGeneratorSeed.z + 324154.0 * result);
    return result;
}

float randomFloatMinRange(float min, float range)
{
    return min + range * randomFloat();
}

vec3 randomVectorMinRange(vec3 min, vec3 range)
{
    return vec3(min.x + range.x * randomFloat(), min.y + range.y * randomFloat(), min.z + range.z * randomFloat());
}

void main()
{
    localRandomGeneratorSeed = randomGeneratorSeed;

    outType = ioType[0];
    outPosition = ioPosition[0];
    outVelocity = ioVelocity[0];
    outSize = ioSize[0];

    if(outType == 0)
    {
        EmitVertex();
        EndPrimitive();

        for(int i = 0; i < numParticlesToGenerate; i++)
        {
            outType = 1;
            outPosition = randomVectorMinRange(generatedPositionMin, generatedPositionRange);
            outVelocity = randomVectorMinRange(generatedVelocityMin, generatedVelocityRange);
            outLifeTime = randomFloatMinRange(generatedLifeTimeMin, generatedLifeTimeRange);
            outSize = randomFloatMinRange(generatedSizeMin, generatedSizeRange);
            EmitVertex();
            EndPrimitive();
        }

        return;
    }

    outLifeTime = ioLifeTime[0] - deltaTime;
    if(outLifeTime > 0.0)
    {
        outPosition += outVelocity * deltaTime;
        outSize = ioSize[0];
        EmitVertex();
        EndPrimitive(); 
    }
}