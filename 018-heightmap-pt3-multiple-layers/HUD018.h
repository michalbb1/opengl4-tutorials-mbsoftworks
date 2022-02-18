#pragma once

// Project
#include "../common_classes/OpenGLWindow.h"
#include "../common_classes/HUD.h"

namespace opengl4_mbsoftworks {
namespace tutorial018 {

/**
 * HUD for tutorial 018 (heightmap pt.1 - random terrain).
 */
class HUD018 : public HUD
{
public:
    HUD018(const OpenGLWindow& window);

    /**
     * Renders HUD.
     */
    void renderHUD() const override {} // Don't need this, but had to override, so that class is not abstract
    void renderHUD(bool displayNormals) const;
};

} // namespace tutorial018
} // namespace opengl4_mbsoftworks