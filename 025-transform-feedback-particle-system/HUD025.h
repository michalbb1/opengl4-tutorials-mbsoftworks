#pragma once

// STL
#include <string>

// Project
#include "../common_classes/OpenGLWindow.h"
#include "../common_classes/HUD.h"
#include "../common_classes/texture.h"

#include "../common_classes/shader_structs/ambientLight.h"
#include "../common_classes/shader_structs/pointLight.h"

/**
 * HUD for tutorial 025 (transform feedback particle system).
 */
class HUD025 : public HUD
{
public:
    HUD025(const OpenGLWindow& window, const shader_structs::AmbientLight& ambientLight);

    /**
     * Renders HUD.
     */
    void renderHUD() const override {} // Don't need this, but had to override, so that class is not abstract
    void renderHUD(int numFireParticlesOnScene, int numSnowParticlesOnScene, bool isFogEnabled, bool isFireDragged) const;

private:
    const shader_structs::AmbientLight& ambientLight_;
};
