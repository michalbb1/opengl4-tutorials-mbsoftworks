#pragma once

// GLM
#include <glm/glm.hpp>

// Project
#include "../common_classes/shader_structs/pointLight.h"

namespace opengl4_mbsoftworks {
namespace tutorial024 {

/**
  Class that extends point light shader structure by moving it around
  the world and reversing the direction at the world edges.
*/
class PointLightExtended : public shader_structs::PointLight
{
public:
    static const double MAX_TRAVEL_DISTANCE;

    glm::vec3 direction; // current direction of the point light

    PointLightExtended(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& color, const float ambientFactor,
        const float constantAttenuation, const float linearAttenuation, const float exponentialAttenuation,
        const bool isOn = true)
        : PointLight(position, color, ambientFactor, constantAttenuation, linearAttenuation, exponentialAttenuation, isOn)
        , direction(direction) {}

    /**
     * Updates point light's position and reverses its direction, when it reaches
     * any edge of the world.
     *
     * @param moveBy  By how much should the light move
     * @param y       Y position to set light to (so that it follows heightmap nicely)
     */
    void update(float moveBy, const float y);

    /**
     * Creates point light at given position with given direction.
     * The color of point light is assigned from the color table.
     * If created outside of the world, clamps its position to the edge.
     *
     * @param position  Position where the light should be generated
     * @param direction Direction in which the light should travel
     */
    static PointLightExtended createRandomPointLight(glm::vec3 position, glm::vec3 direction);
};

} // namespace tutorial024
} // namespace opengl4_mbsoftworks