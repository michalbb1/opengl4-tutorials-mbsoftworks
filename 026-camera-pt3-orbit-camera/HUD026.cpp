// STL
#include <mutex>

// Project
#include "HUD026.h"

#include "../common_classes/matrixManager.h"
#include "../common_classes/ostreamUtils.h"

using namespace ostream_utils;

namespace opengl4_mbsoftworks {
namespace tutorial026 {

HUD026::HUD026(const OpenGLWindow& window)
    : HUD(window)
{
    static std::once_flag prepareOnceFlag;
    std::call_once(prepareOnceFlag, []()
    {
        FreeTypeFontManager::getInstance().loadSystemFreeTypeFont(DEFAULT_FONT_KEY, "arial.ttf", 24);
    });
}

void HUD026::renderHUD(const OrbitCamera& orbitCamera, bool isWireframeModeOn) const
{
    printBuilder().print(10, 10, "FPS: {}", _window.getFPS());
    printBuilder().print(10, 40, "Vertical Synchronization: {} (Press F3 to toggle)", _window.isVerticalSynchronizationEnabled() ? "On" : "Off");

    // Print information about orbit camera
    printBuilder().print(10, 70, "Orbit Camera (rotate with LMB pressed, move with MMB pressed):");
    printBuilder().print(10, 100, "  - Azimuth angle: {} deg", glm::degrees(orbitCamera.getAzimuthAngle()));
    printBuilder().print(10, 130, "  - Polar angle: {} deg", glm::degrees(orbitCamera.getPolarAngle()));
    printBuilder().print(10, 160, "  - Zoom (or radius): {} (change by scrolling with mouse wheel)", orbitCamera.getRadius());
    printBuilder().print(10, 190, "Wireframe mode: {} (Press 'X' to toggle)", isWireframeModeOn ? "On" : "Off");

    printBuilder()
        .fromRight()
        .fromBottom()
        .print(10, 10, "www.mbsoftworks.sk");
}

} // namespace tutorial026
} // namespace opengl4_mbsoftworks