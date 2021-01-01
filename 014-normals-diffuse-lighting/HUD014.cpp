// STL
#include <mutex>

// Project
#include "HUD014.h"

#include "../common_classes/textureManager.h"
#include "../common_classes/matrixManager.h"
#include "../common_classes/ostreamUtils.h"

using namespace ostream_utils;

HUD014::HUD014(const OpenGLWindow& window)
    : HUD(window)
{
    static std::once_flag prepareOnceFlag;
    std::call_once(prepareOnceFlag, []()
    {
        FreeTypeFontManager::getInstance().loadSystemFreeTypeFont(DEFAULT_FONT_KEY, "arial.ttf", 24);
    });
}

void HUD014::renderHUD(const shader_structs::AmbientLight& ambientLight, const shader_structs::DiffuseLight& diffuseLight) const
{
    printBuilder().print(10, 10, "FPS: {}", _window.getFPS());
    printBuilder().print(10, 40, "Vertical Synchronization: {} (Press F3 to toggle)", _window.isVerticalSynchronizationEnabled() ? "On" : "Off");

    // Print information about ambient light
    printBuilder().print(10, 70, "Ambient light: {} (Press 'Z' to toggle)", ambientLight.isOn ? "On" : "Off");
    printBuilder().print(10, 100, " - color: {} (Press '1' and '2' to change)", ambientLight.color);

    // Print information about diffuse light
    printBuilder().print(10, 130, "Diffuse light: {} (Press 'X' to toggle)", diffuseLight.isOn ? "On" : "Off");
    printBuilder().print(10, 160, " - factor: {} (Press '3' and '4' to change)", diffuseLight.factor);
    printBuilder().print(10, 190, " - direction: {} (Press 'L' to lock)", diffuseLight.direction);

    printBuilder()
        .fromRight()
        .fromBottom()
        .print(10, 10, "www.mbsoftworks.sk");
}
