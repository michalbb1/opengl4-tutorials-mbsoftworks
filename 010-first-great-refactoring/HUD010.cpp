// STL
#include <mutex>

// Project
#include "HUD010.h"
#include "../common_classes/textureManager.h"
#include "../common_classes/matrixManager.h"

const std::string HUD010::CHRISTMAS_TREE_TEXTURE_KEY = "christmas_tree";
const std::string HUD010::SNOWFLAKE_TREE_TEXTURE_KEY = "snowflake";

HUD010::HUD010(const OpenGLWindow& window)
    : HUDBase(window)
{
    static std::once_flag prepareOnceFlag;
    std::call_once(prepareOnceFlag, []()
    {
        auto& tm = TextureManager::getInstance();
        tm.loadTexture2D(CHRISTMAS_TREE_TEXTURE_KEY, "data/textures/tut009/christmas_tree.png", false);
        tm.loadTexture2D(SNOWFLAKE_TREE_TEXTURE_KEY, "data/textures/tut009/snowflake.png", false);
    });
}

bool HUD010::isBlendingEnabled() const
{
    return _blendingEnabled;
}

void HUD010::toggleBlending()
{
    _blendingEnabled = !_blendingEnabled;
}

void HUD010::renderHUD() const
{
    auto& shaderProgram = getOrtho2DShaderProgram();
    auto& sampler = getHUDSampler();
    shaderProgram.useProgram();
    sampler.bind();

    glDisable(GL_DEPTH_TEST);
    glDepthMask(0);
    if (_blendingEnabled)
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    shaderProgram[ShaderConstants::projectionMatrix()] = MatrixManager::getInstance().getOrthoProjectionMatrix();
    shaderProgram[ShaderConstants::color()] = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

    // Render Christmas tree bottom left
    const auto& christmasTreeTexture = getChristmasTreeTexture();
    christmasTreeTexture.bind();
    renderTexturedQuad2D(0, 0, christmasTreeTexture.getWidth(), christmasTreeTexture.getHeight());

    // Render snowflake bottom right
    const auto& snowflakeTexture = getSnowflakeTexture();
    snowflakeTexture.bind();
    renderTexturedQuad2D(0, 0, snowflakeTexture.getWidth(), snowflakeTexture.getHeight(), true);

    if (_blendingEnabled) {
        glDisable(GL_BLEND);
    }

    glDepthMask(1);
    glEnable(GL_DEPTH_TEST);
}

const Texture& HUD010::getChristmasTreeTexture() const
{
    return TextureManager::getInstance().getTexture(CHRISTMAS_TREE_TEXTURE_KEY);
}

const Texture& HUD010::getSnowflakeTexture() const
{
    return TextureManager::getInstance().getTexture(SNOWFLAKE_TREE_TEXTURE_KEY);
}
