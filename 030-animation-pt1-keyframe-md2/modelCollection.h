#pragma once

// STL
#include <memory>

// Project
#include "../common_classes/shaderProgram.h"
#include "../common_classes/animated_meshes_3D/md2model.h"

namespace opengl4_mbsoftworks {
namespace tutorial030 {

using namespace common_classes::animated_meshes_3D;

class ModelCollection
{
public:
    static const GLsizei MAX_TEXTURE_SIZE{ 1024 };

    static ModelCollection& getInstance();

    /**
     * Initializes everything in this class.
     */
    void initialize();

    /**
     * Releases everything created in this class.
     */
    void release();

    void renderModels(ShaderProgram& shaderProgram) const;

    const std::string& getCurrentModelName() const;
    const std::string& getCurrentAnimationName() const;
    size_t getCurrentAnimationIndex() const;
    size_t getNumModelAnimations() const;

    void updateAnimations(float deltaTime);
    void nextAnimation();
    void previousAnimation();
    void toggleAnimationLooping();
    bool isAnimationLooping() const;
    void nextModel();
    void previousModel();

private:
    ModelCollection() = default; // Private constructor to make class truly singleton
    ModelCollection(const ModelCollection&) = delete; // No copy constructor allowed
    void operator=(const ModelCollection&) = delete; // No copy assignment allowed

    struct DisplayedModel
    {
        std::string name;
        glm::vec3 position;
        std::unique_ptr<MD2Model> md2Model;
        MD2Model::AnimationState animationState;
        size_t currentAnimationIndex;
        bool loopAnimation{ true };

        void startCurrentAnimation()
        {
            animationState = md2Model->startAnimation(md2Model->getAnimationNames().at(currentAnimationIndex), loopAnimation);
        }

        void toggleAnimationLooping()
        {
            loopAnimation = !loopAnimation;
            animationState = MD2Model::AnimationState{};
        }

        void nextAnimation()
        {
            currentAnimationIndex = (currentAnimationIndex + 1) % md2Model->getAnimationNames().size();
            animationState = MD2Model::AnimationState{};
        }

        void previousAnimation()
        {
            currentAnimationIndex = (currentAnimationIndex + md2Model->getAnimationNames().size() - 1) % md2Model->getAnimationNames().size();
            animationState = MD2Model::AnimationState{};
        }
    };

    std::vector<DisplayedModel> displayedModels_;
    size_t selectedModelIndex_{ 0 };
};

} // namespace tutorial 030
} // namespace opengl4_mbsoftworks