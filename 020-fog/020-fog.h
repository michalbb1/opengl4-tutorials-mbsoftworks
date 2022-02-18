#pragma once

// Project
#include "../common_classes/OpenGLWindow.h"

namespace opengl4_mbsoftworks {
namespace tutorial020 {

class OpenGLWindow020 : public OpenGLWindow
{
public:
    void initializeScene() override;
    void renderScene() override;
    void updateScene() override;
    void releaseScene() override;
};

} // namespace tutorial020
} // namespace opengl4_mbsoftworks