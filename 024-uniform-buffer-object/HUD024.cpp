#include <mutex>

#include "HUD024.h"

#include "../common_classes/textureManager.h"
#include "../common_classes/matrixManager.h"
#include "../common_classes/ostreamUtils.h"

using namespace ostream_utils;

HUD024::HUD024(const OpenGLWindow& window)
	: HUD(window)
{
	static std::once_flag prepareOnceFlag;
	std::call_once(prepareOnceFlag, []()
	{
		FreeTypeFontManager::getInstance().loadSystemFreeTypeFont(DEFAULT_FONT_KEY, "arial.ttf", 24);
	});
}

void HUD024::renderHUD(const shader_structs::AmbientLight& ambientLight, const shader_structs::PointLight& pointLight, const int numPointLights) const
{
	printBuilder().print(10, 10, "FPS: {}", _window.getFPS());
	printBuilder().print(10, 40, "Vertical Synchronization: {} (Press F3 to toggle)", _window.isVerticalSynchronizationEnabled() ? "On" : "Off");
	
	// Print information about ambient light
	printBuilder().print(10, 70, "Ambient light: {} (Press '+' and '-' to change)", ambientLight.color);

    // Print information about point light properties
    printBuilder().print(10, 100, "Point light properties (all are same):");
    printBuilder().print(10, 130, "   - count: {} (press 'R' to reset)", numPointLights);
    printBuilder().print(10, 160, "   - constant attenuation: {} (press '1' and '2' to change)", pointLight.constantAttenuation);
    printBuilder().print(10, 190, "   - linear attenuation: {} (press '3' and '4' to change)", pointLight.linearAttenuation);
    printBuilder().print(10, 220, "   - exponential attenuation: {} (press '5' and '6' to change)", pointLight.exponentialAttenuation);

	printBuilder()
		.fromRight()
		.fromBottom()
		.print(10, 10, "www.mbsoftworks.sk");
}
