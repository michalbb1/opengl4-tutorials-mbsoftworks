#pragma once

// GLAD
#include <glad/glad.h>

// Project
#include "../staticMeshIndexed3D.h"

namespace static_meshes_3D {

/**
 * Sphere static mesh with given number of slices, stacks and radius.
 */
class Sphere : public StaticMeshIndexed3D
{
public:
    Sphere(float radius, int numSlices, int numStacks, bool withPositions = true, bool withTextureCoordinates = true, bool withNormals = true);

    void render() const override;
    void renderPoints() const override;

    /**
     * Gets sphere radius.
     */
    float getRadius() const;

    /**
     * Gets number of sphere slices (horizontal or also known as latitude subdivisions).
     */
    int getNumSlices() const;

    /**
     * Gets number of sphere stacks (vertical or also known as longitude subdivisions).
     */
    int getNumStacks() const;

private:
    float _radius; // Sphere radius
    int _numSlices; // Number of slices
    int _numStacks; // Number of stacks

    GLuint _numPoleIndices; // Number of indices to render poles with GL_TRIANGLES
    GLuint _numBodyIndices; // Number of indices to render body

    GLuint _northPoleIndexOffset; // Index offset to render north pole (always 0)
    GLuint _bodyIndexOffset; // Index offset to render body
    GLuint _southPoleIndexOffset; // Index offset to render south pole

    void initializeData() override;
};

} // namespace static_meshes_3D