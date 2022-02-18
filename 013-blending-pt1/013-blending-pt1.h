#pragma once

// Project
#include "../common_classes/OpenGLWindow.h"

namespace opengl4_mbsoftworks {
namespace tutorial013 {

class OpenGLWindow013 : public OpenGLWindow
{
public:
    void initializeScene() override;
    void renderScene() override;
    void updateScene() override;
    void releaseScene() override;
};

} // namespace tutorial013
} // namespace opengl4_mbsoftworks