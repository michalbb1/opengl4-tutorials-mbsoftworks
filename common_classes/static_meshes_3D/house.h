#pragma once

// STL
#include <string>

// GLM
#include <glm/glm.hpp>

// Project
#include "staticMesh3D.h"
#include "primitives/cube.h"
#include "primitives/pyramid.h"

namespace static_meshes_3D {

/**
 * House static mesh, using scaled cube and pyramid for rendering.
 */
class House : public StaticMesh3D
{
public:
    static const std::string WALLS_TEXTURE_KEY; // Key to store house walls texture with
    static const std::string DOOR_WALL_TEXTURE_KEY; // Key to store house door wall texture with
    static const std::string WINDOWS_WALL_TEXTURE_KEY; // Key to store house windows wall texture with
    static const std::string ROOF_TEXTURE_KEY; // Key to store house roof texture with

    House(bool withPositions = true, bool withTextureCoordinates = true, bool withNormals = true);

    void render() const override;
    void render(const glm::vec3& position, float rotationAngle) const;

protected:
    void initializeData() override {}

private:
    Cube cube; // Cube static mesh used to render house with
    Pyramid pyramid; // Pyramid static mesh used to render house with
};

} // namespace static_meshes_3D