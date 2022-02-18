#pragma once

// Project
#include "../common_classes/OpenGLWindow.h"
#include "../common_classes/HUD.h"

namespace opengl4_mbsoftworks {
namespace tutorial022 {

/**
 * HUD for tutorial 022 (cylinder and sphere).
 */
class HUD022 : public HUD
{
public:
    HUD022(const OpenGLWindow& window);

    /**
     * Renders HUD.
     */
    void renderHUD() const override {} // Don't need this, but had to override, so that class is not abstract
    void renderHUD(const glm::vec3& diffuseLightDirection, bool displayNormals, bool updateSpaceEntities, int sphereSlices, int sphereStacks) const;
};

} // namespace tutorial022
} // namespace opengl4_mbsoftworks