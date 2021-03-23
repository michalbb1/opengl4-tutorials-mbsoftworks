#pragma once

// STL
#include <random>

// GLM
#include <glm/vec3.hpp>

/**
 * Helper class that makes using C++11's random very convenient.
 */
class Random
{
public:
    /**
     * Gets random integer from range <0 ... bound-1>.
     *
     * @param bound  Upper bound for randomly generated integer (not inclusive).
     */
    static int nextInt(int bound);

    /**
     * Generates random vec3 at XZ plane.
     *
     * @param minPosition  Minimal vertex position that can be generated
     * @param maxPosition  Maximal vertex position that can be generated
     */
    static glm::vec3 getRandomVectorFromRectangleXZ(const glm::vec3& minPosition, const glm::vec3& maxPosition);

private:
    static std::random_device rd_; // STD random number generator for non-deterministic random numbers
    static std::mt19937_64 generator_; // 64-bit Mersenne Twister random number engine
};
