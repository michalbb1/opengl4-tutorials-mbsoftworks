#include <mutex>
#include <iomanip>

#include "HUD014.h"

#include "../common_classes/textureManager.h"
#include "../common_classes/matrixManager.h"

std::ostream &operator<<(std::ostream &o, const glm::vec3 vec3)
{
	return o << std::setprecision(3) << "[" << vec3.x << ", " << vec3.y << ", " << vec3.z << "]";
}

HUD014::HUD014(const OpenGLWindow& window)
	: HUD(window)
{
	static std::once_flag prepareOnceFlag;
	std::call_once(prepareOnceFlag, []()
	{
		FreeTypeFontManager::getInstance().loadSystemFreeTypeFont(DEFAULT_FONT_KEY, "arial.ttf", 24);
	});
}

void HUD014::renderHUD(const glm::vec3& ambientColor, float diffuseFactor) const
{
	printBuilder().print(10, 10, "FPS: {}", _window.getFPS());
	printBuilder().print(10, 40, "Vertical Synchronization: {} (Press F3 to toggle)", _window.isVerticalSynchronizationEnabled() ? "On" : "Off");
	printBuilder().print(10, 70, "Ambient color: {} (Press '3' and '4' to change)", ambientColor);
	printBuilder().print(10, 100, "Diffuse factor: {} (Press '1' and '2' to change)", diffuseFactor);

	printBuilder()
		.fromRight()
		.fromBottom()
		.print(10, 10, "www.mbsoftworks.sk");
}
