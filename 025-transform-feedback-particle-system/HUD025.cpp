// STL
#include <mutex>

// Project
#include "HUD025.h"

#include "../common_classes/textureManager.h"
#include "../common_classes/matrixManager.h"
#include "../common_classes/ostreamUtils.h"

using namespace ostream_utils;

HUD025::HUD025(const OpenGLWindow& window, const shader_structs::AmbientLight& ambientLight)
    : HUD(window)
    , ambientLight_(ambientLight)
{
    static std::once_flag prepareOnceFlag;
    std::call_once(prepareOnceFlag, []()
    {
        FreeTypeFontManager::getInstance().loadSystemFreeTypeFont(DEFAULT_FONT_KEY, "arial.ttf", 24);
    });
}

void HUD025::renderHUD(int numFireParticlesOnScene, int numSnowParticlesOnScene, bool isFogEnabled, bool isFireDragged) const
{
    printBuilder().print(10, 10, "FPS: {}", _window.getFPS());
    printBuilder().print(10, 40, "Vertical Synchronization: {} (Press F3 to toggle)", _window.isVerticalSynchronizationEnabled() ? "On" : "Off");
    
    // Print information about ambient light
    printBuilder().print(10, 70, "Ambient light: {} (Press '+' and '-' to change)", ambientLight_.color);

    printBuilder().print(10, 100, "Particles on scene:");
    printBuilder().print(10, 130, "  - Fire: {}", numFireParticlesOnScene);
    printBuilder().print(10, 160, "  - Snow: {}", numSnowParticlesOnScene);
    printBuilder().print(10, 190, "Fog: {} (Press 'F' to toggle)", isFogEnabled ? "On" : "Off");
    printBuilder().print(10, 220, "Drag fire: {} (Press Space to toggle)", isFireDragged ? "Yes" : "No");

    printBuilder()
        .fromRight()
        .fromBottom()
        .print(10, 10, "www.mbsoftworks.sk");
}
