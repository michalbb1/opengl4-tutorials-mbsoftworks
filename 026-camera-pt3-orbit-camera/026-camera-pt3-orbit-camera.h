#pragma once

// Project
#include "../common_classes/OpenGLWindow.h"

class OpenGLWindow026 : public OpenGLWindow
{
public:
    void initializeScene() override;
    void renderScene() override;
    void handleInput() override;
    void releaseScene() override;
    void onMouseWheelScroll(double scrollOffsetX, double scrollOffsetY) override;
};