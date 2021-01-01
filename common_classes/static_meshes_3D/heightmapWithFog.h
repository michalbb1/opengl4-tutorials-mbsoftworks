#pragma once

// Project
#include "heightmap.h"

namespace static_meshes_3D {

/**
 *Same as heightmap class, but renders terrain with a fog.
 */
class HeightmapWithFog : public Heightmap
{
public:
    static const std::string MULTILAYER_SHADER_PROGRAM_WITH_FOG_KEY; // Holds a key for multilayer heightmap shader program with a fog (used as shaders key too)

    HeightmapWithFog(const HillAlgorithmParameters& params, bool withPositions = true, bool withTextureCoordinates = true, bool withNormals = true);
    HeightmapWithFog(const std::string& fileName, bool withPositions = true, bool withTextureCoordinates = true, bool withNormals = true);

    static void prepareMultiLayerShaderProgramWithFog();
    static ShaderProgram& getMultiLayerShaderProgramWithFog();

    /**
     * Renders heightmap with multiple layers.
     * @param textureKeys  Contains which textures should be used (ordered from bottom-most to top-most layer)
     * @param levels       Contains where within the heightmap should layer transitions start / stop
     */
    void renderMultilayered(const std::vector<std::string>& textureKeys, const std::vector<float> levels) const;
};

} // namespace static_meshes_3D