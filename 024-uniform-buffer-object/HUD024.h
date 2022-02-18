#pragma once

// Project
#include "../common_classes/OpenGLWindow.h"
#include "../common_classes/HUD.h"

#include "../common_classes/shader_structs/ambientLight.h"
#include "../common_classes/shader_structs/pointLight.h"

namespace opengl4_mbsoftworks {
namespace tutorial024 {

/**
 * HUD for tutorial 024 (uniform buffer object).
 */
class HUD024 : public HUD
{
public:
    HUD024(const OpenGLWindow& window);

    /**
     * Renders HUD.
     */
    void renderHUD() const override {} // Don't need this, but had to override, so that class is not abstract
    void renderHUD(const shader_structs::AmbientLight& ambientLight, const shader_structs::PointLight& pointLight, const int numPointLights) const;
};

} // namespace tutorial024
} // namespace opengl4_mbsoftworks