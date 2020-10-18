// Project
#include "pointLightExtended.h"

const double PointLightExtended::MAX_TRAVEL_DISTANCE{ 200.0 };

void PointLightExtended::update(const float speedMultiplier, const float y)
{
    const auto newPosition = position + direction * speedMultiplier;
    position += direction * speedMultiplier;

    if (newPosition.x >= MAX_TRAVEL_DISTANCE || newPosition.x <= -MAX_TRAVEL_DISTANCE
        || newPosition.z >= MAX_TRAVEL_DISTANCE || newPosition.z <= -MAX_TRAVEL_DISTANCE)
    {
        // Reverse light direction in case we would go beyong the world edges
        direction *= -1.0f;
    }
    else
    {
        // Otherwise simply update the position
        position = newPosition;
        position.y = y;
    }
}

PointLightExtended PointLightExtended::createRandomPointLight(glm::vec3 position, glm::vec3 direction)
{
    // Color table and index of next color when generating point light
    static int nextColorIndex = 0;
    static const glm::vec3 colors[] =
    {
        glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 0.0f, 1.0f),
        glm::vec3(0.0f, 1.0f, 1.0f),
        glm::vec3(0.5f, 0.0f, 1.0f),
        glm::vec3(1.0f, 0.5f, 0.0f)
    };
    static const int numColors = sizeof(colors) / sizeof(glm::vec3);

    // Clamp the position to the world edges
    if (position.x < -MAX_TRAVEL_DISTANCE) {
        position.x = -MAX_TRAVEL_DISTANCE;
    }

    if (position.x > MAX_TRAVEL_DISTANCE) {
        position.x = MAX_TRAVEL_DISTANCE;
    }

    if (position.z < -MAX_TRAVEL_DISTANCE) {
        position.z = -MAX_TRAVEL_DISTANCE;
    }

    if (position.z > MAX_TRAVEL_DISTANCE) {
        position.z = MAX_TRAVEL_DISTANCE;
    }

    // Ignore Y component of the direction vector
    direction.y = 0.0f;
    direction = glm::normalize(direction);
    
    // Take next color and advance next color index
    const auto color = colors[nextColorIndex];
    nextColorIndex = (nextColorIndex + 1) % numColors;

    return PointLightExtended(position, direction, color, 0.0f, 0.3f, 0.007f, 0.00008f);
}
