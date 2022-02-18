#pragma once

// Project
#include "../common_classes/OpenGLWindow.h"
#include "../common_classes/HUD.h"

namespace opengl4_mbsoftworks {
namespace tutorial012 {

/**
 * HUD for tutorial 012.
 */
class HUD012 : public HUD
{
public:
    HUD012(const OpenGLWindow& window);

    /**
     * Renders HUD.
     */
    void renderHUD() const override;
};

} // namespace tutorial012
} // namespace opengl4_mbsoftworks