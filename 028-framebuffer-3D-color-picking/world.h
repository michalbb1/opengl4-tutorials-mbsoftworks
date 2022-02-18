#pragma once

// STL
#include <memory>

// Project
#include "../common_classes/shaderProgram.h"
#include "../common_classes/static_meshes_3D/primitives/cube.h"
#include "../common_classes/static_meshes_3D/plainGround.h"

namespace opengl4_mbsoftworks {
namespace tutorial028 {

class World
{
public:
    static World& getInstance();

    /**
     * Initializes everything in this class.
     */
    void initialize();

    /**
     * Releases everything created in this class.
     */
    void release();

    /**
     * Renders ground.
     * 
     * @param shaderProgram  Shader program to render ground with
     */
    void renderGround(ShaderProgram& shaderProgram) const;

    /**
     * Renders crates maze.
     * 
     * @param shaderProgram  Shader program to render crates maze with
     */
    void renderCratesMaze(ShaderProgram& shaderProgram) const;

private:
    World() = default; // Private constructor to make class truly singleton
    World(const World&) = delete; // No copy constructor allowed
    void operator=(const World&) = delete; // No copy assignment allowed

    std::unique_ptr<static_meshes_3D::PlainGround> plainGround_; // Plain ground used to render ground
    std::unique_ptr<static_meshes_3D::Cube> cube_; // Cube used to render crates maze
};

} // namespace tutorial028
} // namespace opengl4_mbsoftworks