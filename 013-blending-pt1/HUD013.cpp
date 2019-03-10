#include <mutex>

#include "HUD013.h"

#include "../common_classes/textureManager.h"
#include "../common_classes/matrixManager.h"

HUD013::HUD013(const OpenGLWindow& window)
	: HUD(window)
{
	static std::once_flag prepareOnceFlag;
	std::call_once(prepareOnceFlag, []()
	{
		FreeTypeFontManager::getInstance().loadSystemFreeTypeFont(DEFAULT_FONT_KEY, "arial.ttf", 24);
	});
}

void HUD013::renderHUD(bool turnDepthMaskOff) const
{
	printBuilder().print(10, 10, "FPS: {}", _window.getFPS());
	printBuilder().print(10, 40, "Vertical Synchronization: {} (Press F3 to toggle)", _window.isVerticalSynchronizationEnabled() ? "On" : "Off");
	printBuilder().print(10, 70, "Turn depth mask off? {} (Press F4 to toggle)", turnDepthMaskOff ? "Yes" : "No");

	printBuilder()
		.fromRight()
		.fromBottom()
		.print(10, 10, "www.mbsoftworks.sk");
}
