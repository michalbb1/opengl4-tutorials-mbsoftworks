// GLM
#include <glm/gtc/matrix_transform.hpp>

// Project
#include "objectPicker.h"
#include "world.h"

#include "../common_classes/frameBuffer.h"
#include "../common_classes/matrixManager.h"
#include "../common_classes/shaderProgramManager.h"
#include "../common_classes/textureManager.h"
#include "../common_classes/static_meshes_3D/primitives/cylinder.h"
#include "../common_classes/static_meshes_3D/primitives/sphere.h"
#include "../common_classes/static_meshes_3D/primitives/torus.h"

ObjectPicker& ObjectPicker::getInstance()
{
    static ObjectPicker manager;
    return manager;
}

void ObjectPicker::initialize()
{
    // Create framebuffer for picking objects
    pickingFrameBuffer_.createFrameBufferWithColorAndDepthWithDefaultScreenSize();

    // Create occluder cube - we only need vertices positions here, no texture coordinates or normals
    occluderCube_ = std::make_unique<static_meshes_3D::Cube>(true, false, false);

    // Add cylinder mesh
    meshes_.push_back(std::make_unique<static_meshes_3D::Cylinder>(4.0f, 20, 10.0f, true, true, true));
    const glm::vec3 cylinderOcclusionBoxSize{ 8.0f, 10.01f, 8.0f };

    // Add sphere mesh
    meshes_.push_back(std::make_unique<static_meshes_3D::Sphere>(4.0f, 20, 20));
    const glm::vec3 sphereOcclusionBoxSize{ 8.0f, 8.0f, 8.0f };

    // Add torus mesh
    meshes_.push_back(std::make_unique<static_meshes_3D::Torus>(20, 20, 4.0f, 2.0f, true, true, true));
    const glm::vec3 torusOcclusionBoxSize{ 12.0f, 12.0f, 4.0f };

    // Create several pickable objects on the scene
    objects_.push_back(PickableObject{ glm::vec3(-75.0f, 6.0f, 120.0f), meshes_[0].get(), cylinderOcclusionBoxSize });
    objects_.push_back(PickableObject{ glm::vec3(-75.0f, 6.0f, -75.0f), meshes_[1].get(), sphereOcclusionBoxSize });
    objects_.push_back(PickableObject{ glm::vec3(75.0f, 6.0f, -75.0f), meshes_[2].get(), torusOcclusionBoxSize });
    objects_.push_back(PickableObject{ glm::vec3(75.0f, 6.0f, 55.0f), meshes_[2].get(), torusOcclusionBoxSize });
    objects_.push_back(PickableObject{ glm::vec3(-35.0f, 6.0f, 55.0f), meshes_[1].get(), sphereOcclusionBoxSize });
    objects_.push_back(PickableObject{ glm::vec3(-35.0f, 6.0f, -30.0f), meshes_[0].get(), cylinderOcclusionBoxSize });
    objects_.push_back(PickableObject{ glm::vec3(30.0f, 6.0f, -30.0f), meshes_[1].get(), sphereOcclusionBoxSize });
    objects_.push_back(PickableObject{ glm::vec3(30.0f, 6.0f, 10.0f), meshes_[0].get(), cylinderOcclusionBoxSize });
    objects_.push_back(PickableObject{ glm::vec3(10.0f, 6.0f, 10.0f), meshes_[2].get(), torusOcclusionBoxSize });
}

void ObjectPicker::release()
{
    occluderCube_.reset();
    meshes_.clear();
}

void ObjectPicker::renderAllPickableObjects()
{
    const auto& mm = MatrixManager::getInstance();
    auto& singleColorShaderProgram = ShaderProgramManager::getInstance().getShaderProgram("single-color");
    singleColorShaderProgram.useProgram();
    singleColorShaderProgram[ShaderConstants::projectionMatrix()] = mm.getProjectionMatrix();
    singleColorShaderProgram[ShaderConstants::viewMatrix()] = mm.getViewMatrix();
    singleColorShaderProgram[ShaderConstants::color()] = glm::vec4(1.0f, 0.0f, 0.0f, 0.4f);

    const auto& tm = TextureManager::getInstance();
    auto& mainProgram = ShaderProgramManager::getInstance().getShaderProgram("main");
    mainProgram.useProgram();

    for(auto i = 0; i < static_cast<int>(objects_.size()); i++)
    {
        const auto& object = objects_.at(i);

        auto modelMatrix = glm::translate(glm::mat4(1.0f), object.position);
        modelMatrix = glm::rotate(modelMatrix, object.rotationAngleRad, glm::vec3(0.0f, 1.0f, 0.0f));
        modelMatrix = glm::scale(modelMatrix, object.getRenderScale());
        mainProgram.setModelAndNormalMatrix(modelMatrix);

        const auto color = selectedIndex_ == i ? glm::vec4(0.0f, 0.5f, 1.0f, 1.0f) : glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        mainProgram[ShaderConstants::color()] = color;
        shinyMaterial_.setUniform(mainProgram, ShaderConstants::material());

        if (dynamic_cast<static_meshes_3D::Cylinder*>(object.meshPtr))
        {
            tm.getTexture("diamond").bind();
        }
        else if (dynamic_cast<static_meshes_3D::Sphere*>(object.meshPtr))
        {
            tm.getTexture("white_marble").bind();
        }
        else if (dynamic_cast<static_meshes_3D::Torus*>(object.meshPtr))
        {
            tm.getTexture("scifi_metal").bind();
        }

        object.meshPtr->render();
    }
}

