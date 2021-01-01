#pragma once

// STL
#include <string>

// Project
#include "../common_classes/OpenGLWindow.h"
#include "../common_classes/HUD.h"
#include "../common_classes/texture.h"

#include "../common_classes/shader_structs/ambientLight.h"
#include "../common_classes/shader_structs/diffuseLight.h"

/**
 * HUD for tutorial 017 (heightmap pt.8 - terrain from image and skybox).
 */
class HUD017 : public HUD
{
public:
    HUD017(const OpenGLWindow& window);

    /**
     * Renders HUD.
     */
    void renderHUD() const override {} // Don't need this, but had to override, so that class is not abstract
    void renderHUD(const bool displayNormals) const;
};
