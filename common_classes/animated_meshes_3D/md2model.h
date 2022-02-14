#pragma once

// STL
#include <map>
#include <string>
#include <vector>

// GLM
#include <glm/glm.hpp>

// GLAD
#include <glad/glad.h>

// Project
#include "../../common_classes/vertexBufferObject.h"
#include "../../common_classes/texture.h"

namespace opengl4_mbsoftworks {
namespace common_classes {
namespace animated_meshes_3D {

class MD2Model
{
public:
    struct AnimationState
    {
        std::string animationName;
        size_t startFrame{ 0 }; // First frame of the animation
        size_t endFrame{ 0 }; // Last frame of the animation
        size_t fps{ 0 }; // Frames per second for this animation (defines animation speed basically)

        float totalRunningTime{ 0.0f }; // Current time
        float previousNextFrameTime{ 0.0f }; // old time
        float interpolationFactor { 0.0f }; // percent of interpolation

        size_t currentFrame{ 0 }; // current frame
        size_t nextFrame { 0 }; // next frame
        bool loop{ true }; // True if you want to run animation in loop

        bool isRunning() const
        {
            return !animationName.empty();
        }

        void updateAnimation(float deltaTime);
    };

    MD2Model() = default;
    explicit MD2Model(const std::string& filePath, const glm::mat4& modelTransformMatrix = glm::mat4(1.0f));

    MD2Model& operator=(const MD2Model& other) = delete; // Don't allow copy constructor
    MD2Model& operator=(const MD2Model&& other) = delete; // Don't allow move constructor
    MD2Model(const MD2Model& other) = delete; // Don't allow copy assignment
    MD2Model(MD2Model&& other) = delete; // Don't allow move assignment
    
    ~MD2Model();

    void loadModel(const std::string& filePath, const glm::mat4& modelTransformMatrix = glm::mat4(1.0f));
    bool isLoaded() const;
    void useQuake2AnimationList();

    void renderModelAnimated(const AnimationState& animationState);
    void renderModelStatic();

    const std::vector<std::string>& getAnimationNames();
    
    AnimationState startAnimation(const std::string& animationName, bool loop = true, size_t fps = 0) const;

    void deleteModel();

private:
    static constexpr int MD2_IDENT = ('2' << 24) + ('P' << 16) + ('D' << 8) + 'I'; // magic number "IDP2" or 844121161
    static constexpr int MD2_VERSION = 8;
    static constexpr int ANORMS_TABLE_SIZE = 162;
    static constexpr int MAX_MD2_VERTICES = 2048;

    static constexpr int POSITION_ATTRIBUTE_INDEX = 0;
    static constexpr int TEXTURE_COORDINATE_ATTRIBUTE_INDEX = 1;
    static constexpr int NORMAL_ATTRIBUTE_INDEX = 2;
    static constexpr int NEXT_POSITION_ATTRIBUTE_INDEX = 3;
    static constexpr int NEXT_NORMAL_ATTRIBUTE_INDEX = 4;

    static const glm::vec3 ANORMS_TABLE[ANORMS_TABLE_SIZE];

    // md2 header
    struct MD2Header
    {
        int32_t ident{ 0 };                    // Magic number, must be equal to "IDP2"
        int32_t version{ 0 };                  // MD2 version, must be equal to 8

        int32_t skinWidth{ 0 };                // Width of the texture
        int32_t skinHeight{ 0 };               // Height of the texture
        int32_t frameSize{ 0 };                // Size of one frame in bytes

        int32_t numSkins{ 0 };                 // Number of textures
        int32_t numVertices{ 0 };              // Number of vertices
        int32_t numTextureCoordinates{ 0 };    // Number of texture coordinates
        int32_t numTriangles{ 0 };             // Number of triangles
        int32_t numGlCommands{ 0 };            // Number of OpenGL commands
        int32_t numFrames{ 0 };                // Total number of frames

        int32_t offsetSkins{ 0 };              // Offset to skin names (64 bytes each)
        int32_t offsetTextureCoordinates{ 0 }; // Offset to texture coordinates
        int32_t offsetTriangles{ 0 };          // Offset to triangles
        int32_t offsetFrames{ 0 };             // Offset to frame data
        int32_t offsetGlCommands{ 0 };         // Offset to OpenGL commands
        int32_t offsetEnd{ 0 };                // Offset to the end of file
    };

    // md2 compressed vertex
    struct MD2Vertex
    {
        unsigned char v[3];         // compressed vertex (x, y, z) coordinates
        unsigned char normal_index; // index to a normal vector for the lighting
    };

    // md2 frame data (vertices are stored here)
    struct MD2Frame
    {
        float scale[3];     // scale values
        float translate[3]; // translation vector
        char name[16];      // frame name
        MD2Vertex vertices[1]; // first vertex of this frame
    };

    struct RenderCommandOpenGL
    {
        GLenum renderMode{ 0 };
        std::vector<int32_t> vertexIndex;
        std::vector<glm::vec2> textureCoordinates;
    };

    struct Animation
    {
        std::string baseName;
        size_t firstFrame{ 0 };
        size_t lastFrame{ 0 };
        size_t fps{ 0 };
    };

    std::string filePath_;
    MD2Header header_;

    GLuint vao_{ 0 };
    VertexBufferObject vboFrameVertices_;
    VertexBufferObject vboTextureCoordinates_;
    VertexBufferObject vboNormals_;

    std::vector<GLenum> renderModes; // Rendering modes
    std::vector<GLsizei> numRenderVertices; // with number of vertices
    size_t verticesPerFrame_ { 0 }; // Number of vertices stored per frame that cover whole rendering process

    Texture skinTexture_;

    std::map<std::string, Animation> animations_;
    std::vector<std::string> animationNamesCached_;

    void setupVAO(size_t currentFrame, size_t nextFrame = -1);

    Animation& addNewAnimation(const std::string& animationName, size_t firstFrame, size_t lastFrame, size_t fps);
    static std::string getAnimationBaseName(const std::string& frameName);
};

} // namespace opengl4_mbsoftworks
} // namespace common_classes
} // namespace animated_meshes_3D