#pragma once

// Project
#include "../common_classes/OpenGLWindow.h"
#include "../common_classes/HUD.h"

namespace opengl4_mbsoftworks {
namespace tutorial016 {

/**
 * HUD for tutorial 016 (heightmap pt.1 - random terrain).
 */
class HUD016 : public HUD
{
public:
    HUD016(const OpenGLWindow& window);

    /**
     * Renders HUD.
     */
    void renderHUD() const override {} // Don't need this, but had to override, so that class is not abstract
    void renderHUD(const bool displayNormals) const;
};

} // namespace tutorial016
} // namespace opengl4_mbsoftworks