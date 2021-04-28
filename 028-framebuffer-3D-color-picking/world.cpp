// GLM
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

// Project
#include "world.h"

World& World::getInstance()
{
    static World world;
    return world;
}

void World::initialize()
{
    plainGround_ = std::make_unique<static_meshes_3D::PlainGround>(true, true, true);
    cube_ = std::make_unique<static_meshes_3D::Cube>();
}

void World::release()
{
    plainGround_.reset();
    cube_.reset();
}

void World::renderGround(ShaderProgram& shaderProgram) const
{
    shaderProgram.setModelAndNormalMatrix(glm::mat4(1.0f));
    plainGround_->render();
}

void World::renderCratesMaze(ShaderProgram& shaderProgram) const
{
    const auto crateSize = 20.0f;
    const auto crateOffset = 2.0f;

    glm::vec3 currentPosition{ -100.0f, crateSize / 2.0f + 0.01f, 100.0f };
    glm::vec3 direction{ 0.0f, 0.0f, -1.0f };

    for(auto cratesThisTurn = 10; cratesThisTurn > 0; cratesThisTurn--)
    {
        for (auto i = 0; i < cratesThisTurn; i++)
        {
            if (i > 0) {
                currentPosition += direction * (crateSize + crateOffset);
            }

            auto modelMatrix = glm::translate(glm::mat4(1.0f), currentPosition);
            modelMatrix = glm::scale(modelMatrix, glm::vec3(crateSize, crateSize, crateSize));
            shaderProgram.setModelAndNormalMatrix(modelMatrix);
            cube_->render();
        }

        direction = glm::rotateY(direction, glm::radians(-90.0f));
        currentPosition += direction * (crateSize + crateOffset);
    }
}
