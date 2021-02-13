// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Project
#include "spaceStation.h"
#include "../common_classes/shaderProgramManager.h"
#include "../common_classes/textureManager.h"

SpaceStation::SpaceStation(const glm::vec3& position, float sphereRadius, float cylinderRadius)
    : position_(position)
    , sphereRadius_(sphereRadius)
    , cylinderRadius_(cylinderRadius)
{
    cylinderHeight_ = sphereRadius_ * 1.25f;
}

shader_structs::Material SpaceStation::material(1.0f, 32.0f);
std::unique_ptr<static_meshes_3D::Sphere> SpaceStation::sphereMesh;
std::unique_ptr<static_meshes_3D::Cylinder> SpaceStation::cylinderMesh;

void SpaceStation::render() const
{
    const auto& spm = ShaderProgramManager::getInstance();
    const auto& tm = TextureManager::getInstance();
    auto& mainProgram = spm.getShaderProgram("main");

    tm.getTexture("scifi_metal").bind();

    mainProgram.setModelAndNormalMatrix(getSphereModelMatrix());
    sphereMesh->render();

    for (const auto& cylinderMatrix : getAllCylinderMatrices())
    {
        mainProgram.setModelAndNormalMatrix(cylinderMatrix);
        cylinderMesh->render();
    }
}

void SpaceStation::renderPoints() const
{
    auto& normalsProgram = ShaderProgramManager::getInstance().getShaderProgram("normals");
    normalsProgram.setModelAndNormalMatrix(getSphereModelMatrix());
    sphereMesh->renderPoints();

    for (const auto& cylinderMatrix : getAllCylinderMatrices())
    {
        normalsProgram.setModelAndNormalMatrix(cylinderMatrix);
        cylinderMesh->renderPoints();
    }
}

void SpaceStation::update()
{
    const auto& window = OpenGLWindow::getDefaultWindow();
    sphereAngle_ += window->sof(0.25f);
    cylindersAngle_ += window->sof(0.5f);
}

void SpaceStation::initializeGeometry()
{
    sphereMesh = std::make_unique<static_meshes_3D::Sphere>(1.0f, 10, 6);
    cylinderMesh = std::make_unique<static_meshes_3D::Cylinder>(1.0f, 20, 1.0f);
}

void SpaceStation::freeGeometry()
{
    sphereMesh.reset();
    cylinderMesh.reset();
}

glm::mat4 SpaceStation::getSphereModelMatrix() const
{
    auto modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position_);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(sphereRadius_, sphereRadius_ * 0.25f, sphereRadius_));
    modelMatrix = glm::rotate(modelMatrix, sphereAngle_, glm::vec3(0.0f, 1.0f, 0.0f));
    return modelMatrix;
}

glm::mat4 SpaceStation::getFrontCylinderMatrix() const
{
    auto modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position_);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, -cylinderHeight_));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, cylindersAngle_, glm::vec3(0.0f, 1.0f, 0.0f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(cylinderRadius_, cylinderHeight_, cylinderRadius_));
    return modelMatrix;
}

glm::mat4 SpaceStation::getRightCylinderMatrix() const
{
    auto modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position_);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(cylinderHeight_, 0.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    modelMatrix = glm::rotate(modelMatrix, cylindersAngle_, glm::vec3(0.0f, 1.0f, 0.0f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(cylinderRadius_, cylinderHeight_, cylinderRadius_));
    return modelMatrix;
}

glm::mat4 SpaceStation::getBackCylinderMatrix() const
{
    auto modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position_);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, cylinderHeight_));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, cylindersAngle_, glm::vec3(0.0f, 1.0f, 0.0f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(cylinderRadius_, cylinderHeight_, cylinderRadius_));
    return modelMatrix;
}

glm::mat4 SpaceStation::getLeftCylinderMatrix() const
{
    auto modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position_);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-cylinderHeight_, 0.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    modelMatrix = glm::rotate(modelMatrix, cylindersAngle_, glm::vec3(0.0f, 1.0f, 0.0f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(cylinderRadius_, cylinderHeight_, cylinderRadius_));
    return modelMatrix;
}

glm::mat4 SpaceStation::getTopCylinderMatrix() const
{
    auto modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position_);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, sphereRadius_ * 0.25f + cylinderHeight_ * 0.4f, 0.0f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(cylinderRadius_, cylinderHeight_, cylinderRadius_));
    modelMatrix = glm::rotate(modelMatrix, cylindersAngle_, glm::vec3(0.0f, 1.0f, 0.0f));
    return modelMatrix;
}

glm::mat4 SpaceStation::getBottomCylinderMatrix() const
{
    auto modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position_);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, -sphereRadius_ - 0.25f + cylinderHeight_ * 0.4f, 0.0f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(cylinderRadius_, cylinderHeight_, cylinderRadius_));
    modelMatrix = glm::rotate(modelMatrix, cylindersAngle_, glm::vec3(0.0f, 1.0f, 0.0f));
    return modelMatrix;
}

std::vector<glm::mat4> SpaceStation::getAllCylinderMatrices() const
{
    return
    {
        getFrontCylinderMatrix(),
        getRightCylinderMatrix(),
        getBackCylinderMatrix(),
        getLeftCylinderMatrix(),
        getTopCylinderMatrix(),
        getBottomCylinderMatrix()
    };
}
