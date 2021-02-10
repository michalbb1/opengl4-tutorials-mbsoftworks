// STL
#include <iostream>
#include <memory>
#include <deque>

// GLM
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Project
#include "025-transform-feedback-particle-system.h"
#include "HUD025.h"

#include "../common_classes/flyingCamera.h"

#include "../common_classes/freeTypeFont.h"
#include "../common_classes/shaderManager.h"
#include "../common_classes/shaderProgramManager.h"
#include "../common_classes/textureManager.h"
#include "../common_classes/samplerManager.h"
#include "../common_classes/freeTypeFontManager.h"
#include "../common_classes/matrixManager.h"
#include "../common_classes/uniformBufferObject.h"

#include "../common_classes/static_meshes_3D/assimpModel.h"
#include "../common_classes/static_meshes_3D/skybox.h"

#include "../common_classes/shader_structs/ambientLight.h"
#include "../common_classes/shader_structs/diffuseLight.h"
#include "../common_classes/shader_structs/pointLight.h"

#include "snowCoveredPlainGround.h"
#include "fireParticleSystem.h"
#include "snowParticleSystem.h"

FlyingCamera flyingCamera(glm::vec3(-160.0f, 15.0f, 150.0f), glm::vec3(-160.0f, 15.0f, 149.0f), glm::vec3(0.0f, 1.0f, 0.0f), 75.0f);

std::unique_ptr<static_meshes_3D::SnowCoveredPlainGround> snowCoveredPlainGround;
std::unique_ptr<static_meshes_3D::Skybox> skybox;
std::unique_ptr<static_meshes_3D::AssimpModel> barnModel;
std::unique_ptr<HUD025> hud;

shader_structs::AmbientLight ambientLight(glm::vec3(0.2f, 0.2f, 0.2f));
shader_structs::DiffuseLight diffuseLight(glm::vec3(1.0f, 1.0f, 1.0f), glm::normalize(glm::vec3(0.0f, -1.0f, -1.0f)), 0.2f);
shader_structs::FogParameters fogParameters(glm::vec3(0.7f, 0.7f, 0.7f), 20.0f, 75.0f, 0.002f, shader_structs::FogParameters::FOG_EQUATION_EXP, false);

struct Barn
{
    glm::vec3 position;
    float rotationAngle;
};

std::vector<Barn> barns
{
    Barn{glm::vec3(-120.0f, 0.0f, -110.0f), glm::radians(45.0f)},
    Barn{glm::vec3(55.0f, 0.0f, 170.0f), glm::radians(-90.0f)},
    Barn{glm::vec3(-65.0f, 0.0f, 50.0f), glm::radians(0.0f)},
    Barn{glm::vec3(140.0f, 0.0f, 50.0f), glm::radians(180.0f)},
    Barn{glm::vec3(40.0f, 0.0f, -150.0f), glm::radians(0.0f)}
};

bool isFireDragged = false;

std::unique_ptr<UniformBufferObject> uboMatrices; // UBO for matrices
std::unique_ptr<FireParticleSystem> fireParticleSystem;
std::unique_ptr<SnowParticleSystem> snowParticleSystem;

