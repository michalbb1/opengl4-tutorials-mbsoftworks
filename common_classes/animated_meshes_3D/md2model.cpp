// STL
#include <iostream>
#include <fstream>

// Project
#include "md2model.h"
#include "../../common_classes/shaderProgramManager.h"

namespace opengl4_mbsoftworks {
namespace common_classes {
namespace animated_meshes_3D {

const glm::vec3 MD2Model::ANORMS_TABLE[ANORMS_TABLE_SIZE] =
{
    { -0.525731f,  0.000000f,  0.850651f },
    { -0.442863f,  0.238856f,  0.864188f },
    { -0.295242f,  0.000000f,  0.955423f },
    { -0.309017f,  0.500000f,  0.809017f },
    { -0.162460f,  0.262866f,  0.951056f },
    {  0.000000f,  0.000000f,  1.000000f },
    {  0.000000f,  0.850651f,  0.525731f },
    { -0.147621f,  0.716567f,  0.681718f },
    {  0.147621f,  0.716567f,  0.681718f },
    {  0.000000f,  0.525731f,  0.850651f },
    {  0.309017f,  0.500000f,  0.809017f },
    {  0.525731f,  0.000000f,  0.850651f },
    {  0.295242f,  0.000000f,  0.955423f },
    {  0.442863f,  0.238856f,  0.864188f },
    {  0.162460f,  0.262866f,  0.951056f },
    { -0.681718f,  0.147621f,  0.716567f },
    { -0.809017f,  0.309017f,  0.500000f },
    { -0.587785f,  0.425325f,  0.688191f },
    { -0.850651f,  0.525731f,  0.000000f },
    { -0.864188f,  0.442863f,  0.238856f },
    { -0.716567f,  0.681718f,  0.147621f },
    { -0.688191f,  0.587785f,  0.425325f },
    { -0.500000f,  0.809017f,  0.309017f },
    { -0.238856f,  0.864188f,  0.442863f },
    { -0.425325f,  0.688191f,  0.587785f },
    { -0.716567f,  0.681718f, -0.147621f },
    { -0.500000f,  0.809017f, -0.309017f },
    { -0.525731f,  0.850651f,  0.000000f },
    {  0.000000f,  0.850651f, -0.525731f },
    { -0.238856f,  0.864188f, -0.442863f },
    {  0.000000f,  0.955423f, -0.295242f },
    { -0.262866f,  0.951056f, -0.162460f },
    {  0.000000f,  1.000000f,  0.000000f },
    {  0.000000f,  0.955423f,  0.295242f },
    { -0.262866f,  0.951056f,  0.162460f },
    {  0.238856f,  0.864188f,  0.442863f },
    {  0.262866f,  0.951056f,  0.162460f },
    {  0.500000f,  0.809017f,  0.309017f },
    {  0.238856f,  0.864188f, -0.442863f },
    {  0.262866f,  0.951056f, -0.162460f },
    {  0.500000f,  0.809017f, -0.309017f },
    {  0.850651f,  0.525731f,  0.000000f },
    {  0.716567f,  0.681718f,  0.147621f },
    {  0.716567f,  0.681718f, -0.147621f },
    {  0.525731f,  0.850651f,  0.000000f },
    {  0.425325f,  0.688191f,  0.587785f },
    {  0.864188f,  0.442863f,  0.238856f },
    {  0.688191f,  0.587785f,  0.425325f },
    {  0.809017f,  0.309017f,  0.500000f },
    {  0.681718f,  0.147621f,  0.716567f },
    {  0.587785f,  0.425325f,  0.688191f },
    {  0.955423f,  0.295242f,  0.000000f },
    {  1.000000f,  0.000000f,  0.000000f },
    {  0.951056f,  0.162460f,  0.262866f },
    {  0.850651f, -0.525731f,  0.000000f },
    {  0.955423f, -0.295242f,  0.000000f },
    {  0.864188f, -0.442863f,  0.238856f },
    {  0.951056f, -0.162460f,  0.262866f },
    {  0.809017f, -0.309017f,  0.500000f },
    {  0.681718f, -0.147621f,  0.716567f },
    {  0.850651f,  0.000000f,  0.525731f },
    {  0.864188f,  0.442863f, -0.238856f },
    {  0.809017f,  0.309017f, -0.500000f },
    {  0.951056f,  0.162460f, -0.262866f },
    {  0.525731f,  0.000000f, -0.850651f },
    {  0.681718f,  0.147621f, -0.716567f },
    {  0.681718f, -0.147621f, -0.716567f },
    {  0.850651f,  0.000000f, -0.525731f },
    {  0.809017f, -0.309017f, -0.500000f },
    {  0.864188f, -0.442863f, -0.238856f },
    {  0.951056f, -0.162460f, -0.262866f },
    {  0.147621f,  0.716567f, -0.681718f },
    {  0.309017f,  0.500000f, -0.809017f },
    {  0.425325f,  0.688191f, -0.587785f },
    {  0.442863f,  0.238856f, -0.864188f },
    {  0.587785f,  0.425325f, -0.688191f },
    {  0.688191f,  0.587785f, -0.425325f },
    { -0.147621f,  0.716567f, -0.681718f },
    { -0.309017f,  0.500000f, -0.809017f },
    {  0.000000f,  0.525731f, -0.850651f },
    { -0.525731f,  0.000000f, -0.850651f },
    { -0.442863f,  0.238856f, -0.864188f },
    { -0.295242f,  0.000000f, -0.955423f },
    { -0.162460f,  0.262866f, -0.951056f },
    {  0.000000f,  0.000000f, -1.000000f },
    {  0.295242f,  0.000000f, -0.955423f },
    {  0.162460f,  0.262866f, -0.951056f },
    { -0.442863f, -0.238856f, -0.864188f },
    { -0.309017f, -0.500000f, -0.809017f },
    { -0.162460f, -0.262866f, -0.951056f },
    {  0.000000f, -0.850651f, -0.525731f },
    { -0.147621f, -0.716567f, -0.681718f },
    {  0.147621f, -0.716567f, -0.681718f },
    {  0.000000f, -0.525731f, -0.850651f },
    {  0.309017f, -0.500000f, -0.809017f },
    {  0.442863f, -0.238856f, -0.864188f },
    {  0.162460f, -0.262866f, -0.951056f },
    {  0.238856f, -0.864188f, -0.442863f },
    {  0.500000f, -0.809017f, -0.309017f },
    {  0.425325f, -0.688191f, -0.587785f },
    {  0.716567f, -0.681718f, -0.147621f },
    {  0.688191f, -0.587785f, -0.425325f },
    {  0.587785f, -0.425325f, -0.688191f },
    {  0.000000f, -0.955423f, -0.295242f },
    {  0.000000f, -1.000000f,  0.000000f },
    {  0.262866f, -0.951056f, -0.162460f },
    {  0.000000f, -0.850651f,  0.525731f },
    {  0.000000f, -0.955423f,  0.295242f },
    {  0.238856f, -0.864188f,  0.442863f },
    {  0.262866f, -0.951056f,  0.162460f },
    {  0.500000f, -0.809017f,  0.309017f },
    {  0.716567f, -0.681718f,  0.147621f },
    {  0.525731f, -0.850651f,  0.000000f },
    { -0.238856f, -0.864188f, -0.442863f },
    { -0.500000f, -0.809017f, -0.309017f },
    { -0.262866f, -0.951056f, -0.162460f },
    { -0.850651f, -0.525731f,  0.000000f },
    { -0.716567f, -0.681718f, -0.147621f },
    { -0.716567f, -0.681718f,  0.147621f },
    { -0.525731f, -0.850651f,  0.000000f },
    { -0.500000f, -0.809017f,  0.309017f },
    { -0.238856f, -0.864188f,  0.442863f },
    { -0.262866f, -0.951056f,  0.162460f },
    { -0.864188f, -0.442863f,  0.238856f },
    { -0.809017f, -0.309017f,  0.500000f },
    { -0.688191f, -0.587785f,  0.425325f },
    { -0.681718f, -0.147621f,  0.716567f },
    { -0.442863f, -0.238856f,  0.864188f },
    { -0.587785f, -0.425325f,  0.688191f },
    { -0.309017f, -0.500000f,  0.809017f },
    { -0.147621f, -0.716567f,  0.681718f },
    { -0.425325f, -0.688191f,  0.587785f },
    { -0.162460f, -0.262866f,  0.951056f },
    {  0.442863f, -0.238856f,  0.864188f },
    {  0.162460f, -0.262866f,  0.951056f },
    {  0.309017f, -0.500000f,  0.809017f },
    {  0.147621f, -0.716567f,  0.681718f },
    {  0.000000f, -0.525731f,  0.850651f },
    {  0.425325f, -0.688191f,  0.587785f },
    {  0.587785f, -0.425325f,  0.688191f },
    {  0.688191f, -0.587785f,  0.425325f },
    { -0.955423f,  0.295242f,  0.000000f },
    { -0.951056f,  0.162460f,  0.262866f },
    { -1.000000f,  0.000000f,  0.000000f },
    { -0.850651f,  0.000000f,  0.525731f },
    { -0.955423f, -0.295242f,  0.000000f },
    { -0.951056f, -0.162460f,  0.262866f },
    { -0.864188f,  0.442863f, -0.238856f },
    { -0.951056f,  0.162460f, -0.262866f },
    { -0.809017f,  0.309017f, -0.500000f },
    { -0.864188f, -0.442863f, -0.238856f },
    { -0.951056f, -0.162460f, -0.262866f },
    { -0.809017f, -0.309017f, -0.500000f },
    { -0.681718f,  0.147621f, -0.716567f },
    { -0.681718f, -0.147621f, -0.716567f },
    { -0.850651f,  0.000000f, -0.525731f },
    { -0.688191f,  0.587785f, -0.425325f },
    { -0.587785f,  0.425325f, -0.688191f },
    { -0.425325f,  0.688191f, -0.587785f },
    { -0.425325f, -0.688191f, -0.587785f },
    { -0.587785f, -0.425325f, -0.688191f },
    { -0.688191f, -0.587785f, -0.425325f }
};

bool MD2Model::AnimationState::isRunning() const
{
    return !animationName.empty();
}

void MD2Model::AnimationState::updateAnimation(const float deltaTime)
{
    if(startFrame == endFrame) {
        return;
    }

    totalRunningTime += deltaTime;
    const float oneFrameDuration = 1.0f / static_cast<float>(fps);
    while(totalRunningTime - nextFrameTime > oneFrameDuration)
    {
        nextFrameTime += oneFrameDuration;
        currentFrameIndex = nextFrameIndex;
        nextFrameIndex++;
        if (nextFrameIndex > endFrame) {
            nextFrameIndex = loop ? startFrame : endFrame;
        }
    }

    interpolationFactor = static_cast<float>(fps) * (totalRunningTime - nextFrameTime);
}

MD2Model::MD2Model(const std::string& filePath, const glm::mat4& modelTransformMatrix)
{
    loadModel(filePath, modelTransformMatrix);
}

MD2Model::~MD2Model()
{
    deleteModel();
}

void MD2Model::loadModel(const std::string& filePath, const glm::mat4& modelTransformMatrix)
{
    std::ifstream in(filePath, std::ios::binary);
    if(!in.is_open())
    {
        return;
    }

    // Read header where all info about model is stored
    in.read(reinterpret_cast<char*>(&header_), sizeof(MD2Header));

    // Read all the frames into a buffer which will then be processed
    // Might seem a bit counter-intuitive, but it has to be done like this, because sizeof(MD2Frame) is not same as header_.frameSize
    size_t allFramesSizeBytes = header_.numFrames * header_.frameSize;
    std::vector<char> allFramesData(allFramesSizeBytes);
    in.seekg(header_.offsetFrames);
    in.read(allFramesData.data(), allFramesSizeBytes);

    // Read vertices and normals from all frames data in a per-frame manner
    std::vector<std::vector<glm::vec3>> perFrameVertices(header_.numFrames, std::vector<glm::vec3>(header_.numVertices));
    std::vector<std::vector<glm::vec3>> perFrameNormals(header_.numFrames, std::vector<glm::vec3>(header_.numVertices));
    const auto normalTransformMatrix = glm::transpose(glm::inverse(glm::mat3(modelTransformMatrix)));

    for(size_t frameIndex = 0; frameIndex < static_cast<size_t>(header_.numFrames); frameIndex++)
    {
        const auto& frame = *reinterpret_cast<MD2Frame*>(allFramesData.data() + frameIndex * header_.frameSize);
        for(size_t vertexIndex = 0; vertexIndex < static_cast<size_t>(header_.numVertices); vertexIndex++)
        {
            const auto& frameVertex = frame.vertices[vertexIndex];
            glm::vec3 position(frame.translate[0] + static_cast<float>(frameVertex.v[0]) * frame.scale[0],
                frame.translate[1] + static_cast<float>(frameVertex.v[1]) * frame.scale[1],
                frame.translate[2] + static_cast<float>(frameVertex.v[2]) * frame.scale[2]);
            perFrameVertices[frameIndex][vertexIndex] = glm::vec3(modelTransformMatrix * glm::vec4(position, 1.0f));
            perFrameNormals[frameIndex][vertexIndex] = normalTransformMatrix * ANORMS_TABLE[frameVertex.normal_index];
        }
    }

    // Now try to determine all different animations in the file based on frame names
    Animation* activeAnimation = nullptr;
    for(size_t frameIndex = 0; frameIndex < static_cast<size_t>(header_.numFrames); frameIndex++)
    {
        const auto& frame = *reinterpret_cast<MD2Frame*>(allFramesData.data() + frameIndex * header_.frameSize);
        const auto animationBaseName = getAnimationBaseName(frame.name);
        if(animations_.count(animationBaseName) == 0)
        {
            if(activeAnimation) {
                activeAnimation->lastFrame = frameIndex - 1;
            }

            animations_[animationBaseName] = Animation{ animationBaseName, frameIndex, frameIndex, 7 };
            activeAnimation = &animations_[animationBaseName];
        }

        const auto isLastFrame = frameIndex == static_cast<size_t>(header_.numFrames) - 1;
        if(isLastFrame && activeAnimation) {
            activeAnimation->lastFrame = frameIndex;
        }
    }

    // Now let's read OpenGL rendering commands, which define how to render a single frame
    // There are also texture coordinates mixed within, everything will be buffered in renderCommands
    std::vector<RenderCommandOpenGL> renderCommands;
    std::vector<int32_t> rawGlCommands(header_.numGlCommands);
    in.seekg(header_.offsetGlCommands);
    in.read(reinterpret_cast<char*>(rawGlCommands.data()), static_cast<std::streamsize>(header_.numGlCommands) * sizeof(int32_t));

    // Loop until raw OpenGL command is zero
    verticesPerFrame_ = 0;
    for(auto i = 0; i < header_.numGlCommands && rawGlCommands[i] != 0;) // We while loop until we come to 0 value, which is the end of OpenGL commands
    {
        const auto command = rawGlCommands[i]; // Here is encoded rendering mode and number of vertices
        GLenum renderMode = command < 0 ? GL_TRIANGLE_FAN : GL_TRIANGLE_STRIP; // Rendering mode is one of the two options, depending on the sign
        const auto numVertices = command < 0 ? -command : command; // Number of vertices is just absolute value of the command
        i++;

        // Store render mode and all of the associated drawing commands here
        RenderCommandOpenGL glCommandRender;
        glCommandRender.renderMode = renderMode;

        renderModes.push_back(renderMode); // Remember the values
        numRenderVertices.push_back(numVertices);
        for(auto j = 0; j < numVertices; j++)
        {
            const auto u = *reinterpret_cast<float*>(&rawGlCommands[i++]); // Extract texture coordinates
            const auto v = 1.0f - *reinterpret_cast<float*>(&rawGlCommands[i++]); // Flip t, because it is (for some reasons) stored from top to bottom
            GLint vertexIndex = rawGlCommands[i++];
            glCommandRender.textureCoordinates.emplace_back(u, v);
            glCommandRender.vertexIndex.push_back(vertexIndex);
            verticesPerFrame_++;
        }

        renderCommands.push_back(glCommandRender);
    }

    // Now that we have all the information (per-frame vertices, texture coordinates and per-frame normals), we can construct the VBOs
    vboFrameVertices_.createVBO();
    vboTextureCoordinates_.createVBO();
    vboNormals_.createVBO();
    for(auto i = 0; i < header_.numFrames; i++)
    {
        for(const auto& glCommand : renderCommands)
        {
            for(size_t j = 0; j < glCommand.vertexIndex.size(); j++)
            {
                const auto vertexIndex = glCommand.vertexIndex[j];
                vboFrameVertices_.addData(perFrameVertices[i][vertexIndex]);
                vboTextureCoordinates_.addData(glCommand.textureCoordinates[j]);
                vboNormals_.addData(perFrameNormals[i][vertexIndex]);
            }
        }
    }

    // Now all necessary data are extracted, let's create VAO for rendering MD2 model. Don't set it up though, it is always setup dynamically
    glGenVertexArrays(1, &vao_);
    vboFrameVertices_.bindVBO();
    vboFrameVertices_.uploadDataToGPU(GL_STATIC_DRAW);
    vboTextureCoordinates_.bindVBO();
    vboTextureCoordinates_.uploadDataToGPU(GL_STATIC_DRAW);
    vboNormals_.bindVBO();
    vboNormals_.uploadDataToGPU(GL_STATIC_DRAW);

    // I have read, that if you read the data from header.numSkins and header.offsetSkins,
    // these data are Quake2 specific paths. So usually you will find models on internet
    // with header.numSkins 0 and texture with the same filename as model filename

    // Try to find texture with same name as model and load it
    auto index = filePath.find_last_of("\\/");
    std::string containingDirectory = index != -1 ? filePath.substr(0, index+1) : "";
    std::string justFileName = index != -1 ? filePath.substr(index+1) : filePath;

    const std::string textureExtensions[] = {"jpg", "jpeg", "png", "bmp", "tga"};
    index = justFileName.find_last_of(".");
    if(index != std::string::npos)
    {
        const auto fileNameBase = justFileName.substr(0, index+1);
        for (const auto& textureExtension : textureExtensions)
        {
            const auto fullTexturePath = containingDirectory + fileNameBase + textureExtension;
            if(skinTexture_.loadTexture2D(fullTexturePath, true)) {
                break;
            }
        }
    }

    filePath_ = filePath;
    in.close();
}

bool MD2Model::isLoaded() const
{
    return vao_ != 0;
}

void MD2Model::useQuake2AnimationList()
{
    animations_.clear();
    animationNamesCached_.clear();
    addNewAnimation("Stand", 0, 39, 9);
    addNewAnimation("Run", 40, 45, 10);
    addNewAnimation("Attack", 46, 53, 10);
    addNewAnimation("Pain A", 54, 57, 7);
    addNewAnimation("Pain B", 58, 61, 7);
    addNewAnimation("Pain C", 62, 65, 7);
    addNewAnimation("Jump", 66, 71, 7);
    addNewAnimation("Flip", 72, 83, 7);
    addNewAnimation("Salute", 84, 94, 7);
    addNewAnimation("Fall Back", 95, 111, 10);
    addNewAnimation("Wave", 112, 122, 7);
    addNewAnimation("Pointing", 123, 134, 6);
    addNewAnimation("Crouch Stand", 135, 153, 10);
    addNewAnimation("Crouch Walk", 154, 159, 7);
    addNewAnimation("Crouch Attack", 160, 168, 10);
    addNewAnimation("Crouch Pain", 169, 172, 7);
    addNewAnimation("Crouch Death", 173, 177, 5);
    addNewAnimation("Death Fall Back", 178, 183, 7);
    addNewAnimation("Death Fall Forward", 184, 189, 7);
    addNewAnimation("Death Fall Back Slow", 190, 197, 7);
}

void MD2Model::renderModelAnimated(const AnimationState& animationState)
{
    if (!isLoaded())
    {
        std::cout << "MD2 model has not been loaded, cannot render it!" << std::endl;
        return;
    }

    auto& shaderProgram = ShaderProgramManager::getInstance().getShaderProgram("md2");
    skinTexture_.bind();

    // Setup vertex attributes for current and next frame
    setupVAO(animationState.currentFrameIndex, animationState.nextFrameIndex);

    shaderProgram[ShaderConstants::interpolationFactor()] = animationState.interpolationFactor;
    GLint totalOffset = 0;
    for (size_t i = 0; i < renderModes.size(); i++)
    {
        glDrawArrays(renderModes[i], totalOffset, numRenderVertices[i]);
        totalOffset += numRenderVertices[i];
    } 
}

void MD2Model::renderModelStatic()
{
    if(!isLoaded())
    {
        std::cout << "MD2 model has not been loaded, cannot render it!" << std::endl;
        return;
    }

    skinTexture_.bind();
    setupVAO(0, 0);

    ShaderProgramManager::getInstance().getShaderProgram("md2")[ShaderConstants::interpolationFactor()] = 0;
    GLint totalOffset = 0;
    for (size_t i = 0; i < renderModes.size(); i++) // Just render using previously extracted render modes
    {
        glDrawArrays(renderModes[i], totalOffset, numRenderVertices[i]);
        totalOffset += numRenderVertices[i];
    }
}

const std::vector<std::string>& MD2Model::getAnimationNames()
{
    if(animationNamesCached_.empty())
    {
        for (const auto& kvPair : animations_) {
            animationNamesCached_.push_back(kvPair.first);
        }
    }

    return animationNamesCached_;
}

MD2Model::AnimationState MD2Model::startAnimation(const std::string& animationName, const bool loop, const size_t fps) const
{
    if (animations_.count(animationName) == 0) {
        return AnimationState{};
    }

    const auto& animation = animations_.at(animationName);
    AnimationState animationState;
    animationState.animationName = animationName;
    animationState.startFrame = animation.firstFrame;
    animationState.endFrame = animation.lastFrame;
    animationState.currentFrameIndex = animation.firstFrame;
    animationState.nextFrameIndex = animation.firstFrame + 1;
    animationState.fps = fps > 0 ? fps : animation.fps;
    animationState.loop = loop;
    return animationState;
}

void MD2Model::deleteModel()
{
    if (!isLoaded()) {
        return;
    }

    std::cout << "Deleting MD2 model '" << filePath_ << "':" << std::endl;
    std::cout << "Deleting VAO #" << vao_ << std::endl;
    glDeleteVertexArrays(1, &vao_);
    vao_ = 0;

    vboFrameVertices_.deleteVBO();
    vboTextureCoordinates_.deleteVBO();
    vboNormals_.deleteVBO();

    skinTexture_.deleteTexture();
}

MD2Model::Animation& MD2Model::addNewAnimation(const std::string& animationName, size_t firstFrame, size_t lastFrame, size_t fps)
{
    return animations_[animationName] = Animation{animationName, firstFrame, lastFrame, fps};
}

std::string MD2Model::getAnimationBaseName(const std::string& frameName)
{
    std::string baseName;
    for (const char c : frameName)
    {
        if(isdigit(c)) {
            break;
        }

        baseName += c;
    }

    return baseName;
}

void MD2Model::setupVAO(size_t currentFrame, size_t nextFrame)
{
    if(currentFrame >= static_cast<size_t>(header_.numFrames) || nextFrame >= static_cast<size_t>(header_.numFrames)) {
        return;
    }

    glBindVertexArray(vao_);
    const auto currentFrameByteOffset = currentFrame * verticesPerFrame_ * sizeof(glm::vec3);
    const auto nextFrameByteOffset = nextFrame < static_cast<size_t>(header_.numFrames) ? nextFrame * verticesPerFrame_ * sizeof(glm::vec3) : currentFrameByteOffset;

    // Setup pointers to vertices for current and next frame to perform interpolation
    vboFrameVertices_.bindVBO();
    glEnableVertexAttribArray(POSITION_ATTRIBUTE_INDEX);
    glVertexAttribPointer(POSITION_ATTRIBUTE_INDEX, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), reinterpret_cast<void*>(currentFrameByteOffset));

