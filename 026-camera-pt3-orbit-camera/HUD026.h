#pragma once

// Project
#include "../common_classes/OpenGLWindow.h"
#include "../common_classes/HUD.h"

#include "../common_classes/orbitCamera.h"

namespace opengl4_mbsoftworks {
namespace tutorial026 {

/**
 * HUD for tutorial 026 (orbit camera).
 */
class HUD026 : public HUD
{
public:
    HUD026(const OpenGLWindow& window);

    /**
     * Renders HUD.
     */
    void renderHUD() const override {} // Don't need this, but had to override, so that class is not abstract
    void renderHUD(const OrbitCamera& orbitCamera, bool isWireframeModeOn) const;
};

} // namespace tutorial026
} // namespace opengl4_mbsoftworks