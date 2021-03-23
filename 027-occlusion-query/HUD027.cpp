// STL
#include <mutex>

// Project
#include "HUD027.h"

#include "../common_classes/ostreamUtils.h"

using namespace ostream_utils;

HUD027::HUD027(const OpenGLWindow& window)
    : HUD(window)
{
    static std::once_flag prepareOnceFlag;
    std::call_once(prepareOnceFlag, []()
    {
        FreeTypeFontManager::getInstance().loadSystemFreeTypeFont(DEFAULT_FONT_KEY, "arial.ttf", 24);
    });
}

void HUD027::renderHUD(size_t numObjects, size_t numVisibleObjects, bool isWireframeModeOn, bool visualizeOccluders) const
{
    printBuilder().print(10, 10, "FPS: {}", _window.getFPS());
    printBuilder().print(10, 40, "Vertical Synchronization: {} (Press F3 to toggle)", _window.isVerticalSynchronizationEnabled() ? "On" : "Off");

    // Calculate percentage of visible objects
    auto visiblePercentage = 0.0f;
    if(numObjects > 0)
    {
        visiblePercentage = 100.0f * static_cast<float>(numVisibleObjects) / numObjects;
    }

    // Print information about wireframe mode and occlusion query statistics
    printBuilder().print(10, 70, "Wireframe mode: {} (Press 'X' to toggle)", isWireframeModeOn ? "On" : "Off");
    printBuilder().print(10, 100, "Visualize occluders: {} (Press 'C' to toggle)", visualizeOccluders ? "On" : "Off");
    printBuilder().print(10, 130, "  - Visible / total objects: {} / {} ({}%)", numVisibleObjects, numObjects, visiblePercentage);

    printBuilder()
        .fromRight()
        .fromBottom()
        .print(10, 10, "www.mbsoftworks.sk");
}
