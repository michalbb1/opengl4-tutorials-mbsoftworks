#pragma once

// Project
#include "../common_classes/OpenGLWindow.h"

class OpenGLWindow027 : public OpenGLWindow
{
public:
    void initializeScene() override;
    void renderScene() override;
    void updateScene() override;
    void releaseScene() override;
};