#pragma once

// Project
#include "../staticMesh3D.h"

namespace static_meshes_3D {

/**
 * Cylinder static mesh with given radius, number of slices and height.
 */
class Cylinder : public StaticMesh3D
{
public:
    Cylinder(float radius, int numSlices, float height,
        bool withPositions = true, bool withTextureCoordinates = true, bool withNormals = true);

    void render() const override;
    void renderPoints() const override;

    /**
     * Gets cylinder radius.
     */
    float getRadius() const;

    /**
     * Gets number of cylinder slices.
     */
    int getSlices() const;

    /**
     * Gets cylinder height.
     */
    float getHeight() const;

private:
    float _radius; // Cylinder radius (distance from the center of cylinder to surface)
    int _numSlices; // Number of cylinder slices
    float _height; // Height of the cylinder

    int _numVerticesSide; // How many vertices to render side of the cylinder
    int _numVerticesTopBottom; // How many vertices to render top / bottom of the cylinder
    int _numVerticesTotal; // Just a sum of both numbers above
    
    void initializeData() override;
};

} // namespace static_meshes_3D