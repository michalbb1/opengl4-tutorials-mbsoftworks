// STL
#include <mutex>

// Project
#include "HUD017.h"

#include "../common_classes/ostreamUtils.h"

using namespace ostream_utils;

namespace opengl4_mbsoftworks {
namespace tutorial017 {

HUD017::HUD017(const OpenGLWindow& window)
    : HUD(window)
{
    static std::once_flag prepareOnceFlag;
    std::call_once(prepareOnceFlag, []()
    {
        FreeTypeFontManager::getInstance().loadSystemFreeTypeFont(DEFAULT_FONT_KEY, "arial.ttf", 24);
    });
}

void HUD017::renderHUD(const bool displayNormals) const
{
    printBuilder().print(10, 10, "FPS: {}", _window.getFPS());
    printBuilder().print(10, 40, "Vertical Synchronization: {} (Press F3 to toggle)", _window.isVerticalSynchronizationEnabled() ? "On" : "Off");
    
    // Print information about displaying normals
    printBuilder().print(10, 70, "Display Normals: {} (Press 'N' to toggle)", displayNormals ? "On" : "Off");

    printBuilder()
        .fromRight()
        .fromBottom()
        .print(10, 10, "www.mbsoftworks.sk");
}

} // namespace tutorial017
} // namespace opengl4_mbsoftworks