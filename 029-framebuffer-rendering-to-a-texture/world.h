#pragma once

// STL
#include <memory>

// Project
#include "../common_classes/frameBuffer.h"
#include "../common_classes/shaderProgram.h"
#include "../common_classes/static_meshes_3D/primitives/cube.h"
#include "../common_classes/static_meshes_3D/plainGround.h"
#include "../common_classes/static_meshes_3D/primitives/cylinder.h"
#include "../common_classes/static_meshes_3D/primitives/sphere.h"
#include "../common_classes/static_meshes_3D/primitives/torus.h"

class World
{
public:
    static const GLsizei MAX_TEXTURE_SIZE{ 1024 };

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

    void renderScreensWithPrimitives(ShaderProgram& shaderProgram) const;

    void nextTextureSize();
    void previousTextureSize();

    GLsizei getCurrentTextureSize() const;

    void reshuffleScreenMeshes();
    void reshuffleScreenTextures();
    void updateWorld(float deltaTime);

    struct ScreenWithMesh
    {
        std::unique_ptr<FrameBuffer> frameBuffer;

        std::shared_ptr<static_meshes_3D::StaticMesh3D> mesh;
        int textureFaceBitmask;
        std::string meshTextureKey;
        glm::vec3 position;
        glm::vec3 scale;
    };

private:
    World() = default; // Private constructor to make class truly singleton
    World(const World&) = delete; // No copy constructor allowed
    void operator=(const World&) = delete; // No copy assignment allowed

    std::vector<ScreenWithMesh> screens_;
    GLsizei currentTextureSize_{512};
    float meshesRotationAngle_{ 0.0f };

    std::unique_ptr<static_meshes_3D::PlainGround> plainGround_; // Plain ground used to render ground
    std::shared_ptr<static_meshes_3D::Cube> cube_; // Cube used to render picture frames
    std::vector<std::shared_ptr<static_meshes_3D::StaticMesh3D>> meshes_; // Meshes rendered on the screens
};


