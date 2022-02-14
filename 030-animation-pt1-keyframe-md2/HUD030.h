#pragma once

// Project
#include "../common_classes/OpenGLWindow.h"
#include "../common_classes/HUD.h"

namespace opengl4_mbsoftworks {
namespace tutorial030 {

/**
 * HUD for tutorial 030 (Animation Part 1 Keyframe MD2).
 */
class HUD030 : public HUD
{
public:
    HUD030(const OpenGLWindow& window);

    /**
     * Renders HUD.
     */
    void renderHUD() const override;
};

} // namespace tutorial030
} // namespace opengl4_mbsoftworks