#pragma once

// GLAD
#include <glad/glad.h>

// GLM
#include <glm/glm.hpp>

// Project
#include "../staticMesh2D.h"

namespace static_meshes_2D {

/**
 * Quad static mesh of unit size.
 */
class Quad : public StaticMesh2D
{
public:
    Quad(bool withPositions = true, bool withTextureCoordinates = true);

    void render() const override;

    static glm::vec2 vertices[4]; // Array of mesh vertices (and texture coordinates as well)

private:
    void initializeData() override;
};

} // namespace static_meshes_2D