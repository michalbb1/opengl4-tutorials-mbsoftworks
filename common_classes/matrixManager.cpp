#include "matrixManager.h"

MatrixManager& MatrixManager::getInstance()
{
	static MatrixManager mm;
	return mm;
}

const glm::mat4& MatrixManager::getProjectionMatrix() const
{
	return projectionMatrix;
}

const glm::mat4& MatrixManager::getViewMatrix() const
{
	return viewMatrix;
}

void MatrixManager::setProjectionMatrix(const glm::mat4& projectionMatrix)
{
	this->projectionMatrix = projectionMatrix;
}

void MatrixManager::setViewMatrix(const glm::mat4& viewMatrix)
{
	this->viewMatrix = viewMatrix;
}
