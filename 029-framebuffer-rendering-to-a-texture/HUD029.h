#pragma once

// Project
#include "../common_classes/OpenGLWindow.h"
#include "../common_classes/HUD.h"

namespace opengl4_mbsoftworks {
namespace tutorial029 {

/**
 * HUD for tutorial 029 (framebuffer rendering to a texture).
 */
class HUD029 : public HUD
{
public:
    HUD029(const OpenGLWindow& window);

    /**
     * Renders HUD.
     */
    void renderHUD() const override {} // Don't need this, but had to override, so that class is not abstract
    void renderHUD(GLsizei currentTextureSize) const;
};

} // namespace tutorial029
} // namespace opengl4_mbsoftworks