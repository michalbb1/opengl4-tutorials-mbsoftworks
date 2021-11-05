#version 440 core

#include_part

/**
 * Generates random float from range 0...1.
 *
 * @return Random float from range 0...1.
 */
float randomFloat();

/**
 * Generates random float from a specified range.
 *
 * @param min    Minimal value
 * @param range  Range from minimal value
 *
 * @return Random float from range min...min+range.
 */
float randomFloatMinRange(float min, float range);

/**
 * Generates random integer from a specified range.
 *
 * @param min    Minimal value
 * @param range  Range from minimal value
 *
 * @return Random integer in range min...min+range.
 */
int randomIntMinRange(int min, int range);

/**
 * Generates random vector from a specified range.
 *
 * @param min    Minimal value
 * @param range  Range from minimal value
 *
 * @return Random vector from range min...min+range.
 */
vec3 randomVectorMinRange(vec3 min, vec3 range);

/**
 * Initializes RNG Seed by setting current RNG seed to the one
 * stored in the initial RNG seed.
 */
void initializeRandomNumberGeneratorSeed();

#definition_part

layout(max_vertices = 64) out; // TODO: this must be present here, otherwise this doesn't compile on some Graphic cards

uniform vec3 initialRandomGeneratorSeed; // Initial RNG seed
vec3 currentRandomGeneratorSeed; // Current RNG seed

float randomFloat()
{
    uint n = floatBitsToUint(currentRandomGeneratorSeed.y * 214013.0 + currentRandomGeneratorSeed.x * 2531011.0 + currentRandomGeneratorSeed.z * 141251.0);
    n = n * (n * n * 15731u + 789221u);
    n = (n >> 9u) | 0x3F800000u;

    float result =  2.0 - uintBitsToFloat(n);
    currentRandomGeneratorSeed = vec3(currentRandomGeneratorSeed.x + 147158.0 * result,
        currentRandomGeneratorSeed.y * result  + 415161.0 * result,
        currentRandomGeneratorSeed.z + 324154.0 * result);
    return result;
}

float randomFloatMinRange(float min, float range)
{
    return min + range * randomFloat();
}

int randomIntMinRange(int min, int range)
{
    uint n = floatBitsToUint(currentRandomGeneratorSeed.y * 214013.0 + currentRandomGeneratorSeed.x * 2531011.0 + currentRandomGeneratorSeed.z * 141251.0);
    n = n * (n * n * 15731u + 789221u);
    n = (n >> 9u) | 0x3F800000u;
    float result =  2.0 - uintBitsToFloat(n);

    currentRandomGeneratorSeed = vec3(currentRandomGeneratorSeed.x + 147158.0 * result,
        currentRandomGeneratorSeed.y * result  + 415161.0 * result,
        currentRandomGeneratorSeed.z + 324154.0 * result);

    return min + int(n) % range;
}

vec3 randomVectorMinRange(vec3 min, vec3 range)
{
    return vec3(min.x + range.x * randomFloat(), min.y + range.y * randomFloat(), min.z + range.z * randomFloat());
}

void initializeRandomNumberGeneratorSeed()
{
    currentRandomGeneratorSeed = initialRandomGeneratorSeed;
}