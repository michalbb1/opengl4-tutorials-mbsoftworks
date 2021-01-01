#pragma once

// Project
#include "../staticMeshIndexed3D.h"

namespace static_meshes_3D {

/**
 * Torus static mesh with given subdivisions and radii.
 */
class Torus : public StaticMeshIndexed3D
{
public:
    Torus(int mainSegments, int tubeSegments, float mainRadius, float tubeRadius,
        bool withPositions = true, bool withTextureCoordinates = true, bool withNormals = true);

    void render() const override;
    void renderPoints() const override;

    /**
     * Gets main torus radius.
     */
    float getMainRadius() const;

    /**
     * Gets torus tube radius.
     */
    float getTubeRadius() const;

private:
    int _mainSegments; // Number of main subdivisons (around whole torus)
    int _tubeSegments; // Number of tube subdivisions
    float _mainRadius; // Radius of torus (distance from center of torus to the center of tube)
    float _tubeRadius; // Radius of tube

    void initializeData() override;
};

} // namespace static_meshes_3D