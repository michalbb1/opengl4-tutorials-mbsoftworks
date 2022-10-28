// Project
#include "random.h"

std::random_device Random::rd_;
std::mt19937_64 Random::generator_{rd_()};

int Random::nextInt(int bound)
{
    std::uniform_int_distribution<int> distribution(0, bound - 1);
    return distribution(generator_);
}

glm::vec3 Random::getRandomVectorFromRectangleXZ(const glm::vec3& minPosition, const glm::vec3& maxPosition)
{
    std::uniform_real_distribution<float> distributionX(minPosition.x, maxPosition.x);
    std::uniform_real_distribution<float> distributionZ(minPosition.z, maxPosition.z);

    return glm::vec3(distributionX(generator_), minPosition.y, distributionZ(generator_));
}
