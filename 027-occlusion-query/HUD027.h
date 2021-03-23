#pragma once

// Project
#include "../common_classes/OpenGLWindow.h"
#include "../common_classes/HUD.h"

/**
 * HUD for tutorial 027 (occlusion query).
 */
class HUD027 : public HUD
{
public:
    HUD027(const OpenGLWindow& window);

    /**
     * Renders HUD.
     */
    void renderHUD() const override {} // Don't need this, but had to override, so that class is not abstract
    void renderHUD(size_t numObjects, size_t numVisibleObjects, bool isWireframeModeOn, bool visualizeOccluders) const;
};
