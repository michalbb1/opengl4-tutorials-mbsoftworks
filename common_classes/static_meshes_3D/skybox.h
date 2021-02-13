#pragma once

// STL
#include <string>

// GLM
#include <glm/glm.hpp>

// Project
#include "../shaderProgram.h"
#include "primitives/cube.h"

namespace static_meshes_3D {

/**
 * Skybox static mesh - basically cube with some additional textures
 */
class Skybox : public Cube
{
public:
    static const std::string SAMPLER_KEY; // Key to store skybox sampler with

    Skybox(const std::string& baseDirectory, const std::string& imageExtension, bool withPositions = true, bool withTextureCoordinates = true, bool withNormals = true);
    ~Skybox();

    /**
     * Renders skybox around given point.
     * 
     * @param renderPosition  Position to render skybox around (usually around the player or camera)
     * @param shaderProgram   Shader program used for rendering
     */
    void render(const glm::vec3& renderPosition, ShaderProgram& shaderProgram) const;

private:
    std::string _baseDirectory; // Path to directory where skybox images are located
    std::string _imageExtension; // Image extension of images, should be consistent (png, jpg etc.)

    /**
     * Gets filename of a specified skybox side.
     *
     * @param sideBit  Bit that represents the cube side (from cube.h)
     *
     * @return Combined directory path, side name and image extension.
     */
    static std::string getSideFileName(const int sideBit);

    /**
     * Gets texture key of a specified skybox side.
     *
     * @param sideBit  Bit that represents the cube side (from cube.h)
     *
     * @return Key, within which is texture of this skybox stored.
     */
    std::string getTextureKey(const int sideBit) const;

    /**
     * Tries to load the texture of a specified skybox side. If suceeeds, texture is stored in manager.
     * 
     * @param sideBit  Bit that represents the cube side (from cube.h)
     */
    void tryLoadTexture(const int sideBit) const;
};

} // namespace static_meshes_3D