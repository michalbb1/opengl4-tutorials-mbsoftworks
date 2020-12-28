#pragma once

// Project
#include "../common_classes/transformFeedbackParticleSystem.h"
#include "../common_classes/flyingCamera.h"

#include "../common_classes/shader_structs/fogParameters.h"

class SnowParticleSystem : public TransformFeedbackParticleSystem
{
public:
    SnowParticleSystem(const int numMaxParticlesInBuffer, const FlyingCamera& flyingCamera, const shader_structs::FogParameters& fogParameters);

    /**
     * Sets color of generated particles.
     *
     * @param particlesColor  New color of generated particles
     */
    void setParticlesColor(const glm::vec3& particlesColor);

    /**
     * Sets position boundaries of generated particles.
     *
     * @param generatedPositionMin  Minimal possible position of generated particles
     * @param generatedPositionMax  Maximal possible position of generated particles
     */
    void setGeneratedPositionMinMax(const glm::vec3& generatedPositionMin, const glm::vec3& generatedPositionMax);

    /**
     * Sets size boundaries of generated particles.
     *
     * @param generatedSizeMin  Minimal possible size of generated particles
     * @param generatedSizeMax  Maximal possible size of generated particles
     */
    void setGeneratedSizeMinMax(const float generatedSizeMin, const float generatedSizeMax);

    /**
     * Sets alpha boundaries of generated particles.
     *
     * @param generatedAlphaMin  Minimal possible alpha of generated particles
     * @param generatedAlphaMax  Maximal possible alpha of generated particles
     */
    void setGeneratedAlphaMinMax(const float generatedAlphaMin, const float generatedAlphaMax);

    /**
     * Sets amount of generated particles.
     *
     * @param numParticlesToGenerate  How many particles should be generated in one batch
     * @param generateEverySeconds    How often should one batch be generated (in seconds)
     */
    void setGeneratedAmount(int numParticlesToGenerate, float generateEverySeconds);

protected:
    virtual bool initializeShadersAndRecordedVariables() override;

    virtual void prepareUpdateParticles(float deltaTime) override;
    virtual void prepareRenderParticles() override;

private:
    const FlyingCamera& flyingCamera_; // Used camera in the scene
    const shader_structs::FogParameters& fogParameters_; // Fog parameters to be set for shader program

    float remainingTimeToGenerateSeconds_{ 0.0f }; // How many seconds remain until next generation batch
    float generateEverySeconds_{ 0.05f }; // How often is one batch of particles generated (in seconds)
    int numParticlesToGenerate_{ 40 }; // Number of generated particles in one batch

    glm::vec3 generatedPositionMin_{ glm::vec3(-5.0f, 0.0f, -5.0f) }; // Minimal position of generated particles
    glm::vec3 generatedPositionRange_{ glm::vec3(10.0f, 0.0f, 10.0f) }; // Maximal position of generated particles

    glm::vec3 particlesColor_{ glm::vec3(0.83f, 0.435f, 0.0f) }; // Color of generated particles

    float generatedSizeMin_{ 0.4f }; // Minimal size of generated particles
    float generatedSizeRange_{ 0.75f }; // Maximal size of generated particles

    float generatedAlphaMin_{ 0.65f }; // Minimal alpha of generated particles
    float generatedAlphaRange_{ 0.28f }; // Maximal alpha of generated particles
};