#pragma once

// Project
#include "../common_classes/OpenGLWindow.h"

namespace opengl4_mbsoftworks {
namespace tutorial017 {

class OpenGLWindow017 : public OpenGLWindow
{
public:
    void initializeScene() override;
    void renderScene() override;
    void updateScene() override;
    void releaseScene() override;
};

} // namespace tutorial017
} // namespace opengl4_mbsoftworks