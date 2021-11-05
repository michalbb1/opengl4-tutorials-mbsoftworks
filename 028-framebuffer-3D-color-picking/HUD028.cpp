// STL
#include <mutex>

// Project
#include "HUD028.h"

#include "../common_classes/ostreamUtils.h"

using namespace ostream_utils;

HUD028::HUD028(const OpenGLWindow& window)
    : HUD(window)
{
    static std::once_flag prepareOnceFlag;
    std::call_once(prepareOnceFlag, []()
    {
        FreeTypeFontManager::getInstance().loadSystemFreeTypeFont(DEFAULT_FONT_KEY, "arial.ttf", 24);
    });
}

void HUD028::renderHUD(bool isPixelPerfectSelectionMode, bool visualizeColorFrameBuffer, const std::string& selectedObjectDescription, bool updateCamera) const
{
    printBuilder().print(10, 10, "FPS: {}", _window.getFPS());
    printBuilder().print(10, 40, "Vertical Synchronization: {} (Press F3 to toggle)", _window.isVerticalSynchronizationEnabled() ? "On" : "Off");

    // Print information about 3D color picking
    printBuilder().print(10, 100, "Pixel perfect selection mode: {} (Press 'X' to toggle)", isPixelPerfectSelectionMode ? "On" : "Off");
    printBuilder().print(10, 130, "Visualize color frame buffer: {} (Press 'C' to toggle)", visualizeColorFrameBuffer ? "On" : "Off");
    printBuilder().print(10, 160, "Selected object: {}", selectedObjectDescription);
    printBuilder().print(10, 190, "Update camera: {} (Press 'U' to toggle)", updateCamera ? "Yes" : "No");

    printBuilder()
        .fromRight()
        .fromBottom()
        .print(10, 10, "www.mbsoftworks.sk");
}