void OpenGLWindow025::initializeScene()
{
    try
    {
        // Create UBO for matrices and bind it to the MATRICES_BLOCK_BINDING_POINT
        uboMatrices = std::make_unique<UniformBufferObject>();
        uboMatrices->createUBO(sizeof(glm::mat4) * 2);
        uboMatrices->bindBufferBaseToBindingPoint(UniformBlockBindingPoints::MATRICES);

        auto& sm = ShaderManager::getInstance();
        auto& spm = ShaderProgramManager::getInstance();
        auto& tm = TextureManager::getInstance();

        sm.loadVertexShader("tut025_main", "data/shaders/tut025-particle-system-tf/shader.vert");
        sm.loadFragmentShader("tut025_main", "data/shaders/tut025-particle-system-tf/shader.frag");
        sm.loadFragmentShader(ShaderKeys::ambientLight(), "data/shaders/lighting/ambientLight.frag");
        sm.loadFragmentShader(ShaderKeys::diffuseLight(), "data/shaders/lighting/diffuseLight.frag");
        sm.loadFragmentShader(ShaderKeys::pointLight(), "data/shaders/lighting/pointLight.frag");
        sm.loadFragmentShader(ShaderKeys::fog(), "data/shaders/fog/fog.frag");

        auto& mainShaderProgram = spm.createShaderProgram("main");
        mainShaderProgram.addShaderToProgram(sm.getVertexShader("tut025_main"));
        mainShaderProgram.addShaderToProgram(sm.getFragmentShader("tut025_main"));
        mainShaderProgram.addShaderToProgram(sm.getFragmentShader(ShaderKeys::ambientLight()));
        mainShaderProgram.addShaderToProgram(sm.getFragmentShader(ShaderKeys::diffuseLight()));
        mainShaderProgram.addShaderToProgram(sm.getFragmentShader(ShaderKeys::pointLight()));
        mainShaderProgram.addShaderToProgram(sm.getFragmentShader(ShaderKeys::fog()));
         
        skybox = std::make_unique<static_meshes_3D::Skybox>("data/skyboxes/jajsnow1", "jpg");
        hud = std::make_unique<HUD025>(*this, ambientLight);
        
        SamplerManager::getInstance().createSampler("main", MAG_FILTER_BILINEAR, MIN_FILTER_TRILINEAR);

        // Load barn, but transform the model a bit, as it's strangely rotated and a bit too big
        auto barnMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        barnMatrix = glm::scale(barnMatrix, glm::vec3(0.05f, 0.05f, 0.05f));
        barnModel = std::make_unique<static_meshes_3D::AssimpModel>("data/models/scheune_3ds/scheune.3ds", "", true, true, true, barnMatrix);

        // Create and initialize fire particle system
        fireParticleSystem = std::make_unique<FireParticleSystem>(10000, flyingCamera, fogParameters);
        fireParticleSystem->initialize();

        fireParticleSystem->setGeneratedAmount(40, 0.01f);
        fireParticleSystem->setParticlesColor(glm::vec3(0.83f, 0.435f, 0.0f));
        fireParticleSystem->setGeneratedPositionMinMax(glm::vec3(50.0f, 0.0f, 16.0f), glm::vec3(54.0f, 0.0f, 20.0f));
        fireParticleSystem->setGeneratedVelocityMinMax(glm::vec3(-2.5f, 0.0f, -2.5f), glm::vec3(2.5f, 8.0f, 2.5f));
        fireParticleSystem->setGeneratedLifetimeMinMax(1.5f, 4.0f);
        fireParticleSystem->setGeneratedSizeMinMax(0.2f, 0.7f);

        // Create and initialize snow particle system
        snowParticleSystem = std::make_unique<SnowParticleSystem>(50000, flyingCamera, fogParameters);
        snowParticleSystem->initialize();

        snowParticleSystem->setGeneratedAmount(40, 0.005f);
        snowParticleSystem->setParticlesColor(glm::vec3(0.85f, 0.85f, 0.85f));
        snowParticleSystem->setGeneratedSizeMinMax(0.15f, 0.4f);
        snowParticleSystem->setGeneratedAlphaMinMax(0.5f, 0.8f);

        snowCoveredPlainGround = std::make_unique<static_meshes_3D::SnowCoveredPlainGround>(ambientLight, diffuseLight, fireParticleSystem->getPointLight(), fogParameters);
        spm.linkAllPrograms();

        // Bind uniform blocks with binding points for main program
        mainShaderProgram.bindUniformBlockToBindingPoint("MatricesBlock", UniformBlockBindingPoints::MATRICES);
    }
    catch (const std::runtime_error& ex)
    {
        std::cout << "Error occured during initialization: " << ex.what() << std::endl;
        closeWindow(true);
        return;
    }

    glEnable(GL_DEPTH_TEST);
    glClearDepth(1.0);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void OpenGLWindow025::renderScene()
{
    const auto& spm = ShaderProgramManager::getInstance();
    const auto& tm = TextureManager::getInstance();
    auto& mm = MatrixManager::getInstance();
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set matrices in matrix manager
    mm.setProjectionMatrix(getProjectionMatrix());
    mm.setOrthoProjectionMatrix(getOrthoProjectionMatrix());
    mm.setViewMatrix(flyingCamera.getViewMatrix());

    // Set matrices uniform buffer object data - we just set projection and view matrix here, they are
    // consistent across all shader programs
    uboMatrices->bindUBO();
    uboMatrices->setBufferData(0, glm::value_ptr(getProjectionMatrix()), sizeof(glm::mat4));
    uboMatrices->setBufferData(sizeof(glm::mat4), glm::value_ptr(flyingCamera.getViewMatrix()), sizeof(glm::mat4));

    // Set up some common properties in the main shader program
    auto& mainProgram = spm.getShaderProgram("main");
    mainProgram.useProgram();
    mainProgram.setModelAndNormalMatrix(glm::mat4(1.0f));
    mainProgram[ShaderConstants::color()] = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    mainProgram[ShaderConstants::sampler()] = 0;
    
    // Render skybox first with only ambient light and without point lights
    // Btw it's a cool effect if you leave point lights here, looks like the sky is illuminated :)
    ambientLight.setUniform(mainProgram, ShaderConstants::ambientLight());
    shader_structs::DiffuseLight::none().setUniform(mainProgram, ShaderConstants::diffuseLight());
    shader_structs::PointLight::none().setUniform(mainProgram, "firePointLight");
    fogParameters.setUniform(mainProgram, ShaderConstants::fogParams());
    skybox->render(flyingCamera.getEye(), mainProgram);

    // Set up material properties for specular highlights and restore point light effects
    SamplerManager::getInstance().getSampler("main").bind();
    diffuseLight.setUniform(mainProgram, ShaderConstants::diffuseLight());
    fireParticleSystem->getPointLight().setUniform(mainProgram, "firePointLight");

    // Render barns with no specular reflections
    for (const auto& barn : barns)
    {
        auto basicModelMatrix = glm::translate(glm::mat4(1.0f), barn.position);
        basicModelMatrix = glm::rotate(basicModelMatrix, barn.rotationAngle, glm::vec3(0.0f, 1.0f, 0.0f));
        mainProgram.setModelAndNormalMatrix(basicModelMatrix);
        barnModel->render();
    }

    // Render ground
    snowCoveredPlainGround->render();

    // Enable blending to render particles and disable writing to depth buffer
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glDepthMask(GL_FALSE);

    // Update generated position of fire particles when it's dragged
    if (isFireDragged)
    {
        const auto newFirePosition = flyingCamera.getEye() + flyingCamera.getNormalizedViewVector() * 20.0f;
        const auto newGeneratedPositionMin = glm::vec3(newFirePosition.x - 2.0f, newFirePosition.y, newFirePosition.z - 2.0f);
        const auto newGeneratedPositionMax = glm::vec3(newFirePosition.x + 2.0f, newFirePosition.y, newFirePosition.z + 2.0f);
        fireParticleSystem->setGeneratedPositionMinMax(newGeneratedPositionMin, newGeneratedPositionMax);
    }

    // Render particles
    fireParticleSystem->renderParticles();
    snowParticleSystem->renderParticles();

    // Re-enable writing to depth buffer and disable blending
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);

    // Render HUD
    hud->renderHUD(fireParticleSystem->getNumParticles(), snowParticleSystem->getNumParticles(), fogParameters.isEnabled, isFireDragged);
}

