#pragma once

// Project
#include "../common_classes/OpenGLWindow.h"
#include "../common_classes/HUD.h"

#include "../common_classes/shader_structs/fogParameters.h"

/**
 * HUD for tutorial 020 (fog).
 */
class HUD020 : public HUD
{
public:
    HUD020(const OpenGLWindow& window);

    /** \brief  Renders HUD. */
    void renderHUD() const override {} // Don't need this, but had to override, so that class is not abstract
    void renderHUD(const shader_structs::FogParameters& fogParameters) const;
};
