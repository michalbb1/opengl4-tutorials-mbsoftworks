// Project
#include "snowParticleSystem.h"

#include "../common_classes/matrixManager.h"
#include "../common_classes/shaderManager.h"
#include "../common_classes/shaderProgramManager.h"
#include "../common_classes/textureManager.h"

SnowParticleSystem::SnowParticleSystem(const int numMaxParticlesInBuffer, const FlyingCamera& flyingCamera, const shader_structs::FogParameters& fogParameters)
    : TransformFeedbackParticleSystem(numMaxParticlesInBuffer)
    , flyingCamera_(flyingCamera)
    , fogParameters_(fogParameters)
{
}

void SnowParticleSystem::setParticlesColor(const glm::vec3& particlesColor)
{
    particlesColor_ = particlesColor;
}

void SnowParticleSystem::setGeneratedPositionMinMax(const glm::vec3& generatedPositionMin, const glm::vec3& generatedPositionMax)
{
    generatedPositionMin_ = generatedPositionMin;
    generatedPositionRange_ = generatedPositionMax - generatedPositionMin;
}

void SnowParticleSystem::setGeneratedSizeMinMax(const float generatedSizeMin, const float generatedSizeMax)
{
    generatedSizeMin_ = generatedSizeMin;
    generatedSizeRange_ = generatedSizeMax - generatedSizeMin;
}

void SnowParticleSystem::setGeneratedAlphaMinMax(const float generatedAlphaMin, const float generatedAlphaMax)
{
    generatedAlphaMin_ = generatedAlphaMin;
    generatedAlphaRange_ = generatedAlphaMax - generatedAlphaMin;
}

void SnowParticleSystem::setGeneratedAmount(int numParticlesToGenerate, float generateEverySeconds)
{
    numParticlesToGenerate_ = numParticlesToGenerate;
    generateEverySeconds_ = generateEverySeconds;
}

bool SnowParticleSystem::initializeShadersAndRecordedVariables()
{
    auto& sm = ShaderManager::getInstance();
    auto& spm = ShaderProgramManager::getInstance();
    auto& tm = TextureManager::getInstance();

    // Record size, alpha and index of the snowflake texture
    addRecordedFloat("outSize");
    addRecordedFloat("outAlpha");
    addRecordedInt("outSnowflakeIndex");

    // Create shader program for updating particles
    sm.loadVertexShader("snow_particle_system_update", "data/shaders/tut-025-particle-system-tf/snow_update.vert");
    sm.loadGeometryShader("snow_particle_system_update", "data/shaders/tut-025-particle-system-tf/snow_update.geom");

    auto& spUpdateParticles = spm.createShaderProgram("snow_particle_system_update");
    spUpdateParticles.addShaderToProgram(sm.getVertexShader("snow_particle_system_update"));
    spUpdateParticles.addShaderToProgram(sm.getGeometryShader("snow_particle_system_update"));

    // Before linking the program, we have to tell OpenGL which output variables we want to record during transform feedback
    spUpdateParticles.setTransformFeedbackRecordedVariables(getRecordedVariablesNames());
    if (!spUpdateParticles.linkProgram()) {
        return false;
    }

    // Create shader program for rendering particles
    sm.loadVertexShader("snow_particle_system_render", "data/shaders/tut-025-particle-system-tf/snow_render.vert");
    sm.loadGeometryShader("snow_particle_system_render", "data/shaders/tut-025-particle-system-tf/snow_render.geom");
    sm.loadFragmentShader("snow_particle_system_render", "data/shaders/tut-025-particle-system-tf/snow_render.frag");

    auto& spRenderParticles = ShaderProgramManager::getInstance().createShaderProgram("snow_particle_system_render");
    spRenderParticles.addShaderToProgram(sm.getVertexShader("snow_particle_system_render"));
    spRenderParticles.addShaderToProgram(sm.getGeometryShader("snow_particle_system_render"));
    spRenderParticles.addShaderToProgram(sm.getFragmentShader("snow_particle_system_render"));
    spRenderParticles.addShaderToProgram(sm.getFragmentShader(ShaderKeys::fog()));

    // Load texture for snowflake particles
    tm.loadTexture2D("snowflake_particles", "data/textures/particles/snowflakes.bmp");

    return spRenderParticles.linkProgram();
}

void SnowParticleSystem::prepareUpdateParticles(float deltaTime)
{
    // Get and use program for updating snow particles
    auto& spUpdateParticles = ShaderProgramManager::getInstance().getShaderProgram("snow_particle_system_update");
    spUpdateParticles.useProgram();

    // Update time passed since last frame
    spUpdateParticles["deltaTime"] = deltaTime;

    // Set uniform variables for generated position boundaries
    spUpdateParticles["generatedPositionMin"] = generatedPositionMin_;
    spUpdateParticles["generatedPositionRange"] = generatedPositionRange_;

    // Set uniform variables for generated size boundaries
    spUpdateParticles["generatedSizeMin"] = generatedSizeMin_;
    spUpdateParticles["generatedSizeRange"] = generatedSizeRange_;

    // Set uniform variables for generated alpha boundaries
    spUpdateParticles["generatedAlphaMin"] = generatedAlphaMin_;
    spUpdateParticles["generatedAlphaRange"] = generatedAlphaRange_;

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

void SnowParticleSystem::prepareRenderParticles()
{
    const auto& tm = TextureManager::getInstance();
    const auto& mm = MatrixManager::getInstance();

    // Get and use program for rendering snow particles
    auto& spRenderParticles = ShaderProgramManager::getInstance().getShaderProgram("snow_particle_system_render");
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

    // Bind snowflakes texture
    tm.getTexture("snowflake_particles").bind();
}
