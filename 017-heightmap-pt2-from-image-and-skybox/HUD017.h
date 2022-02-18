#pragma once

// Project
#include "../common_classes/OpenGLWindow.h"
#include "../common_classes/HUD.h"

namespace opengl4_mbsoftworks {
namespace tutorial017 {

/**
 * HUD for tutorial 017 (heightmap pt.8 - terrain from image and skybox).
 */
class HUD017 : public HUD
{
public:
    HUD017(const OpenGLWindow& window);

    /**
     * Renders HUD.
     */
    void renderHUD() const override {} // Don't need this, but had to override, so that class is not abstract
    void renderHUD(const bool displayNormals) const;
};

} // namespace tutorial017
} // namespace opengl4_mbsoftworks