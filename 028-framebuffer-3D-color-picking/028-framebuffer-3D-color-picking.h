#pragma once

// Project
#include "../common_classes/OpenGLWindow.h"

class OpenGLWindow028 : public OpenGLWindow
{
public:
    void initializeScene() override;
    void renderScene() override;
    void updateScene() override;
    void releaseScene() override;

    void onWindowSizeChanged(int width, int height) override;
    void onMouseButtonPressed(int button, int action) override;
};