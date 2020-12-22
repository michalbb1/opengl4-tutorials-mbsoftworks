#pragma once

// Project
#include "../common_classes/transformFeedbackParticleSystem.h"
#include "../common_classes/flyingCamera.h"

#include "../common_classes/shader_structs/fogParameters.h"

class SnowParticleSystem : public TransformFeedbackParticleSystem
{
public:
    SnowParticleSystem(const int numMaxParticlesInBuffer, const FlyingCamera& flyingCamera, const shader_structs::FogParameters& fogParameters);

    void setParticlesColor(const glm::vec3& particlesColor);
    void setGeneratedPositionMinMax(const glm::vec3& generatedPositionMin, const glm::vec3& generatedPositionMax);
    void setGeneratedSizeMinMax(const float generatedSizeMin, const float generatedSizeMax);
    void setGeneratedAlphaMinMax(const float generatedAlphaMin, const float generatedAlphaMax);
    void setGeneratedAmount(int numParticlesToGenerate, float generateEverySeconds);

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

    glm::vec3 particlesColor_{ glm::vec3(0.83f, 0.435f, 0.0f) };

    float generatedSizeMin_{ 0.4f };
    float generatedSizeRange_{ 0.75f };

    float generatedAlphaMin_{ 0.65f };
    float generatedAlphaRange_{ 0.28f };

    int numParticlesToGenerate_{ 40 };
};