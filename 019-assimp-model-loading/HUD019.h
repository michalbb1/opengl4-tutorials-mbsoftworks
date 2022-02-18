#pragma once

// Project
#include "../common_classes/OpenGLWindow.h"
#include "../common_classes/HUD.h"

namespace opengl4_mbsoftworks {
namespace tutorial019 {

/**
 * HUD for tutorial 019 (assimp model loading).
 */
class HUD019 : public HUD
{
public:
    HUD019(const OpenGLWindow& window);

    /**
     * Renders HUD.
     */
    void renderHUD() const override {} // Don't need this, but had to override, so that class is not abstract
    void renderHUD(const bool displayNormals) const;
};

} // namespace tutorial019
} // namespace opengl4_mbsoftworks