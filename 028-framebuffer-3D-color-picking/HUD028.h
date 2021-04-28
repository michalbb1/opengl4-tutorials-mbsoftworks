#pragma once

// Project
#include "../common_classes/OpenGLWindow.h"
#include "../common_classes/HUD.h"

/**
 * HUD for tutorial 028 (framebuffer 3D color picking).
 */
class HUD028 : public HUD
{
public:
    HUD028(const OpenGLWindow& window);

    /**
     * Renders HUD.
     */
    void renderHUD() const override {} // Don't need this, but had to override, so that class is not abstract
    void renderHUD(bool isPixelPerfectSelectionMode, bool visualizeColorFrameBuffer, const std::string& selectedObjectDescription, bool updateCamera) const;
};
