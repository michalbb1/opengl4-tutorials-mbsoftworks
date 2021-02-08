#pragma once

// Project
#include "../common_classes/OpenGLWindow.h"

class OpenGLWindow026 : public OpenGLWindow
{
public:
    void onMouseWheelScroll(double scrollOffsetX, double scrollOffsetY) override;
};