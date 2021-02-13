#pragma once

// Project
#include "../common_classes/OpenGLWindow.h"
#include "../common_classes/HUD.h"

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
