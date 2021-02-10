#pragma once

// Project
#include "../common_classes/OpenGLWindow.h"

class OpenGLWindow023 : public OpenGLWindow
{
public:
    void initializeScene() override;
    void renderScene() override;
    void handleInput() override;
    void releaseScene() override;
};