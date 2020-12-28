#pragma once

// Project
#include "../common_classes/transformFeedbackParticleSystem.h"
#include "../common_classes/flyingCamera.h"

#include "../common_classes/shader_structs/pointLight.h"
#include "../common_classes/shader_structs/fogParameters.h"

class FireParticleSystem : public TransformFeedbackParticleSystem
{
public:
    FireParticleSystem(const int numMaxParticlesInBuffer, const FlyingCamera& flyingCamera, const shader_structs::FogParameters& fogParameters);

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
     * Sets velocity boundaries of generated particles.
     * 
     * @param generatedVelocityMin  Minimal possible velocity of generated particles
     * @param generatedVelocityMax  Maximal possible velocity of generated particles
     */
    void setGeneratedVelocityMinMax(const glm::vec3& generatedVelocityMin, const glm::vec3& generatedVelocityMax);
    
    /**
     * Sets life time boundaries of generated particles.
     * 
     * @param generatedLifeTimeMin  Minimal possible life time of generated particles
     * @param generatedLifeTimeMax  Maximal possible life time of generated particles
     */    
    void setGeneratedLifeTimeMinMax(const float generatedLifeTimeMin, const float generatedLifeTimeMax);

    /**
     * Sets size boundaries of generated particles.
     * 
     * @param generatedSizeMin  Minimal possible size of generated particles
     * @param generatedSizeMax  Maximal possible size of generated particles
     */
    void setGeneratedSizeMinMax(const float generatedSizeMin, const float generatedSizeMax);

    /**
     * Sets amount of generated particles.
     * 
     * @param numParticlesToGenerate  How many particles should be generated in one batch
     * @param generateEverySeconds    How often should one batch be generated (in seconds)
     */
    void setGeneratedAmount(int numParticlesToGenerate, float generateEverySeconds);

    /**
     * Gets point light of the fire particle system. This point light
     * is used to illuminate fire surroundings.
     */
    const shader_structs::PointLight& getPointLight() const;

    /**
     * Gets generated particles color.
     */
    glm::vec3 getParticlesColor() const;

    /**
     * Gets center point of generated fire.
     */
    glm::vec3 getCenterOfGeneratedFire() const;

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

    glm::vec3 generatedVelocityMin_{ glm::vec3(-3, 0, -3) }; // Minimal velocity of generated particles
    glm::vec3 generatedVelocityRange_{ glm::vec3(6, 10, 6) }; // Maximal velocity of generated particles

    glm::vec3 particlesColor_{ glm::vec3(0.83f, 0.435f, 0.0f) }; // Color of generated particles

    float generatedLifeTimeMin_{ 1.5f }; // Minimal life time of generated particles
    float generatedLifeTimeRange_{ 2.5f }; // Maximal life time of generated particles
    float generatedSizeMin_{ 0.4f }; // Minimal size of generated particles
    float generatedSizeRange_{ 0.75f }; // Maximal size of generated particles

    

    shader_structs::PointLight pointLight_{getCenterOfGeneratedFire(), particlesColor_, 0.0f, 0.3f, 0.007f, 0.00008f}; // Point light of fire
};