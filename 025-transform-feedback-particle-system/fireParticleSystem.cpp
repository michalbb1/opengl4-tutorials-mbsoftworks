// Project
#include "fireParticleSystem.h"

#include "../common_classes/matrixManager.h"
#include "../common_classes/shaderManager.h"
#include "../common_classes/shaderProgramManager.h"
#include "../common_classes/textureManager.h"

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

void FireParticleSystem::setGeneratedLifeTimeMinMax(const float generatedLifeTimeMin, const float generatedLifeTimeMax)
{
    generatedLifeTimeMin_ = generatedLifeTimeMin;
    generatedLifeTimeRange_ = generatedLifeTimeMax - generatedLifeTimeMin;
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

    // Record velocity, life time and size
    addRecordedVec3("outVelocity", false);
    addRecordedFloat("outLifeTime");
    addRecordedFloat("outSize");

    // Create shader program for updating particles
    sm.loadVertexShader("fire_particle_system_update", "data/shaders/tut-025-particle-system-tf/fire_update.vert");
    sm.loadGeometryShader("fire_particle_system_update", "data/shaders/tut-025-particle-system-tf/fire_update.geom");

    auto& spUpdateParticles = spm.createShaderProgram("fire_particle_system_update");
    spUpdateParticles.addShaderToProgram(sm.getVertexShader("fire_particle_system_update"));
    spUpdateParticles.addShaderToProgram(sm.getGeometryShader("fire_particle_system_update"));

    // Before linking the program, we have to tell OpenGL which output variables we want to record during transform feedback
    spUpdateParticles.setTransformFeedbackRecordedVariables(getRecordedVariablesNames());
    if (!spUpdateParticles.linkProgram()) {
        return false;
    }

    // Create shader program for rendering particles
    sm.loadVertexShader("fire_particle_system_render", "data/shaders/tut-025-particle-system-tf/fire_render.vert");
    sm.loadGeometryShader("fire_particle_system_render", "data/shaders/tut-025-particle-system-tf/fire_render.geom");
    sm.loadFragmentShader("fire_particle_system_render", "data/shaders/tut-025-particle-system-tf/fire_render.frag");

    auto& spRenderParticles = ShaderProgramManager::getInstance().createShaderProgram("fire_particle_system_render");
    spRenderParticles.addShaderToProgram(sm.getVertexShader("fire_particle_system_render"));
    spRenderParticles.addShaderToProgram(sm.getGeometryShader("fire_particle_system_render"));
    spRenderParticles.addShaderToProgram(sm.getFragmentShader("fire_particle_system_render"));
    spRenderParticles.addShaderToProgram(sm.getFragmentShader(ShaderKeys::fog()));

    // Load texture for fire particles
    tm.loadTexture2D("fire_particle", "data/textures/particles/fire.bmp");

    return spRenderParticles.linkProgram();
}

void FireParticleSystem::prepareUpdateParticles(float deltaTime)
{
    // Get and use program for updating fire particles
    auto& spUpdateParticles = ShaderProgramManager::getInstance().getShaderProgram("fire_particle_system_update");
    spUpdateParticles.useProgram();

    // Update time passed since last frame
    spUpdateParticles["deltaTime"] = deltaTime;

    // Set uniform variables for generated position boundaries
    spUpdateParticles["generatedPositionMin"] = generatedPositionMin_;
    spUpdateParticles["generatedPositionRange"] = generatedPositionRange_;

    // Set uniform variables for generated velocity boundaries
    spUpdateParticles["generatedVelocityMin"] = generatedVelocityMin_;
    spUpdateParticles["generatedVelocityRange"] = generatedVelocityRange_;

    // Set uniform variables for generated life time boundaries
    spUpdateParticles["generatedLifeTimeMin"] = generatedLifeTimeMin_;
    spUpdateParticles["generatedLifeTimeRange"] = generatedLifeTimeRange_;

    // Set uniform variables for generated size boundaries
    spUpdateParticles["generatedSizeMin"] = generatedSizeMin_;
    spUpdateParticles["generatedSizeRange"] = generatedSizeRange_;

    // Update remaining time to generate particles and then decide how many particles will be generated
    remainingTimeToGenerateSeconds_ -= deltaTime;
    if (remainingTimeToGenerateSeconds_ <= 0.0f)
    {
        spUpdateParticles["numParticlesToGenerate"] = numParticlesToGenerate_;
        spUpdateParticles["randomGeneratorSeed"] = generateRandomNumberGeneratorSeed();
        remainingTimeToGenerateSeconds_ += generateEverySeconds_;
    }
    else {
        spUpdateParticles["numParticlesToGenerate"] = 0;
    }
}

void FireParticleSystem::prepareRenderParticles()
{
    const auto& tm = TextureManager::getInstance();
    const auto& mm = MatrixManager::getInstance();

    // Get and use program for rendering snow particles
    auto& spRenderParticles = ShaderProgramManager::getInstance().getShaderProgram("fire_particle_system_render");
    spRenderParticles.useProgram();

    spRenderParticles["matrices.mProj"] = mm.getProjectionMatrix();
    spRenderParticles["matrices.mView"] = mm.getViewMatrix();

    // Calculate and set billboarding vectors from our flying camera
    calculateBillboardingVectors(flyingCamera_.getNormalizedViewVector(), flyingCamera_.getUpVector());
    spRenderParticles["billboardHorizontalVector"] = billboardHorizontalVector_;
    spRenderParticles["billboardVerticalVector"] = billboardVerticalVector_;

    // Set sampler, particles color and fog uniforms
    spRenderParticles["gSampler"] = 0;
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
