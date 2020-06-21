// STL
#include <mutex>

// glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Project
#include "planet.h"
#include "../common_classes/shaderProgramManager.h"
#include "../common_classes/textureManager.h"

std::unique_ptr<static_meshes_3D::Sphere> Planet::sphereMesh;

Planet::Planet(const std::string& textureKey, const shader_structs::Material& material, float radius, float distanceFromCenter, float orbitalPeriodDays, float rotationAngleSpeed, float initialOrbitAngle)
    : _textureKey(textureKey)
    , _material(material)
    , _radius(radius)
    , _distanceFromCenter(distanceFromCenter)
    , _travelAngleSpeed(orbitalPeriodDays == 0.0f ? 0.0f : 0.25f * 365.0f / orbitalPeriodDays)
    , _rotationAngleSpeed(rotationAngleSpeed)
    , _orbitAngle(initialOrbitAngle)
    , _rotationAngle(0.0f)
{
}

void Planet::render() const
{
    const auto& spm = ShaderProgramManager::getInstance();
    const auto& tm = TextureManager::getInstance();
    auto& mainProgram = spm.getShaderProgram("main");

    tm.getTexture(_textureKey).bind();
    _material.setUniform(mainProgram, "material");

    mainProgram.setModelAndNormalMatrix(getModelMatrix());
    sphereMesh->render();
}

void Planet::renderPoints() const
{
    auto& normalsProgram = ShaderProgramManager::getInstance().getShaderProgram("normals");
    normalsProgram.setModelAndNormalMatrix(getModelMatrix());
    sphereMesh->renderPoints();
}

void Planet::update()
{
    const auto& window = OpenGLWindow::getDefaultWindow();
    _orbitAngle += window->sof(_travelAngleSpeed);
    _rotationAngle += window->sof(_rotationAngleSpeed);
}

void Planet::initializeGeometry(int slices, int stacks)
{
    sphereMesh = std::make_unique<static_meshes_3D::Sphere>(1.0f, slices, stacks);
}

void Planet::freeGeometry()
{
    sphereMesh.reset();
}

glm::mat4 Planet::getModelMatrix() const
{
    auto modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::rotate(modelMatrix, _orbitAngle, glm::vec3(0.0f, 1.0f, 0.0f));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, _distanceFromCenter));
    modelMatrix = glm::rotate(modelMatrix, _rotationAngle, glm::vec3(0.0f, 1.0f, 0.0f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(_radius, _radius, _radius));
    return modelMatrix;
}
