// STL
#include <mutex>

// GLM
#include <glm/glm.hpp>

// Project
#include "snowCoveredPlainGround.h"

#include "../shaderManager.h"
#include "../shaderProgramManager.h"
#include "../textureManager.h"
#include "../samplerManager.h"
#include "../matrixManager.h"

namespace static_meshes_3D {

SnowCoveredPlainGround::SnowCoveredPlainGround(bool withPositions, bool withTextureCoordinates, bool withNormals)
	: PlainGround(withPositions, withTextureCoordinates, withNormals)
{
    static std::once_flag prepareOnceFlag;
    std::call_once(prepareOnceFlag, []()
    {
        auto& sm = ShaderManager::getInstance();
        sm.loadVertexShader("tut008_ground", "data/shaders/tut008/ground_shader.vert");
        sm.loadFragmentShader("tut008_ground", "data/shaders/tut008/ground_shader.frag");

        auto& groundProgram = ShaderProgramManager::getInstance().createShaderProgram("snow_ground");
        groundProgram.addShaderToProgram(sm.getVertexShader("tut008_ground"));
        groundProgram.addShaderToProgram(sm.getFragmentShader("tut008_ground"));

        auto& tm = TextureManager::getInstance();
        tm.loadTexture2D("snow", "data/textures/snow.png");
        tm.loadTexture2D("pavement", "data/textures/pavement.jpg");
        tm.loadTexture2D("tut008_path", "data/textures/tut008/path.png");
    });
}

void SnowCoveredPlainGround::render() const
{
    const auto& tm = TextureManager::getInstance();
    auto& groundProgram = ShaderProgramManager::getInstance().getShaderProgram("snow_ground");
    const auto& groundSampler = SamplerManager::getInstance().getSampler("main");
    groundProgram.useProgram();

    groundProgram[ShaderConstants::projectionMatrix()] = MatrixManager::getInstance().getProjectionMatrix();
    groundProgram[ShaderConstants::viewMatrix()] = MatrixManager::getInstance().getViewMatrix();

    // Render ground
    groundProgram[ShaderConstants::modelMatrix()] = glm::mat4(1.0);

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