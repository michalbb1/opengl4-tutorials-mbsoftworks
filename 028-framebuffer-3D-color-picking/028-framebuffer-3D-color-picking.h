#pragma once

// Project
#include "../common_classes/OpenGLWindow.h"

namespace opengl4_mbsoftworks {
namespace tutorial028 {

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

} // namespace tutorial028
} // namespace opengl4_mbsoftworks