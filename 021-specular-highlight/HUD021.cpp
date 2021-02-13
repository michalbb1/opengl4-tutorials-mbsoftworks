// STL
#include <mutex>

// Project
#include "HUD021.h"

#include "../common_classes/matrixManager.h"
#include "../common_classes/ostreamUtils.h"

using namespace ostream_utils;

HUD021::HUD021(const OpenGLWindow& window)
    : HUD(window)
{
    static std::once_flag prepareOnceFlag;
    std::call_once(prepareOnceFlag, []()
    {
        FreeTypeFontManager::getInstance().loadSystemFreeTypeFont(DEFAULT_FONT_KEY, "arial.ttf", 24);
    });
}

void HUD021::renderHUD(const shader_structs::Material& material, const glm::vec3& diffuseLightDirection) const
{
    printBuilder().print(10, 10, "FPS: {}", _window.getFPS());
    printBuilder().print(10, 40, "Vertical Synchronization: {} (Press F3 to toggle)", _window.isVerticalSynchronizationEnabled() ? "On" : "Off");
    
    // Print information about material
    printBuilder().print(10, 70, "Specular highlight:");
    printBuilder().print(10, 100, "   - calculation: {} (Press 'X' to toggle)", material.isEnabled ? "On" : "Off");
    if (material.isEnabled)
    {
        printBuilder().print(10, 130, "   - specular intensity: {} (Press '1' and '2' to change)", material.specularIntensity);
        printBuilder().print(10, 160, "   - specular power: {} (Press '3' and '4' to change)", material.specularPower);
    }
    printBuilder().print(10, 190, "Diffuse light direction: {} (Press 'L' to lock)", diffuseLightDirection);

    printBuilder()
        .fromRight()
        .fromBottom()
        .print(10, 10, "www.mbsoftworks.sk");
}
