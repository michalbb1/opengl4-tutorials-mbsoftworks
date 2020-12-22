// STL
#include <mutex>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Project
#include "snowCoveredPlainGround.h"

#include "../common_classes/shaderManager.h"
#include "../common_classes/shaderProgramManager.h"
#include "../common_classes/textureManager.h"
#include "../common_classes/samplerManager.h"
#include "../common_classes/matrixManager.h"

namespace static_meshes_3D {

SnowCoveredPlainGround::SnowCoveredPlainGround(const shader_structs::AmbientLight& ambientLight, const shader_structs::DiffuseLight& diffuseLight,
    const shader_structs::PointLight& firePointLight, const shader_structs::FogParameters& fogParameters,
    bool withPositions, bool withTextureCoordinates, bool withNormals)
    : PlainGround(withPositions, withTextureCoordinates, withNormals)
    , ambientLight_(ambientLight)
    , diffuseLight_(diffuseLight)
    , firePointLight_(firePointLight)
    , fogParameters_(fogParameters)
{
    static std::once_flag prepareOnceFlag;
    std::call_once(prepareOnceFlag, []()
    {
        auto& sm = ShaderManager::getInstance();
        sm.loadVertexShader("tut025_ground", "data/shaders/tut-025-particle-system-tf/ground_shader.vert");
        sm.loadFragmentShader("tut025_ground", "data/shaders/tut-025-particle-system-tf/ground_shader.frag");

        auto& groundProgram = ShaderProgramManager::getInstance().createShaderProgram("tut025_ground");
        groundProgram.addShaderToProgram(sm.getVertexShader("tut025_ground"));
        groundProgram.addShaderToProgram(sm.getFragmentShader("tut025_ground"));
        groundProgram.addShaderToProgram(sm.getFragmentShader(ShaderKeys::ambientLight()));
        groundProgram.addShaderToProgram(sm.getFragmentShader(ShaderKeys::diffuseLight()));
        groundProgram.addShaderToProgram(sm.getFragmentShader(ShaderKeys::pointLight()));
        groundProgram.addShaderToProgram(sm.getFragmentShader(ShaderKeys::fog()));

        auto& tm = TextureManager::getInstance();
        tm.loadTexture2D("snow", "data/textures/snow.png");
        tm.loadTexture2D("pavement", "data/textures/pavement.jpg");
        tm.loadTexture2D("tut008_path", "data/textures/tut008/path.png");
    });
}

void SnowCoveredPlainGround::render() const
{
    const auto& tm = TextureManager::getInstance();
    auto& groundProgram = ShaderProgramManager::getInstance().getShaderProgram("tut025_ground");
    const auto& groundSampler = SamplerManager::getInstance().getSampler("main");
    groundProgram.useProgram();

    groundProgram[ShaderConstants::projectionMatrix()] = MatrixManager::getInstance().getProjectionMatrix();
    groundProgram[ShaderConstants::viewMatrix()] = MatrixManager::getInstance().getViewMatrix();

    ambientLight_.setUniform(groundProgram, ShaderConstants::ambientLight());
    diffuseLight_.setUniform(groundProgram, ShaderConstants::diffuseLight());
    firePointLight_.setUniform(groundProgram, "firePointLight");
    fogParameters_.setUniform(groundProgram, ShaderConstants::fogParams());

    // Render ground
    groundProgram.setModelAndNormalMatrix(glm::mat4(1.0));

    // Setup snow texture
    tm.getTexture("snow").bind(0);
    groundSampler.bind(0);
    groundProgram["snowSampler"] = 0;

    // Setup path texture
    tm.getTexture("tut008_path").bind(1);
    groundSampler.bind(1);
    groundProgram["pathSampler"] = 1;

    // Setup pavement texture
    tm.getTexture("pavement").bind(2);
    groundSampler.bind(2);
    groundProgram["pavementSampler"] = 2;

    PlainGround::render();
}

} // namespace static_meshes_3D