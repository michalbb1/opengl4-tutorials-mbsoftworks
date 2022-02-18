#pragma once

// Project
#include "../common_classes/OpenGLWindow.h"
#include "../common_classes/HUD.h"

namespace opengl4_mbsoftworks {
namespace tutorial013 {

/**
 * HUD for tutorial 013.
 */
class HUD013 : public HUD
{
public:
    HUD013(const OpenGLWindow& window);

    /**
     * Renders HUD.
     */
    void renderHUD() const override {} // Don't need this, but had to override, so that class is not abstract
    void renderHUD(bool turnDepthMaskOff) const;
};

} // namespace tutorial013
} // namespace opengl4_mbsoftworks