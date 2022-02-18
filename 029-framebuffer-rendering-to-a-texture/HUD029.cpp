// STL
#include <mutex>

// Project
#include "HUD029.h"

#include "../common_classes/ostreamUtils.h"

using namespace ostream_utils;

namespace opengl4_mbsoftworks {
namespace tutorial029 {

HUD029::HUD029(const OpenGLWindow& window)
    : HUD(window)
{
    static std::once_flag prepareOnceFlag;
    std::call_once(prepareOnceFlag, []()
    {
        FreeTypeFontManager::getInstance().loadSystemFreeTypeFont(DEFAULT_FONT_KEY, "arial.ttf", 24);
    });
}

void HUD029::renderHUD(GLsizei currentTextureSize) const
{
    printBuilder().print(10, 10, "FPS: {}", _window.getFPS());
    printBuilder().print(10, 40, "Vertical Synchronization: {} (Press F3 to toggle)", _window.isVerticalSynchronizationEnabled() ? "On" : "Off");

    // Print information about 3D color picking
    printBuilder().print(10, 100, "Current texture size: {}x{} (Press '+' and '-' to change)", currentTextureSize, currentTextureSize);
    printBuilder().print(10, 130, "Press 'X' to re-shuffle meshes on screens");
    printBuilder().print(10, 160, "Press 'C' to re-shuffle textures on screens");

    printBuilder()
        .fromRight()
        .fromBottom()
        .print(10, 10, "www.mbsoftworks.sk");
}

} // namespace tutorial029
} // namespace opengl4_mbsoftworks