// STL
#include <iostream>
#include <random>

// Project
#include "heightmapWithFog.h"

#include "../../common_classes/textureManager.h"
#include "../../common_classes/shaderManager.h"
#include "../../common_classes/shaderProgramManager.h"

namespace static_meshes_3D {

const std::string HeightmapWithFog::MULTILAYER_SHADER_PROGRAM_WITH_FOG_KEY = "multilayer_heightmap_fog";

HeightmapWithFog::HeightmapWithFog(const HillAlgorithmParameters& params, bool withPositions, bool withTextureCoordinates, bool withNormals)
    : Heightmap(params, withPositions, withTextureCoordinates, withNormals)
{
    createFromHeightData(generateRandomHeightData(params));
}

HeightmapWithFog::HeightmapWithFog(const std::string& fileName, bool withPositions, bool withTextureCoordinates, bool withNormals)
    : Heightmap(fileName, withPositions, withTextureCoordinates, withNormals)
{
    const auto heightData = getHeightDataFromImage(fileName);
    if (heightData.size() == 0) {
        return;
    }

    createFromHeightData(heightData);
}

void HeightmapWithFog::prepareMultiLayerShaderProgramWithFog()
{
    auto& sm = ShaderManager::getInstance();
    
    if (!sm.containsFragmentShader(ShaderKeys::ambientLight())
        || !sm.containsFragmentShader(ShaderKeys::diffuseLight())) {
        throw std::runtime_error("Please load fragment shaders for ambient light and diffuse light before calling this method!");
    }

    sm.loadVertexShader(MULTILAYER_SHADER_PROGRAM_WITH_FOG_KEY, "data/shaders/heightmap/multilayer_fog.vert");
    sm.loadFragmentShader(MULTILAYER_SHADER_PROGRAM_WITH_FOG_KEY, "data/shaders/heightmap/multilayer_fog.frag");

    auto& spm = ShaderProgramManager::getInstance();

    auto& multiLayerHeightmapShaderProgramWithFog = spm.createShaderProgram(MULTILAYER_SHADER_PROGRAM_WITH_FOG_KEY);
    multiLayerHeightmapShaderProgramWithFog.addShaderToProgram(sm.getVertexShader(MULTILAYER_SHADER_PROGRAM_WITH_FOG_KEY));
    multiLayerHeightmapShaderProgramWithFog.addShaderToProgram(sm.getFragmentShader(MULTILAYER_SHADER_PROGRAM_WITH_FOG_KEY));

    multiLayerHeightmapShaderProgramWithFog.addShaderToProgram(sm.getFragmentShader(ShaderKeys::ambientLight()));
    multiLayerHeightmapShaderProgramWithFog.addShaderToProgram(sm.getFragmentShader(ShaderKeys::diffuseLight()));
    multiLayerHeightmapShaderProgramWithFog.addShaderToProgram(sm.getFragmentShader(ShaderKeys::fog()));
}

ShaderProgram& HeightmapWithFog::getMultiLayerShaderProgramWithFog()
{
    return ShaderProgramManager::getInstance().getShaderProgram(MULTILAYER_SHADER_PROGRAM_WITH_FOG_KEY);
}

void HeightmapWithFog::renderMultilayered(const std::vector<std::string>& textureKeys, const std::vector<float> levels) const
{
    if (!_isInitialized) {
        return;
    }

    // If there are less than 2 textures, does not even make sense to render heightmap in multilayer way
    if (textureKeys.size() < 2) {
        return;
    }

    // Number of levels defined must be correct
    if ((textureKeys.size() - 1) * 2 != levels.size()) {
        return;
    }

    // Bind chosen textures first
    const auto& tm = TextureManager::getInstance();
    auto& heightmapShaderProgram = getMultiLayerShaderProgramWithFog();
    for (auto i = 0; i < static_cast<int>(textureKeys.size()); i++)
    {
        tm.getTexture(textureKeys[i]).bind(i);
        heightmapShaderProgram[Heightmap::ShaderConstants::terrainSampler(i)] = i;
    }

    // Set uniform levels
    heightmapShaderProgram[Heightmap::ShaderConstants::numLevels()] = static_cast<int>(levels.size());
    heightmapShaderProgram[Heightmap::ShaderConstants::levels()] = levels;

    // Finally render heightmap
    render();
}

} // namespace static_meshes_3D