#pragma once

// Project
#include "../common_classes/OpenGLWindow.h"

namespace opengl4_mbsoftworks {
namespace tutorial026 {

class OpenGLWindow026 : public OpenGLWindow
{
public:
    void initializeScene() override;
    void renderScene() override;
    void updateScene() override;
    void releaseScene() override;
    void onMouseWheelScroll(double scrollOffsetX, double scrollOffsetY) override;
};

} // namespace tutorial026
} // namespace opengl4_mbsoftworks