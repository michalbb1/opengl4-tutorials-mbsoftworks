#pragma once

#include <string>

#include "../common_classes/OpenGLWindow.h"
#include "../common_classes/HUD.h"
#include "../common_classes/texture.h"

/**
  HUD for tutorial 012.
*/
class HUD012 : public HUD
{
public:
	HUD012(const OpenGLWindow& window);

	/** \brief  Renders HUD. */
	void renderHUD() const override;
};
