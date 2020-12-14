// STL
#include <mutex>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Project
#include "house.h"

#include "../textureManager.h"
#include "../shaderProgramManager.h"

namespace static_meshes_3D {

const std::string House::WALLS_TEXTURE_KEY = "house_walls";
const std::string House::DOOR_WALL_TEXTURE_KEY = "house_door_wall";
const std::string House::WINDOWS_WALL_TEXTURE_KEY = "house_windows_wall";
const std::string House::ROOF_TEXTURE_KEY = "house_roof";

House::House(bool withPositions, bool withTextureCoordinates, bool withNormals)
    : StaticMesh3D(withPositions, withTextureCoordinates, withNormals)
    , cube(withPositions, withTextureCoordinates, withNormals)
    , pyramid(withPositions, withTextureCoordinates, withNormals)
{
    static std::once_flag prepareOnceFlag;
    std::call_once(prepareOnceFlag, []()
    {
        auto& tm = TextureManager::getInstance();
        tm.loadTexture2D(WALLS_TEXTURE_KEY, "data/textures/brick.png");
        tm.loadTexture2D(DOOR_WALL_TEXTURE_KEY, "data/textures/tut008/house_texture_front.png");
        tm.loadTexture2D(WINDOWS_WALL_TEXTURE_KEY, "data/textures/tut008/house_texture_side.png");
        tm.loadTexture2D(ROOF_TEXTURE_KEY, "data/textures/prismarine_dark.png");
    });
}

void House::render() const
{
    render(glm::vec3(0.0f, 0.0f, 0.0f), 0.0f);
}

void House::render(const glm::vec3& position, float rotationAngle) const
{
    auto& mainProgram = ShaderProgramManager::getInstance().getShaderProgram("main");
    const Texture& wallsTexture = TextureManager::getInstance().getTexture(WALLS_TEXTURE_KEY);
    const Texture& doorWallTexture = TextureManager::getInstance().getTexture(DOOR_WALL_TEXTURE_KEY);
    const Texture& windowsWallTexture = TextureManager::getInstance().getTexture(WINDOWS_WALL_TEXTURE_KEY);
    const Texture& roofTexture = TextureManager::getInstance().getTexture(ROOF_TEXTURE_KEY);

    const auto houseBottomSize = 10.0f;
    const auto roofTopSize = 12.0f;
    // First, calculate the basic position of house
    auto modelMatrixHouse = glm::mat4(1.0);
    modelMatrixHouse = glm::translate(modelMatrixHouse, position);

    // Render bottom cube of the house
    glm::mat4 modelMatrixBottom = glm::translate(modelMatrixHouse, glm::vec3(0.0f, houseBottomSize / 2.0f, 0.0f));
    modelMatrixBottom = glm::rotate(modelMatrixBottom, rotationAngle, glm::vec3(0.0f, 1.0f, 0.0f));
    modelMatrixBottom = glm::scale(modelMatrixBottom, glm::vec3(houseBottomSize, houseBottomSize, houseBottomSize));

    mainProgram[ShaderConstants::modelMatrix()] = modelMatrixBottom;

    // Render just plain house walls made of bricks
    wallsTexture.bind();
    cube.renderFaces(CUBE_BACK_FACE | CUBE_TOP_FACE);
    // Render house wall with a door and windows
    doorWallTexture.bind();
    cube.renderFaces(CUBE_FRONT_FACE);
    // Render house wall with a window
    windowsWallTexture.bind();
    cube.renderFaces(CUBE_LEFT_FACE | CUBE_RIGHT_FACE);
    
    roofTexture.bind();
    const auto translateTopY = houseBottomSize + roofTopSize / 2.0f - 0.25f;
    glm::mat4 modelMatrixTop = glm::translate(modelMatrixHouse, glm::vec3(0.0f, translateTopY, 0.0f));
    modelMatrixTop = glm::rotate(modelMatrixTop, rotationAngle, glm::vec3(0.0f, 1.0f, 0.0f));
    modelMatrixTop = glm::scale(modelMatrixTop, glm::vec3(roofTopSize, roofTopSize, roofTopSize));
    mainProgram[ShaderConstants::modelMatrix()] = modelMatrixTop;
    pyramid.render();
}

} // namespace static_meshes_3D