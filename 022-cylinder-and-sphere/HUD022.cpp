// STL
#include <mutex>

// Project
#include "HUD022.h"

#include "../common_classes/matrixManager.h"
#include "../common_classes/ostreamUtils.h"

using namespace ostream_utils;

HUD022::HUD022(const OpenGLWindow& window)
    : HUD(window)
{
    static std::once_flag prepareOnceFlag;
    std::call_once(prepareOnceFlag, []()
    {
        FreeTypeFontManager::getInstance().loadSystemFreeTypeFont(DEFAULT_FONT_KEY, "arial.ttf", 24);
    });
}

void HUD022::renderHUD(const glm::vec3& diffuseLightDirection, bool displayNormals, bool updateSpaceEntities, int sphereSlices, int sphereStacks) const
{
    // Basic information like FPS or V-Sync
    printBuilder().print(10, 10, "FPS: {}", _window.getFPS());
    printBuilder().print(10, 40, "Vertical Synchronization: {} (Press F3 to toggle)", _window.isVerticalSynchronizationEnabled() ? "On" : "Off");
    
    // Diffuse light
    printBuilder().print(10, 70, "Diffuse light direction: {} (Press 'L' to lock)", diffuseLightDirection);

    // Sphere parameters
    printBuilder().print(10, 100, "Sphere parameters:");
    printBuilder().print(40, 130, "Slices: {} (Press '1' and '2' to alter)", sphereSlices);
    printBuilder().print(40, 160, "Stacks: {} (Press '3' and '4' to alter)", sphereStacks);
    
    // Space entities
    printBuilder().fromBottom().print(10, 40, "Update space entities: {} (Press 'X' to toggle)", updateSpaceEntities ? "On" : "Off");

    // Normals
    printBuilder().fromBottom().print(10, 10, "Display Normals: {} (Press 'N' to toggle)", displayNormals ? "On" : "Off");

    printBuilder()
        .fromRight()
        .fromBottom()
        .print(10, 10, "www.mbsoftworks.sk");
}
