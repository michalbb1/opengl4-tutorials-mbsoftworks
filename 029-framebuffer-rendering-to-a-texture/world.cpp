// GLM
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

// Project
#include "world.h"

#include "../common_classes/gldebug.h"
#include "../common_classes/matrixManager.h"
#include "../common_classes/static_meshes_3D/primitives/torus.h"
#include "../common_classes/textureManager.h"
#include "../common_classes/random.h"
#include "../common_classes/samplerManager.h"
#include "../common_classes/shader_structs/ambientLight.h"
#include "../common_classes/shader_structs/diffuseLight.h"

World& World::getInstance()
{
    static World world;
    return world;
}

void World::initialize()
{
    plainGround_ = std::make_unique<static_meshes_3D::PlainGround>(true, true, true);
    cube_ = std::make_shared<static_meshes_3D::Cube>();

    meshes_.push_back(std::make_shared<static_meshes_3D::Torus>(20, 20, 4.0f, 2.0f, true, true, true));
    meshes_.push_back(std::make_shared<static_meshes_3D::Cylinder>(4.0f, 20, 8.0f, true, true, true));
    meshes_.push_back(std::make_shared<static_meshes_3D::Sphere>(8.0f, 16, 16, true, true, true));
    
    auto z = 50.0f;
    auto size = 20.0f;
    for(auto i = 0; i < 4; i++)
    {
        for (auto j = 0; j < 2; j++)
        {
            ScreenWithMesh screen;
            screen.frameBuffer = FrameBuffer::Builder()
                .createAndBind(currentTextureSize_, currentTextureSize_)
                .withDepthAttachment(GL_DEPTH_COMPONENT24)
                .withTextureColorAttachment()
                .finishAndGetUnique();
           
            screen.position = glm::vec3(-40 + j*80, size / 2.0f + 0.01f, z);
            screen.scale = glm::vec3(2.0f, size, size);
            screen.textureFaceBitmask = j ? static_meshes_3D::CUBE_LEFT_FACE : static_meshes_3D::CUBE_RIGHT_FACE;
            screens_.push_back(std::move(screen));
        }

        z -= size + 10.0f;
        size += 10.0f;
    }

    ScreenWithMesh screen;
    screen.frameBuffer = FrameBuffer::Builder()
        .createAndBind(currentTextureSize_, currentTextureSize_)
        .withDepthAttachment(GL_DEPTH_COMPONENT24)
        .withTextureColorAttachment()
        .finishAndGetUnique();

    screen.position = glm::vec3(0, size / 2.0f + 0.01f, z);
    screen.scale = glm::vec3(size, size, 2.0f);
    screen.textureFaceBitmask = static_meshes_3D::CUBE_FRONT_FACE;
    screens_.push_back(std::move(screen));

    reshuffleScreenMeshes();
    reshuffleScreenTextures();
}

void World::release()
{
    screens_.clear();
    plainGround_.reset();
    cube_.reset();
    meshes_.clear();
}

void World::renderGround(ShaderProgram& shaderProgram) const
{
    TextureManager::getInstance().getTexture("wood").bind(0);
    shaderProgram.setModelAndNormalMatrix(glm::mat4(1.0f));
    plainGround_->render();
}

void World::renderScreensWithPrimitives(ShaderProgram& shaderProgram) const
{
    const auto& tm = TextureManager::getInstance();
    const auto& mm = MatrixManager::getInstance();

    const shader_structs::AmbientLight ambientLight(glm::vec3(1.0f, 1.0f, 1.0f));
    const auto meshModelMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(meshesRotationAngle_), glm::vec3(0.0f, 1.0f, 0.0f));
    const auto projectionMatrix = glm::perspective(45.0f, static_cast<float>(1024) / static_cast<float>(1024), 0.5f, 150.0f);

    shaderProgram[ShaderConstants::projectionMatrix()] = projectionMatrix;
    ambientLight.setUniform(shaderProgram, ShaderConstants::ambientLight());
    shader_structs::DiffuseLight::none().setUniform(shaderProgram, ShaderConstants::diffuseLight());
    for (const auto& screen : screens_)
    {
        screen.frameBuffer->bindAsDraw();
        screen.frameBuffer->setFullViewport();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        tm.getTexture(screen.meshTextureKey).bind();

        shaderProgram.setModelAndNormalMatrix(meshModelMatrix);
        shaderProgram[ShaderConstants::viewMatrix()] = glm::lookAt(glm::vec3(0, 0, -20), glm::vec3(), glm::vec3(0, 1, 0));

        tm.getTexture(screen.meshTextureKey).bind();
        screen.mesh->render();
    }

    SamplerManager::getInstance().getSampler("framebuffer").bind();
    FrameBuffer::Default::bindAsBothReadAndDraw();
    FrameBuffer::Default::setFullViewport();
    shaderProgram[ShaderConstants::projectionMatrix()] = mm.getProjectionMatrix();
    shaderProgram[ShaderConstants::viewMatrix()] = mm.getViewMatrix();
    
    for (const auto& screen : screens_)
    {
        auto modelMatrix = glm::translate(glm::mat4(1.0f), screen.position);
        modelMatrix = glm::scale(modelMatrix, screen.scale);
        shaderProgram.setModelAndNormalMatrix(modelMatrix);

        tm.getTexture("wood").bind();
        cube_->renderFaces(static_meshes_3D::CUBE_ALLFACES & ~screen.textureFaceBitmask);

        screen.frameBuffer->getTexture()->bind();
        cube_->renderFaces(screen.textureFaceBitmask);
    }
}

void World::nextTextureSize()
{
    currentTextureSize_ >>= 1;
    if (currentTextureSize_ > MAX_TEXTURE_SIZE)
    {
        currentTextureSize_ = 1;
    }

    for (auto& screen : screens_)
    {
        screen.frameBuffer->resize(currentTextureSize_, currentTextureSize_);
    }
}

void World::previousTextureSize()
{
    currentTextureSize_ <<= 1;
    if (currentTextureSize_ == 0)
    {
        currentTextureSize_ = MAX_TEXTURE_SIZE;
    }

    for(auto& screen : screens_)
    {
        screen.frameBuffer->resize(currentTextureSize_, currentTextureSize_);
    }
}

GLsizei World::getCurrentTextureSize() const
{
    return currentTextureSize_;
}

void World::reshuffleScreenMeshes()
{
    for (auto& screen : screens_)
    {
        const auto randomMeshIndex = Random::nextInt(meshes_.size());
        screen.mesh = meshes_.at(randomMeshIndex);
    }
}

void World::reshuffleScreenTextures()
{
    std::vector<std::string> availableTextureKeys = { "prismarine_dark", "diamond", "white_marble", "scifi_metal", "wood", "grass"};

    for (auto& screen : screens_)
    {
        const auto randomTextureIndex = Random::nextInt(availableTextureKeys.size());
        screen.meshTextureKey = availableTextureKeys.at(randomTextureIndex);
    }
}

void World::updateWorld(float deltaTime)
{
    meshesRotationAngle_ += deltaTime * 60.0f;
}
