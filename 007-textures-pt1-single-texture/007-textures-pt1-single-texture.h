#pragma once

// Project
#include "../common_classes/OpenGLWindow.h"

namespace opengl4_mbsoftworks {
namespace tutorial007 {

class OpenGLWindow007 : public OpenGLWindow
{
public:
    void initializeScene() override;
    void renderScene() override;
    void updateScene() override;
    void releaseScene() override;
};

} // namespace tutorial007
} // namespace opengl4_mbsoftworks