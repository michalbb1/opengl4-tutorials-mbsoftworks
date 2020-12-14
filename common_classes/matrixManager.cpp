// Project
#include "matrixManager.h"

MatrixManager& MatrixManager::getInstance()
{
    static MatrixManager mm;
    return mm;
}

const glm::mat4& MatrixManager::getProjectionMatrix() const
{
    return _projectionMatrix;
}

const glm::mat4& MatrixManager::getOrthoProjectionMatrix() const
{
    return _orthoProjectionMatrix;
}

const glm::mat4& MatrixManager::getViewMatrix() const
{
    return _viewMatrix;
}

void MatrixManager::setProjectionMatrix(const glm::mat4& projectionMatrix)
{
    _projectionMatrix = projectionMatrix;
}

void MatrixManager::setOrthoProjectionMatrix(const glm::mat4& orthoProjectionMatrix)
{
    _orthoProjectionMatrix = orthoProjectionMatrix;
}

void MatrixManager::setViewMatrix(const glm::mat4& viewMatrix)
{
    _viewMatrix = viewMatrix;
}