void OpenGLWindow025::updateScene()
{
    if (keyPressedOnce(GLFW_KEY_ESCAPE)) {
        closeWindow();
    }

    if (keyPressedOnce(GLFW_KEY_F3)) {
        setVerticalSynchronization(!isVerticalSynchronizationEnabled());
    }

    if (keyPressed(GLFW_KEY_KP_ADD))
    {
        ambientLight.color += sof(0.2f);
        if (ambientLight.color.r > 1.0f) {
            ambientLight.color = glm::vec3(1.0f);
        }
    }

    if (keyPressed(GLFW_KEY_KP_SUBTRACT))
    {
        ambientLight.color -= sof(0.2f);
        if (ambientLight.color.r < 0.0f) {
            ambientLight.color = glm::vec3(0.0f);
        }
    }

    if (keyPressedOnce(GLFW_KEY_F)) {
        fogParameters.isEnabled = !fogParameters.isEnabled;
    }

    if (keyPressedOnce(GLFW_KEY_SPACE)) {
        isFireDragged = !isFireDragged;
    }

    int posX, posY, width, height;
    glfwGetWindowPos(getWindow(), &posX, &posY);
    glfwGetWindowSize(getWindow(), &width, &height);
    flyingCamera.setWindowCenterPosition(glm::i32vec2(posX + width / 2, posY + height / 2));

    flyingCamera.update([this](int keyCode) {return this->keyPressed(keyCode); },
        [this]() {double curPosX, curPosY; glfwGetCursorPos(this->getWindow(), &curPosX, &curPosY); return glm::u32vec2(curPosX, curPosY); },
        [this](const glm::i32vec2& pos) {glfwSetCursorPos(this->getWindow(), pos.x, pos.y); },
        [this](float f) {return this->sof(f); });

    // Update fire particles
    fireParticleSystem->updateParticles(sof(1.0f));

    // Update and render snow, also set generated position of snow to always revolve around player
    snowParticleSystem->setGeneratedPositionMinMax(flyingCamera.getEye() + glm::vec3(-200.0f, 50.0f, -200.0f), flyingCamera.getEye() + glm::vec3(200.0f, 50.0f, 200.0f));
    snowParticleSystem->updateParticles(sof(1.0f));
}

void OpenGLWindow025::releaseScene()
{
    snowCoveredPlainGround.reset();
    skybox.reset();
    barnModel.reset();

    hud.reset();
    uboMatrices.reset();

    fireParticleSystem.reset();
    snowParticleSystem.reset();

    ShaderProgramManager::getInstance().clearShaderProgramCache();
    ShaderManager::getInstance().clearShaderCache();
    TextureManager::getInstance().clearTextureCache();
    SamplerManager::getInstance().clearSamplerCache();
    FreeTypeFontManager::getInstance().clearFreeTypeFontCache();
}
