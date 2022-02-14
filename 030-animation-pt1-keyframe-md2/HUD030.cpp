// STL
#include <mutex>

// Project
#include "HUD030.h"
#include "modelCollection.h"
#include "../common_classes/ostreamUtils.h"

using namespace ostream_utils;

namespace opengl4_mbsoftworks {
namespace tutorial030 {

HUD030::HUD030(const OpenGLWindow& window)
    : HUD(window)
{
    static std::once_flag prepareOnceFlag;
    std::call_once(prepareOnceFlag, []()
    {
        FreeTypeFontManager::getInstance().loadSystemFreeTypeFont(DEFAULT_FONT_KEY, "arial.ttf", 24);
    });
}

void HUD030::renderHUD() const
{
    const auto& modelCollection = ModelCollection::getInstance();
    printBuilder().print(10, 10, "FPS: {}", _window.getFPS());
    printBuilder().print(10, 40, "Vertical Synchronization: {} (Press F3 to toggle)", _window.isVerticalSynchronizationEnabled() ? "On" : "Off");

    // Print information about 3D color picking
    printBuilder().print(10, 100, "Selected model: {} (Press 'Z' and 'X' to change)", modelCollection.getCurrentModelName());
    printBuilder().print(10, 130, "Selected animation: {} ({} / {}) ", modelCollection.getCurrentAnimationName(), modelCollection.getCurrentAnimationIndex() + 1, modelCollection.getNumModelAnimations());
    printBuilder().print(10, 160, "Loop animation: {} (Press 'Q' to toggle)", modelCollection.isAnimationLooping() ? "Yes" : "No");

    printBuilder()
        .fromRight()
        .fromBottom()
        .print(10, 10, "www.mbsoftworks.sk");
}

} // namespace tutorial030
} // namespace opengl4_mbsoftworks