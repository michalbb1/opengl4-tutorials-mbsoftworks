// Project
#include "fireParticleSystem.h"

#include "../common_classes/shaderManager.h"
#include "../common_classes/shaderProgramManager.h"
#include "../common_classes/textureManager.h"
#include "../common_classes/uniformBufferObject.h"

FireParticleSystem::FireParticleSystem(const int numMaxParticlesInBuffer, const FlyingCamera& flyingCamera, const shader_structs::FogParameters& fogParameters)
    : TransformFeedbackParticleSystem(numMaxParticlesInBuffer)
    , flyingCamera_(flyingCamera)
    , fogParameters_(fogParameters)
{
}

void FireParticleSystem::setParticlesColor(const glm::vec3& particlesColor)
{
    particlesColor_ = particlesColor;
    pointLight_.color = particlesColor;
}

void FireParticleSystem::setGeneratedPositionMinMax(const glm::vec3& generatedPositionMin, const glm::vec3& generatedPositionMax)
{
    generatedPositionMin_ = generatedPositionMin;
    generatedPositionRange_ = generatedPositionMax - generatedPositionMin;
    pointLight_.position = getCenterOfGeneratedFire();
}

void FireParticleSystem::setGeneratedVelocityMinMax(const glm::vec3& generatedVelocityMin, const glm::vec3& generatedVelocityMax)
{
    generatedVelocityMin_ = generatedVelocityMin;
    generatedVelocityRange_ = generatedVelocityMax - generatedVelocityMin;
}

void FireParticleSystem::setGeneratedLifetimeMinMax(const float generatedLifetimeMin, const float generatedLifetimeMax)
{
    generatedLifetimeMin_ = generatedLifetimeMin;
    generatedLifetimeRange_ = generatedLifetimeMax - generatedLifetimeMin;
}

void FireParticleSystem::setGeneratedSizeMinMax(const float generatedSizeMin, const float generatedSizeMax)
{
    generatedSizeMin_ = generatedSizeMin;
    generatedSizeRange_ = generatedSizeMax - generatedSizeMin;
}

void FireParticleSystem::setGeneratedAmount(int numParticlesToGenerate, float generateEverySeconds)
{
    numParticlesToGenerate_ = numParticlesToGenerate;
    generateEverySeconds_ = generateEverySeconds;
}

const shader_structs::PointLight& FireParticleSystem::getPointLight() const
{
    return pointLight_;
}

glm::vec3 FireParticleSystem::getParticlesColor() const
{
    return particlesColor_;
}

bool FireParticleSystem::initializeShadersAndRecordedVariables()
{
    auto& sm = ShaderManager::getInstance();
    auto& spm = ShaderProgramManager::getInstance();
    auto& tm = TextureManager::getInstance();

    // Record velocity, lifetime and size
    addRecordedVec3("outVelocity", false);
    addRecordedFloat("outLifetime");
    addRecordedFloat("outSize");

    // Create shader program for updating particles
    sm.loadVertexShader("fire_particle_system_update", "data/shaders/tut025-particle-system-tf/fire_update.vert");
    sm.loadGeometryShader("fire_particle_system_update", "data/shaders/tut025-particle-system-tf/fire_update.geom");
    sm.tryLoadGeometryShader("random", "data/shaders/common/random.glsl");

    auto& updateProgram = spm.createShaderProgram("fire_particle_system_update");
    updateProgram.addShaderToProgram(sm.getVertexShader("fire_particle_system_update"));
    updateProgram.addShaderToProgram(sm.getGeometryShader("fire_particle_system_update"));
    updateProgram.addShaderToProgram(sm.getGeometryShader("random"));

    // Before linking the program, we have to tell OpenGL which output variables we want to record during transform feedback
    updateProgram.setTransformFeedbackRecordedVariables(getRecordedVariablesNames());
    if (!updateProgram.linkProgram()) {
        return false;
    }

    // Create shader program for rendering particles
    sm.loadVertexShader("fire_particle_system_render", "data/shaders/tut025-particle-system-tf/fire_render.vert");
    sm.loadGeometryShader("fire_particle_system_render", "data/shaders/tut025-particle-system-tf/fire_render.geom");
    sm.loadFragmentShader("fire_particle_system_render", "data/shaders/tut025-particle-system-tf/fire_render.frag");

    auto& renderProgram = ShaderProgramManager::getInstance().createShaderProgram("fire_particle_system_render");
    renderProgram.addShaderToProgram(sm.getVertexShader("fire_particle_system_render"));
    renderProgram.addShaderToProgram(sm.getGeometryShader("fire_particle_system_render"));
    renderProgram.addShaderToProgram(sm.getFragmentShader("fire_particle_system_render"));
    renderProgram.addShaderToProgram(sm.getFragmentShader(ShaderKeys::fog()));

    // Load texture for fire particles
    tm.loadTexture2D("fire_particle", "data/textures/particles/fire.bmp");

    if (!renderProgram.linkProgram()) {
        return false;
    }

    renderProgram.bindUniformBlockToBindingPoint("MatricesBlock", UniformBlockBindingPoints::MATRICES);
    return true;
}

