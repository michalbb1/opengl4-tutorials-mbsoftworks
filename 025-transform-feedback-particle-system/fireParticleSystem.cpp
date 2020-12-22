// Project
#include "fireParticleSystem.h"

#include "../common_classes/matrixManager.h"
#include "../common_classes/shaderManager.h"
#include "../common_classes/shaderProgramManager.h"
#include "../common_classes/textureManager.h"

bool FireParticleSystem::initializeShadersAndVaryings()
{
    auto& sm = ShaderManager::getInstance();
    auto& spm = ShaderProgramManager::getInstance();
    auto& tm = TextureManager::getInstance();

    addVaryingVec3("outVelocity", false);
    addVaryingFloat("outLifeTime");
    addVaryingFloat("outSize");

    // Create shader program for updating particles
    sm.loadVertexShader("fire_particle_system_update", "data/shaders/tut-025-particle-system-tf/fire_update.vert");
    sm.loadGeometryShader("fire_particle_system_update", "data/shaders/tut-025-particle-system-tf/fire_update.geom");

    auto& spUpdateParticles = spm.createShaderProgram("fire_particle_system_update");
    spUpdateParticles.addShaderToProgram(sm.getVertexShader("fire_particle_system_update"));
    spUpdateParticles.addShaderToProgram(sm.getGeometryShader("fire_particle_system_update"));

    // Before linking the program, we have to tell OpenGL which output variables we want to record during transform feedback
    spUpdateParticles.setTransformFeedbackVaryings(getVaryingNames());
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

    tm.loadTexture2D("fire_particle", "data/textures/particles/fire.bmp");

    return spRenderParticles.linkProgram();
}

void FireParticleSystem::prepareUpdateParticles(float deltaTime)
{
    auto& spUpdateParticles = ShaderProgramManager::getInstance().getShaderProgram("fire_particle_system_update");

    spUpdateParticles.useProgram();

    spUpdateParticles["deltaTime"] = deltaTime;

    spUpdateParticles["generatedPositionMin"] = generatedPositionMin_;
    spUpdateParticles["generatedPositionRange"] = generatedPositionRange_;

    spUpdateParticles["generatedVelocityMin"] = generatedVelocityMin_;
    spUpdateParticles["generatedVelocityRange"] = generatedVelocityRange_;

    spUpdateParticles["generatedLifeTimeMin"] = generatedLifeTimeMin_;
    spUpdateParticles["generatedLifeTimeRange"] = generatedLifeTimeRange_;

    spUpdateParticles["generatedSizeMin"] = generatedSizeMin_;
    spUpdateParticles["generatedSizeRange"] = generatedSizeRange_;

    spUpdateParticles["numParticlesToGenerate"] = 0;

    remainingTimeToGenerateSeconds_ -= deltaTime;

    if (remainingTimeToGenerateSeconds_ <= 0.0f)
    {
        spUpdateParticles["numParticlesToGenerate"] = numParticlesToGenerate_;
        remainingTimeToGenerateSeconds_ += generateEverySeconds_;

        spUpdateParticles["randomGeneratorSeed"] = generateRandomNumberGeneratorSeed();
    }
}

void FireParticleSystem::prepareRenderParticles()
{
    const auto& tm = TextureManager::getInstance();
    const auto& mm = MatrixManager::getInstance();
    auto& spRenderParticles = ShaderProgramManager::getInstance().getShaderProgram("fire_particle_system_render");

    spRenderParticles.useProgram();
    spRenderParticles["matrices.mProj"] = mm.getProjectionMatrix();
    spRenderParticles["matrices.mView"] = mm.getViewMatrix();

    const auto cameraViewVector = glm::normalize(flyingCamera_.getViewPoint() - flyingCamera_.getEye());
    calculateBillboardingVectors(cameraViewVector, flyingCamera_.getUpVector());
    spRenderParticles["billboardHorizontalVector"] = billboardHorizontalVector_;
    spRenderParticles["billboardVerticalVector"] = billboardVerticalVector_;
    spRenderParticles["gSampler"] = 0;
    spRenderParticles["particlesColor"] = particlesColor_;
    fogParameters_.setUniform(spRenderParticles, ShaderConstants::fogParams());

    tm.getTexture("fire_particle").bind();
}

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
    pointLight_.position = getFireCenter();
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

const shader_structs::PointLight & FireParticleSystem::getPointLight() const
{
    return pointLight_;
}

glm::vec3 FireParticleSystem::getParticlesColor() const
{
    return particlesColor_;
}

glm::vec3 FireParticleSystem::getFireCenter() const
{
    auto result = generatedPositionMin_ + generatedPositionRange_ * 0.5f;
    result.y += 3.0f;
    return result;
}
