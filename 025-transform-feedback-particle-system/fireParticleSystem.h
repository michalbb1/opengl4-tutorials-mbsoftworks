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

    void setParticlesColor(const glm::vec3& particlesColor);
    void setGeneratedPositionMinMax(const glm::vec3& generatedPositionMin, const glm::vec3& generatedPositionMax);
    void setGeneratedVelocityMinMax(const glm::vec3& generatedVelocityMin, const glm::vec3& generatedVelocityMax);
    void setGeneratedLifeTimeMinMax(const float generatedLifeTimeMin, const float generatedLifeTimeMax);
    void setGeneratedSizeMinMax(const float generatedSizeMin, const float generatedSizeMax);
    void setGeneratedAmount(int numParticlesToGenerate, float generateEverySeconds);

    const shader_structs::PointLight& getPointLight() const;
    glm::vec3 getParticlesColor() const;
    glm::vec3 getFireCenter() const;

protected:
    virtual bool initializeShadersAndVaryings() override;

    virtual void prepareUpdateParticles(float deltaTime) override;
    virtual void prepareRenderParticles() override;

private:
    const FlyingCamera& flyingCamera_;
    const shader_structs::FogParameters& fogParameters_;

    float remainingTimeToGenerateSeconds_{ 0.0f };
    float generateEverySeconds_{ 0.05f };

    glm::vec3 generatedPositionMin_{ glm::vec3(-5.0f, 0.0f, -5.0f) };
    glm::vec3 generatedPositionRange_{ glm::vec3(10.0f, 0.0f, 10.0f) };

    glm::vec3 generatedVelocityMin_{ glm::vec3(-3, 0, -3) };
    glm::vec3 generatedVelocityRange_{ glm::vec3(6, 10, 6) };

    glm::vec3 particlesColor_{ glm::vec3(0.83f, 0.435f, 0.0f) };

    float generatedLifeTimeMin_{ 1.5f };
    float generatedLifeTimeRange_{ 2.5f };
    float generatedSizeMin_{ 0.4f };
    float generatedSizeRange_{ 0.75f };

    int numParticlesToGenerate_{ 40 };

    shader_structs::PointLight pointLight_{getFireCenter(), particlesColor_, 0.0f, 0.3f, 0.007f, 0.00008f};
};