// GLM
#include <glm/gtx/rotate_vector.hpp>

// Project
#include "modelCollection.h"

#include "../common_classes/textureManager.h"
#include "../common_classes/shader_structs/diffuseLight.h"

namespace opengl4_mbsoftworks {
namespace tutorial030 {

ModelCollection& ModelCollection::getInstance()
{
    static ModelCollection world;
    return world;
}

void ModelCollection::initialize()
{
	auto modelBladeTransformMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 24.0f, 0.0f));
    modelBladeTransformMatrix = glm::rotate(modelBladeTransformMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    auto modelBlade = std::make_unique<MD2Model>();
    modelBlade->loadModel("data/models/animated/md2/blade/blade.md2", modelBladeTransformMatrix);
    modelBlade->useQuake2AnimationList();

    auto modelWarriorTransformMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    modelWarriorTransformMatrix = glm::scale(modelWarriorTransformMatrix, glm::vec3(0.65f, 0.65f, 0.65f));
    auto modelWarrior = std::make_unique<MD2Model>();
    modelWarrior->loadModel("data/models/animated/md2/warrior/warrior.md2", modelWarriorTransformMatrix);

    auto modelSamuraiTransformMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 24.0f, 0.0f));
    modelSamuraiTransformMatrix = glm::rotate(modelSamuraiTransformMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    auto modelSamurai = std::make_unique<MD2Model>();
    modelSamurai->loadModel("data/models/animated/md2/samourai/samourai.md2", modelSamuraiTransformMatrix);
    modelSamurai->useQuake2AnimationList();

    displayedModels_.push_back({ "Blade", glm::vec3(-40.0f, 0.0f, 0.0f), std::move(modelBlade), MD2Model::AnimationState{}, 0});
    displayedModels_.push_back({ "Warrior", glm::vec3(0.0f, 0.0f, 0.0f), std::move(modelWarrior), MD2Model::AnimationState{}, 0});
    displayedModels_.push_back({ "Samurai", glm::vec3(40.0f, 0.0f, 20.0f), std::move(modelSamurai), MD2Model::AnimationState{}, 0});
}

void ModelCollection::release()
{
    displayedModels_.clear();
}

void ModelCollection::renderModels(ShaderProgram& shaderProgram) const
{
    for (size_t i = 0; i < displayedModels_.size(); i++)
    {
        const auto& displayedModel = displayedModels_.at(i);
        const auto usedColor = i == selectedModelIndex_ ? glm::vec4(1.0f, 0.5f, 0.5f, 1.0f) : glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        shaderProgram[ShaderConstants::color()] = usedColor;
        auto modelMatrix = glm::translate(glm::mat4(1.0f), displayedModel.position);
        modelMatrix = glm::rotate(modelMatrix, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        shaderProgram.setModelAndNormalMatrix(modelMatrix);
        displayedModel.md2Model->renderModelAnimated(displayedModel.animationState);
    }
}

const std::string& ModelCollection::getCurrentModelName() const
{
    return displayedModels_.at(selectedModelIndex_).name;
}

const std::string& ModelCollection::getCurrentAnimationName() const
{
    return displayedModels_.at(selectedModelIndex_).animationState.animationName;
}

size_t ModelCollection::getCurrentAnimationIndex() const
{
    return displayedModels_.at(selectedModelIndex_).currentAnimationIndex;
}

size_t ModelCollection::getNumModelAnimations() const
{
    return displayedModels_.at(selectedModelIndex_).md2Model->getAnimationNames().size();
}

void ModelCollection::updateAnimations(float deltaTime)
{
    for (auto& displayedModel : displayedModels_)
    {
        if (!displayedModel.animationState.isRunning())
        {
            displayedModel.startCurrentAnimation();
        }

        displayedModel.animationState.updateAnimation(deltaTime);
    }
}

void ModelCollection::nextAnimation()
{
    displayedModels_[selectedModelIndex_].nextAnimation();
}

void ModelCollection::previousAnimation()
{
    displayedModels_[selectedModelIndex_].previousAnimation();
}

void ModelCollection::toggleAnimationLooping()
{
    displayedModels_[selectedModelIndex_].toggleAnimationLooping();
}

bool ModelCollection::isAnimationLooping() const
{
    return displayedModels_.at(selectedModelIndex_).loopAnimation;
}

void ModelCollection::nextModel()
{
    selectedModelIndex_ = (selectedModelIndex_ + 1) % displayedModels_.size();
}

void ModelCollection::previousModel()
{
    selectedModelIndex_ = (selectedModelIndex_ + displayedModels_.size() - 1) % displayedModels_.size();
}

} // namespace tutorial 030
} // namespace opengl4_mbsoftworks