    glEnableVertexAttribArray(NEXT_POSITION_ATTRIBUTE_INDEX);
    glVertexAttribPointer(NEXT_POSITION_ATTRIBUTE_INDEX, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), reinterpret_cast<void*>(nextFrameByteOffset));

    // Setup pointers to texture coordinates
    vboTextureCoordinates_.bindVBO();
    glEnableVertexAttribArray(TEXTURE_COORDINATE_ATTRIBUTE_INDEX);
    glVertexAttribPointer(TEXTURE_COORDINATE_ATTRIBUTE_INDEX, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), nullptr);

    // Setup pointers to normals for current and next frame to perform interpolation
    vboNormals_.bindVBO();
    glEnableVertexAttribArray(NORMAL_ATTRIBUTE_INDEX);
    glVertexAttribPointer(NORMAL_ATTRIBUTE_INDEX, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), reinterpret_cast<void*>(currentFrameByteOffset));

    glEnableVertexAttribArray(NEXT_NORMAL_ATTRIBUTE_INDEX);
    glVertexAttribPointer(NEXT_NORMAL_ATTRIBUTE_INDEX, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), reinterpret_cast<void*>(nextFrameByteOffset));
}

} // namespace animated_meshes_3D
} // namespace common_classes
} // namespace opengl4_mbsoftworks