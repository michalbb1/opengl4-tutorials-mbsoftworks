#pragma once

// STL
#include <memory>

// GLM
#include <glm/vec3.hpp>

// Project
#include "../common_classes/frameBuffer.h"
#include "../common_classes/shader_structs/material.h"
#include "../common_classes/static_meshes_3D/primitives/cube.h"
#include "../common_classes/static_meshes_3D/staticMesh3D.h"

namespace opengl4_mbsoftworks {
namespace tutorial028 {

/**
 * Manages rendering and picking of all objects on the scene.
 */
class ObjectPicker
{
public:
    static ObjectPicker& getInstance();

    /**
     * Initializes everything in this class - loads meshes, creates pickable objects etc.
     */
    void initialize();

    /**
     * Releases everything created in this class.
     */
    void release();

    /**
     * Renders all pickable objects.
     */
    void renderAllPickableObjects();

    /**
     * Updates all pickable objects.
     *
     * @param deltaTime  Time passed since the last frame (in seconds)
     */
    void updateAllPickableObjects(float deltaTime);

    /**
     * Returns true if we have pixel-perfect selection mode enabled or false otherwise.
     */
    bool isPixelPerfectSelectionMode() const;

    /**
     * Toggles pixel-perfect selection mode.
     */
    void togglePixelPerfectSelectionMode();

    /**
     * Performs object picking at certain point.
     *
     * @param x  X coordinate of the pick
     * @param y  Y coordinate of the pick
     */
    void performObjectPicking(int x, int y);

    /**
     * Copies color from our picking FBO to the default FBO.
     * Used to visualize how picking works.
     */
    void copyColorToDefaultFrameBuffer() const;

    /**
     * Gets description of currently selected object (mesh type and index).
     */
    std::string getSelectedObjectDescription() const;

    /**
     * Resizes picking FBO. This method is called when window is resized.
     */
    void resizePickingFrameBuffer(int width, int height);

private:
    ObjectPicker() = default; // Private constructor to make class truly singleton
    ObjectPicker(const ObjectPicker&) = delete; // No copy constructor allowed
    void operator=(const ObjectPicker&) = delete; // No copy assignment allowed

    /**
     * Renders all pickable objects in color picking mode.
     */
    void renderInColorMode();

    /**
     * Helper struct that holds all information about pickable object.
     */
    struct PickableObject
    {
        glm::vec3 position; // Position of the object
        static_meshes_3D::StaticMesh3D* meshPtr; // Pointer to a mesh to be rendered
        glm::vec3 occlusionBoxSize; // Size of occlusion box in case we don't render mesh
        float rotationAngleRad{ 0.0f }; // Rotation angle for cool effect
        float renderScaleAngleRad{ 0.0f }; // Angle used to calculate scale and create nice pulsing effect when object is selected 

        /**
         * Calculates rendering scale from the renderScaleAngleRad with sine method. Creates nice pulsing effect.
         */
        glm::vec3 getRenderScale() const
        {
            const auto sine = sin(renderScaleAngleRad);
            const auto scaleFactor = 1.0f + sine * 0.1f;
            return glm::vec3{ scaleFactor };
        }
    };

    std::unique_ptr<static_meshes_3D::Cube> occluderCube_; // Mesh serving as occluder (cube)
    std::vector<std::unique_ptr<static_meshes_3D::StaticMesh3D>> meshes_; // Available meshes
    std::vector<PickableObject> objects_; // All pickable objects on the scene

    FrameBuffer pickingFrameBuffer_; // Framebuffer used for 3D color picking
    bool pixelPerfectSelectionMode_{ true }; // Are we rendering objects exactly as they are or just their occlusion boxes?
    int selectedIndex_{ std::numeric_limits<int>::max() }; // Index of selected object - if greater or equal than number of objects, nothing is selected

    shader_structs::Material shinyMaterial_{ 1.0f, 32.0f }; // Shiny material to render objects with
};

} // namespace tutorial028
} // namespace opengl4_mbsoftworks