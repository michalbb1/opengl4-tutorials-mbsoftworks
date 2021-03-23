// GLM
#include <glm/gtc/matrix_transform.hpp>

// Project
#include "../common_classes/matrixManager.h"
#include "../common_classes/random.h"
#include "../common_classes/shaderProgramManager.h"
#include "../common_classes/static_meshes_3D/primitives/cylinder.h"
#include "../common_classes/static_meshes_3D/primitives/sphere.h"
#include "../common_classes/static_meshes_3D/primitives/torus.h"
#include "../common_classes/textureManager.h"

#include "objectsWithOccluderManager.h"

ObjectsWithOccludersManager& ObjectsWithOccludersManager::getInstance()
{
    static ObjectsWithOccludersManager manager;
    return manager;
}

void ObjectsWithOccludersManager::initialize()
{
    // Create occluder cube - we only need vertices positions here, no texture coordinates or normals
    occluderCube_ = std::make_unique<static_meshes_3D::Cube>(true, false, false);
    occlusionQuery_ = std::make_unique<OcclusionQuery>();

    // Add cylinder mesh
    meshes_.push_back(std::make_unique<static_meshes_3D::Cylinder>(2.0f, 20, 4.0f, true, true, true));
    occlusionBoxSizes_.emplace_back(4.01f, 4.01f, 4.01f);

    // Add sphere mesh
    meshes_.push_back(std::make_unique<static_meshes_3D::Sphere>(2.0f, 20, 20));
    occlusionBoxSizes_.emplace_back(4.01f, 4.01f, 4.01f);

    // Add torus mesh
    meshes_.push_back(std::make_unique<static_meshes_3D::Torus>(20, 20, 1.5f, 0.75f, true, true, true));
    occlusionBoxSizes_.emplace_back(4.51f, 4.51f, 2.26f);
}

void ObjectsWithOccludersManager::release()
{
    occluderCube_.reset();
    occlusionQuery_.reset();
    meshes_.clear();
}

void ObjectsWithOccludersManager::updateAndPerformOcclusionQuery(float deltaTime)
{
    if (timePassedSinceLastGeneration_ > GENERATE_OBJECT_EVERY_SECONDS)
    {
        timePassedSinceLastGeneration_ -= GENERATE_OBJECT_EVERY_SECONDS;
        const auto randomIndex = Random::nextInt(static_cast<int>(meshes_.size()));
        const ObjectWithOccluder object{ Random::getRandomVectorFromRectangleXZ(glm::vec3(-150.0f, -10.0f, -150.0f), glm::vec3(150.0f, -10.0f, 150.0f)), meshes_[randomIndex].get(), occlusionBoxSizes_[randomIndex], true };
        objects_.push_back(object);
    }

    timePassedSinceLastGeneration_ += deltaTime;

    const auto& mm = MatrixManager::getInstance();
    auto& singleColorShaderProgram = ShaderProgramManager::getInstance().getShaderProgram("single-color");
    singleColorShaderProgram.useProgram();
    singleColorShaderProgram[ShaderConstants::projectionMatrix()] = mm.getProjectionMatrix();
    singleColorShaderProgram[ShaderConstants::viewMatrix()] = mm.getViewMatrix();
    singleColorShaderProgram[ShaderConstants::color()] = glm::vec4(1.0f, 0.0f, 0.0f, 0.4f);

    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glDepthMask(GL_FALSE);

    size_t i = 0;
    numVisibleObjects_ = 0;
    while (i < objects_.size())
    {
        auto& object = objects_[i];
        if (object.position.y > 100.0f)
        {
            std::swap(objects_[i], objects_[objects_.size() - 1]);
            objects_.pop_back();
            continue;
        }

        object.position.y += 10.0f * deltaTime;
        auto modelMatrix = glm::translate(glm::mat4(1.0f), object.position);
        modelMatrix = glm::scale(modelMatrix, object.occlusionBoxSize);
        singleColorShaderProgram[ShaderConstants::modelMatrix()] = modelMatrix;
        occlusionQuery_->beginQuery();
        occluderCube_->render();
        occlusionQuery_->endQuery();
        object.isVisible = occlusionQuery_->anySamplesPassed();
        if (object.isVisible)
        {
            numVisibleObjects_++;
        }

        i++;
    }

    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glDepthMask(GL_TRUE);
}

void ObjectsWithOccludersManager::renderAllVisibleObjects()
{
    const auto& tm = TextureManager::getInstance();
    auto& mainProgram = ShaderProgramManager::getInstance().getShaderProgram("main");
    mainProgram.useProgram();

    for (auto& object : objects_)
    {
        if (!object.isVisible)
        {
            continue;
        }

        auto modelMatrix = glm::translate(glm::mat4(1.0f), object.position);
        mainProgram.setModelAndNormalMatrix(modelMatrix);
        if (dynamic_cast<static_meshes_3D::Cylinder*>(object.meshPtr))
        {
            dimMaterial_.setUniform(mainProgram, ShaderConstants::material());
            tm.getTexture("crate").bind();
        }
        else if (dynamic_cast<static_meshes_3D::Sphere*>(object.meshPtr))
        {
            shinyMaterial_.setUniform(mainProgram, ShaderConstants::material());
            tm.getTexture("white_marble").bind();
        }
        else if (dynamic_cast<static_meshes_3D::Torus*>(object.meshPtr))
        {
            shinyMaterial_.setUniform(mainProgram, ShaderConstants::material());
            tm.getTexture("scifi_metal").bind();
        }

        object.meshPtr->render();
    }
}

void ObjectsWithOccludersManager::renderOccluders()
{
    const auto& mm = MatrixManager::getInstance();

    glDepthMask(GL_FALSE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    auto& singleColorShaderProgram = ShaderProgramManager::getInstance().getShaderProgram("single-color");
    singleColorShaderProgram.useProgram();
    singleColorShaderProgram[ShaderConstants::projectionMatrix()] = mm.getProjectionMatrix();
    singleColorShaderProgram[ShaderConstants::viewMatrix()] = mm.getViewMatrix();
    singleColorShaderProgram[ShaderConstants::color()] = glm::vec4(1.0f, 0.0f, 0.0f, 0.4f);

    for (auto& object : objects_)
    {
        if (!object.isVisible)
        {
            continue;
        }

        auto modelMatrix = glm::translate(glm::mat4(1.0f), object.position);
        modelMatrix = glm::scale(modelMatrix, object.occlusionBoxSize);
        singleColorShaderProgram[ShaderConstants::modelMatrix()] = modelMatrix;
        occluderCube_->render();
    }

    glDisable(GL_BLEND);
    glDepthMask(GL_TRUE);
}

size_t ObjectsWithOccludersManager::getNumObjects() const
{
    return objects_.size();
}

size_t ObjectsWithOccludersManager::getNumVisibleObjects() const
{
    return numVisibleObjects_;
}