void FireParticleSystem::prepareUpdateParticles(float deltaTime)
{
    // Get and use program for updating fire particles
    auto& updateProgram = ShaderProgramManager::getInstance().getShaderProgram("fire_particle_system_update");
    updateProgram.useProgram();

    // Update time passed since last frame
    updateProgram["deltaTime"] = deltaTime;

    // Set uniform variables for generated position boundaries
    updateProgram["generatedPositionMin"] = generatedPositionMin_;
    updateProgram["generatedPositionRange"] = generatedPositionRange_;

    // Set uniform variables for generated velocity boundaries
    updateProgram["generatedVelocityMin"] = generatedVelocityMin_;
    updateProgram["generatedVelocityRange"] = generatedVelocityRange_;

    // Set uniform variables for generated lifetime boundaries
    updateProgram["generatedLifetimeMin"] = generatedLifetimeMin_;
    updateProgram["generatedLifetimeRange"] = generatedLifetimeRange_;

    // Set uniform variables for generated size boundaries
    updateProgram["generatedSizeMin"] = generatedSizeMin_;
    updateProgram["generatedSizeRange"] = generatedSizeRange_;

    // Update remaining time to generate particles and then decide how many particles will be generated
    remainingTimeToGenerateSeconds_ -= deltaTime;
    if (remainingTimeToGenerateSeconds_ <= 0.0f)
    {
        updateProgram["numParticlesToGenerate"] = numParticlesToGenerate_;
        updateProgram["initialRandomGeneratorSeed"] = generateRandomNumberGeneratorSeed();
        remainingTimeToGenerateSeconds_ += generateEverySeconds_;
    }
    else {
        updateProgram["numParticlesToGenerate"] = 0;
    }
}

void FireParticleSystem::prepareRenderParticles()
{
    const auto& tm = TextureManager::getInstance();

    // Get and use program for rendering snow particles
    auto& spRenderParticles = ShaderProgramManager::getInstance().getShaderProgram("fire_particle_system_render");
    spRenderParticles.useProgram();

    // Calculate and set billboarding vectors from our flying camera
    calculateBillboardingVectors(flyingCamera_.getNormalizedViewVector(), flyingCamera_.getUpVector());
    spRenderParticles["billboardHorizontalVector"] = billboardHorizontalVector_;
    spRenderParticles["billboardVerticalVector"] = billboardVerticalVector_;

    // Set sampler, particles color and fog uniforms
    spRenderParticles["sampler"] = 0;
    spRenderParticles["particlesColor"] = particlesColor_;
    fogParameters_.setUniform(spRenderParticles, ShaderConstants::fogParams());

    // Bind fire particle texture
    tm.getTexture("fire_particle").bind();
}

glm::vec3 FireParticleSystem::getCenterOfGeneratedFire() const
{
    auto result = generatedPositionMin_ + generatedPositionRange_ * 0.5f;
    result.y += 3.0f;
    return result;
}
