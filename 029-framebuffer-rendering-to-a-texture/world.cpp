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

    // Prepare list of meshes that can be rendered in the gallery
    meshes_.push_back(std::make_shared<static_meshes_3D::Torus>(20, 20, 4.0f, 2.0f, true, true, true));
    meshes_.push_back(std::make_shared<static_meshes_3D::Cylinder>(4.0f, 20, 8.0f, true, true, true));
    meshes_.push_back(std::make_shared<static_meshes_3D::Sphere>(8.0f, 16, 16, true, true, true));

    // Now create gallery screens on the sideways while increasing size of the screens
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
                .withTextureColorAttachment(GL_RGB)
                .finishAndGetUnique();
           
            screen.position = glm::vec3(-40 + j*80, size / 2.0f + 0.01f, z);
            screen.scale = glm::vec3(2.0f, size, size);
            screen.textureFaceBitmask = j ? static_meshes_3D::CUBE_LEFT_FACE : static_meshes_3D::CUBE_RIGHT_FACE;
            screens_.push_back(std::move(screen));
        }

        z -= size + 10.0f;
        size += 10.0f;
    }

    // One final screen at the end follows
    ScreenWithMesh screen;
    screen.frameBuffer = FrameBuffer::Builder()
        .createAndBind(currentTextureSize_, currentTextureSize_)
        .withDepthAttachment(GL_DEPTH_COMPONENT24)
        .withTextureColorAttachment(GL_RGB)
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

    // Pre-calculate model matrix of rendered meshes and the projection matrix
    const shader_structs::AmbientLight ambientLight(glm::vec3(1.0f, 1.0f, 1.0f));
    const auto viewMatrix = glm::lookAt(glm::vec3(0, 0, -20), glm::vec3(), glm::vec3(0, 1, 0));
    const auto projectionMatrix = glm::perspective(glm::radians(57.0f), static_cast<float>(1024) / static_cast<float>(1024), 0.5f, 150.0f);
    const auto meshModelMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(meshesRotationAngle_), glm::vec3(0.0f, 1.0f, 0.0f));

    // Set the matrices and other uniforms of the shader program
    shaderProgram[ShaderConstants::viewMatrix()] = viewMatrix;
    shaderProgram[ShaderConstants::projectionMatrix()] = projectionMatrix;
    shaderProgram.setModelAndNormalMatrix(meshModelMatrix);
    ambientLight.setUniform(shaderProgram, ShaderConstants::ambientLight());
    shader_structs::DiffuseLight::none().setUniform(shaderProgram, ShaderConstants::diffuseLight());

    // Now let's render all the screens of the gallery - as first step, we have to render to the textures
    for(const auto& screen : screens_)
    {
        screen.frameBuffer->bindAsDraw();
        screen.frameBuffer->setFullViewport();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        tm.getTexture(screen.meshTextureKey).bind();
        screen.mesh->render();
    }

    // Now let's draw again to the screen as before and render all the screens with textures we have rendered to
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

        // Draw first the frame of the screen
        tm.getTexture("wood").bind();
        cube_->renderFaces(static_meshes_3D::CUBE_ALLFACES & ~screen.textureFaceBitmask);

        // Then draw the screen itself
        screen.frameBuffer->getTexture()->bind();
        cube_->renderFaces(screen.textureFaceBitmask);
    }
}

void World::decreaseTextureSize()
{
    currentTextureSize_ >>= 1;
    if (currentTextureSize_ == 0)
    {
        currentTextureSize_ = MAX_TEXTURE_SIZE;
    }

    for (auto& screen : screens_)
    {
        screen.frameBuffer->resize(currentTextureSize_, currentTextureSize_);
    }
}

void World::increaseTextureSize()
{
    currentTextureSize_ <<= 1;
    if (currentTextureSize_ > MAX_TEXTURE_SIZE)
    {
        currentTextureSize_ = 1;
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
