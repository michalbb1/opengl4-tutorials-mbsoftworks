#pragma once

// STL
#include <memory>

// Project
#include "../common_classes/OpenGLWindow.h"

#include "../common_classes/shader_structs/material.h"
#include "../common_classes/static_meshes_3D/primitives/sphere.h"
#include "../common_classes/static_meshes_3D/primitives/cylinder.h"

class SpaceStation
{
public:
    SpaceStation(const glm::vec3& position, float sphereRadius, float cylinderRadius);

    void render() const;
    void renderPoints() const;

    void update();

    /**
     * Initializes space station geometry (sphere and cylinder).
     */
    static void initializeGeometry();

    /**
     * Frees geometry (releases used memory).
     */
    static void freeGeometry();

private:
    static shader_structs::Material material; // Material used (shiny)
    static std::unique_ptr<static_meshes_3D::Sphere> sphereMesh; // Sphere mesh
    static std::unique_ptr<static_meshes_3D::Cylinder> cylinderMesh; // Cylinder mesh

    glm::vec3 position_; // Position of the space station
    float sphereRadius_; // Radius of sphere
    float cylinderRadius_; // Radius of its cylinders
    
    float sphereAngle_; // Sphere rotation angle
    float cylindersAngle_; // Cylinders rotation angle

    float cylinderHeight_; // Cached height of cylinder based on sphere radius

    /**
     * Calculated model matrix for the sphere.
     */
    glm::mat4 getSphereModelMatrix() const;

    /**
     * Calculates model matrices for individual cylinders.
     */
    glm::mat4 getFrontCylinderMatrix() const;
    glm::mat4 getRightCylinderMatrix() const;
    glm::mat4 getBackCylinderMatrix() const;
    glm::mat4 getLeftCylinderMatrix() const;
    glm::mat4 getTopCylinderMatrix() const;
    glm::mat4 getBottomCylinderMatrix() const;

    /**
     * Gathers all cylinder matrices in one vector.
     */
    std::vector<glm::mat4> getAllCylinderMatrices() const;
};
 