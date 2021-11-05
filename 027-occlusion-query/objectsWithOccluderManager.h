#pragma once

// STL
#include <memory>

// GLM
#include <glm/vec3.hpp>

// Project
#include "../common_classes/occlusionQuery.h"
#include "../common_classes/shader_structs/material.h"
#include "../common_classes/static_meshes_3D/primitives/cube.h"
#include "../common_classes/static_meshes_3D/staticMesh3D.h"

/**
 * Manages all objects with occluders on the scene.
 */
class ObjectsWithOccludersManager
{
public:
    static ObjectsWithOccludersManager& getInstance();

    /**
     * Initializes everything in this class - loads meshes, creates occlusion query etc.
     */
    void initialize();

    /**
     * Releases everything created in this class.
     */
    void release();

    /**
     * Updates objects - generates new, moves them upwards, removes ones that flew too high 
     * and then performs occlusion test on the remaining objects.
     */
    void updateAndPerformOcclusionQuery(float deltaTime);

    /**
     * Renders all objects, but skips objects, that are not visible
     * (that haven't passed the occlusion test).
     */
    void renderAllVisibleObjects();

    /**
     * Renders occluders (helps to visualize how occlusion query works).
     */
    void renderOccluders();

    /**
     * Gets number of all objects on the scene.
     */
    size_t getNumObjects() const;

    /**
     * Gets number of only visible objects on the scene.
     */
    size_t getNumVisibleObjects() const;
    
private:
    ObjectsWithOccludersManager() = default; // Private constructor to make class truly singleton
    ObjectsWithOccludersManager(const ObjectsWithOccludersManager&) = delete; // No copy constructor allowed
    void operator=(const ObjectsWithOccludersManager&) = delete; // No copy assignment allowed

    const float GENERATE_OBJECT_EVERY_SECONDS{ 0.2f }; // How often should new objects be generated

    /**
     * Helper struct that holds object on the scene with information
     * about its occlusion box.
     */
    struct ObjectWithOccluder
    {
        glm::vec3 position;
        static_meshes_3D::StaticMesh3D* meshPtr;
        glm::vec3 occlusionBoxSize;
        bool isVisible;
    };

    std::unique_ptr<static_meshes_3D::Cube> occluderCube_; // Mesh serving as occluder (cube)
    std::unique_ptr<OcclusionQuery> occlusionQuery_; // Occlusion query object

    std::vector<std::unique_ptr<static_meshes_3D::StaticMesh3D>> meshes_; // Available meshes
    std::vector<glm::vec3> occlusionBoxSizes_; // With their corresponding occlusion box sizes

    shader_structs::Material shinyMaterial_{ 1.0f, 32.0f }; // Shiny material to render objects with
    shader_structs::Material dimMaterial_{0.1f, 64.0f}; // Dim material to render objects with

    std::vector<ObjectWithOccluder> objects_; // All objects on the scene
    float timePassedSinceLastGeneration_{ 0.0f }; // Tracks the time of objects generation
    size_t numVisibleObjects_{ 0 }; // Number of visible objects since the last update
};