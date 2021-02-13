// STL
#include <mutex>

// Project
#include "HUDBase.h"
#include "shaderManager.h"
#include "shaderProgramManager.h"
#include "samplerManager.h"

// GLM
#include <glm/gtc/matrix_transform.hpp>

const std::string HUDBase::DEFAULT_FONT_KEY = "default";
const std::string HUDBase::ORTHO_2D_PROGRAM_KEY = "ortho_2D";
const std::string HUDBase::HUD_SAMPLER_KEY = "HUD";

HUDBase::HUDBase(const OpenGLWindow& window)
    : _window(window)
    , _texturedQuad(true, true)
{
    static std::once_flag prepareOnceFlag;
    std::call_once(prepareOnceFlag, []()
    {
        auto& sm = ShaderManager::getInstance();
        sm.loadVertexShader(ORTHO_2D_PROGRAM_KEY, "data/shaders/tut009/ortho2D.vert");
        sm.loadFragmentShader(ORTHO_2D_PROGRAM_KEY, "data/shaders/tut009/ortho2D.frag");

        auto& spm = ShaderProgramManager::getInstance();
        auto& shaderProgram = spm.createShaderProgram(ORTHO_2D_PROGRAM_KEY);
        shaderProgram.addShaderToProgram(sm.getVertexShader(ORTHO_2D_PROGRAM_KEY));
        shaderProgram.addShaderToProgram(sm.getFragmentShader(ORTHO_2D_PROGRAM_KEY));

        auto& smm = SamplerManager::getInstance();
        smm.createSampler(HUD_SAMPLER_KEY, MAG_FILTER_BILINEAR, MIN_FILTER_BILINEAR);
    });
}

int HUDBase::getWidth() const
{
    int width, height;
    glfwGetFramebufferSize(_window.getWindow(), &width, &height);
    return width;
}

int HUDBase::getHeight() const
{
    int width, height;
    glfwGetFramebufferSize(_window.getWindow(), &width, &height);
    return height;
}

void HUDBase::renderTexturedQuad2D(int x, int y, int renderedWidth, int renderedHeight, bool fromRight, bool fromTop) const
{
    if (fromRight) {
        x = getWidth() - x - renderedWidth;
    }
    if (fromTop) {
        y = getHeight() - y - renderedHeight;
    }

    auto& shaderProgram = getOrtho2DShaderProgram();
    glm::mat4 model = glm::translate(glm::mat4(1.0), glm::vec3(static_cast<float>(x), static_cast<float>(y), 0.0f));
    model = glm::scale(model, glm::vec3(static_cast<float>(renderedWidth), static_cast<float>(renderedHeight), 1.0f));
    shaderProgram[ShaderConstants::modelMatrix()] = model;
    
    _texturedQuad.render();
}

ShaderProgram& HUDBase::getOrtho2DShaderProgram() const
{
    return ShaderProgramManager::getInstance().getShaderProgram(ORTHO_2D_PROGRAM_KEY);
}

const Sampler& HUDBase::getHUDSampler() const
{
    return SamplerManager::getInstance().getSampler(HUD_SAMPLER_KEY);
}
