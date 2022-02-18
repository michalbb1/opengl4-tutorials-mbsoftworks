#pragma once

// Project
#include "../common_classes/OpenGLWindow.h"

namespace opengl4_mbsoftworks {
namespace tutorial023 {

class OpenGLWindow023 : public OpenGLWindow
{
public:
    void initializeScene() override;
    void renderScene() override;
    void updateScene() override;
    void releaseScene() override;
};

} // namespace tutorial023
} // namespace opengl4_mbsoftworks