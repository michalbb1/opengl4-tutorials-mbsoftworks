#pragma once

// Project
#include "../common_classes/OpenGLWindow.h"
#include "../common_classes/HUD.h"

#include "../common_classes/shader_structs/material.h"

namespace opengl4_mbsoftworks {
namespace tutorial021 {

/**
 * HUD for tutorial 021 (specular highlight).
 */
class HUD021 : public HUD
{
public:
    HUD021(const OpenGLWindow& window);

    /**
     * Renders HUD.
     */
    void renderHUD() const override {} // Don't need this, but had to override, so that class is not abstract
    void renderHUD(const shader_structs::Material& material, const glm::vec3& diffuseLightDirection) const;
};

} // namespace tutorial021
} // namespace opengl4_mbsoftworks