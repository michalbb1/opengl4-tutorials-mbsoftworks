// STL
#include <mutex>

// Project
#include "HUD020.h"

#include "../common_classes/textureManager.h"
#include "../common_classes/matrixManager.h"
#include "../common_classes/ostreamUtils.h"

using namespace ostream_utils;

HUD020::HUD020(const OpenGLWindow& window)
    : HUD(window)
{
    static std::once_flag prepareOnceFlag;
    std::call_once(prepareOnceFlag, []()
    {
        FreeTypeFontManager::getInstance().loadSystemFreeTypeFont(DEFAULT_FONT_KEY, "arial.ttf", 24);
    });
}

void HUD020::renderHUD(const shader_structs::FogParameters& fogParameters) const
{
    printBuilder().print(10, 10, "FPS: {}", _window.getFPS());
    printBuilder().print(10, 40, "Vertical Synchronization: {} (Press F3 to toggle)", _window.isVerticalSynchronizationEnabled() ? "On" : "Off");
    
    // Print information about fog
    printBuilder().print(10, 70, "Fog: {} (Press 'X' to toggle)", fogParameters.isEnabled ? "On" : "Off");
    printBuilder().print(10, 100, "Fog equation: {} (Press 'F' to change)", fogParameters.getFogEquationName());
    if (fogParameters.equation == shader_structs::FogParameters::FOG_EQUATION_LINEAR)
    {
        printBuilder().print(10, 130, "Start distance: {} (Press '1' and '2' to change)", fogParameters.linearStart);
        printBuilder().print(10, 160, "End distance: {} (Press '3' and '4' to change)", fogParameters.linearEnd);
    }
    else if (fogParameters.equation == shader_structs::FogParameters::FOG_EQUATION_EXP
        || fogParameters.equation == shader_structs::FogParameters::FOG_EQUATION_EXP2)
    {
        printBuilder().print(10, 130, "Density: {} (Press '1' and '2' to change)", fogParameters.density);
    }

    printBuilder()
        .fromRight()
        .fromBottom()
        .print(10, 10, "www.mbsoftworks.sk");
}
