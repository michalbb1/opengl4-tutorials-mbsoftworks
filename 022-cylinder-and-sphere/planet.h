#pragma once

// STL
#include <string>

// Project
#include "../common_classes/OpenGLWindow.h"
#include "../common_classes/HUD.h"

#include "../common_classes/shader_structs/material.h"
#include "../common_classes/static_meshes_3D/primitives/sphere.h"

/**
    Simple class representing planet (sun is also a planet in this tutorial :D)
*/
class Planet
{
public:
    Planet(const std::string& textureKey, const shader_structs::Material& material,
        float radius, float distanceFromCenter, float orbitalPeriodDays, float rotationAngleSpeed, float initialOrbitAngle);

    void render() const;
    void renderPoints() const;

    /**
     * Updates planet data (orbit / rotation angle).
     */
    void update();

    /**
     * Initializes planet geometry (sphere) with given number of slices and stacks.
     */
    static void initializeGeometry(int slices, int stacks);

    /**
     * Frees geometry (releases used memory).
     */
    static void freeGeometry();

private:
    static std::unique_ptr<static_meshes_3D::Sphere> sphereMesh; // Sphere mesh

    std::string _textureKey; // Texture key used to render this planet
    shader_structs::Material _material; // Material used

    float _radius; // Radius of the planet
    float _distanceFromCenter; // Distance from the center of solar system (from sun)
  
    float _travelAngleSpeed; // How fast planet orbits (in radians)
    float _rotationAngleSpeed; // How fast planet rotates around its axis (in radians)

    float _orbitAngle; // Current orbit angle
    float _rotationAngle; // Current rotation angle

     /**
     * Gets model matrix to render this planet.
     */
    glm::mat4 getModelMatrix() const;
};
 