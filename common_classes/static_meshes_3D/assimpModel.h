#pragma once

// STL
#include <vector>
#include <map>

// Assimp
#include <assimp/Importer.hpp>

// GLM
#include <glm/glm.hpp>

// Project
#include "staticMesh3D.h"

namespace static_meshes_3D {

/**
 * Represents 3D model loaded with Assimp library.
 */
class AssimpModel : public StaticMesh3D
{
public:
    AssimpModel(const std::string& filePath, const std::string& defaultTextureName, bool withPositions, bool withTextureCoordinates, bool withNormals, const glm::mat4& modelTransformMatrix = glm::mat4(1.0f));
    AssimpModel(const std::string& filePath, bool withPositions = true, bool withTextureCoordinates = true, bool withNormals = true, const glm::mat4& modelTransformMatrix = glm::mat4(1.0f));

    /**
     * Loads a model from a given file using Assimp library. Default texture name should be provided,
     * if Assimp can't parse material properties from models (happens with many free models from internet).
     *
     * @param filePath              File path to load model from (can be of any format supported by Assimp)
     * @param defaultTextureName    Optional default texture name, if model would be loaded without textures
     * @param modelTransformMatrix  Optional parameter to transform the model data (sometimes the model is strangely rotated, big, small etc.)
     *
     * @return True, if model has been loaded successfully or false otherwise.
     */
    bool loadModelFromFile(const std::string& filePath, const std::string& defaultTextureName = "", const glm::mat4& modelTransformMatrix = glm::mat4(1.0f));

    void render() const override;
    void renderPoints() const override;

protected:
    void loadMaterialTexture(const int materialIndex, const std::string& textureFileName);
    static std::string aiStringToStdString(const aiString& aiStringStruct);

    std::string _modelRootDirectoryPath; // Path of the directory where model (and possibly its assets) is located
    std::vector<int> _meshStartIndices; // Indices of where the meshes start in the VBO
    std::vector<int> _meshVerticesCount; // How many vertices are there for every mesh
    std::vector<int> _meshMaterialIndices; // Index of material for every mesh
    std::map<int, std::string> _materialTextureKeys; // Map for index of material -> texture key to be retrieved from TextureManager
};

}; // namespace static_meshes_3D