void ObjectPicker::updateAllPickableObjects(float deltaTime)
{
    for (auto i = 0; i < static_cast<int>(objects_.size()); i++)
    {
        auto& object = objects_.at(i);
        if(selectedIndex_ == i) {
            object.renderScaleAngleRad += deltaTime * 4.0f;
        }
        else
        {
            object.rotationAngleRad += deltaTime;
            object.renderScaleAngleRad = 0.0f;
        }
    }
}

void ObjectPicker::renderInColorMode()
{
    const auto& mm = MatrixManager::getInstance();

    auto& singleColorShaderProgram = ShaderProgramManager::getInstance().getShaderProgram("single-color");
    singleColorShaderProgram.useProgram();
    singleColorShaderProgram[ShaderConstants::projectionMatrix()] = mm.getProjectionMatrix();
    singleColorShaderProgram[ShaderConstants::viewMatrix()] = mm.getViewMatrix();

    singleColorShaderProgram[ShaderConstants::color()] = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
    World::getInstance().renderGround(singleColorShaderProgram);
    singleColorShaderProgram[ShaderConstants::color()] = glm::vec4(0.75f, 0.75f, 0.75f, 1.0f);
    World::getInstance().renderCratesMaze(singleColorShaderProgram);
    
    for (auto i = 0; i < static_cast<int>(objects_.size()); i++)
    {
        const auto& object = objects_.at(i);

        const auto r = i & 0xFF;
        const auto g = (i >> 8) & 0xFF;
        const auto b = (i >> 16) & 0xFF;
        const auto fr = static_cast<float>(r) / 255.0f;
        const auto fg = static_cast<float>(g) / 255.0f;
        const auto fb = static_cast<float>(b) / 255.0f;
        singleColorShaderProgram[ShaderConstants::color()] = glm::vec4(fr, fg, fb, 1.0f);
        
        auto modelMatrix = glm::translate(glm::mat4(1.0f), object.position);
        modelMatrix = glm::rotate(modelMatrix, object.rotationAngleRad, glm::vec3(0.0f, 1.0f, 0.0f));
        modelMatrix = glm::scale(modelMatrix, object.getRenderScale());
        if (!pixelPerfectSelectionMode_)
        {
            modelMatrix = glm::scale(modelMatrix, object.occlusionBoxSize);
            singleColorShaderProgram[ShaderConstants::modelMatrix()] = modelMatrix;
            occluderCube_->render();
        }
        else
        {
            singleColorShaderProgram[ShaderConstants::modelMatrix()] = modelMatrix;
            object.meshPtr->render();
        }
    }
}

bool ObjectPicker::isPixelPerfectSelectionMode() const
{
    return pixelPerfectSelectionMode_;
}

void ObjectPicker::togglePixelPerfectSelectionMode()
{
    pixelPerfectSelectionMode_ = !pixelPerfectSelectionMode_;
}

std::string ObjectPicker::getSelectedObjectDescription() const
{
    if(selectedIndex_ >= static_cast<int>(objects_.size())) {
        return "No object";
    }

    const auto& object = objects_.at(selectedIndex_);
    if (dynamic_cast<static_meshes_3D::Cylinder*>(object.meshPtr)) {
        return "Cylinder (ID #" + std::to_string(selectedIndex_) + ")";
    }
    else if (dynamic_cast<static_meshes_3D::Sphere*>(object.meshPtr))
    {
        return "Sphere (ID #" + std::to_string(selectedIndex_) + ")";
    }
    else if (dynamic_cast<static_meshes_3D::Torus*>(object.meshPtr))
    {
        return "Torus (ID #" + std::to_string(selectedIndex_) + ")";
    }

    return "Unknown";
}

void ObjectPicker::resizePickingFrameBuffer(int width, int height)
{
    pickingFrameBuffer_.resize(width, height);
    pickingFrameBuffer_.setFullViewport();
}

void ObjectPicker::performObjectPicking(int x, int y)
{
    pickingFrameBuffer_.bindAsBothReadAndDraw();
    pickingFrameBuffer_.setFullViewport();

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderInColorMode();

    const auto colorValues = FrameBuffer::readColorValue(x, y);
    const auto r = static_cast<int>(colorValues[0]);
    const auto g = static_cast<int>(colorValues[1]);
    const auto b = static_cast<int>(colorValues[2]);
    selectedIndex_ = r | (g << 8) | (b << 16);
}

void ObjectPicker::copyColorToDefaultFrameBuffer() const
{
    pickingFrameBuffer_.copyColorToDefaultFrameBuffer();
}
