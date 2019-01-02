#pragma once

#include <glm/glm.hpp>

class MatrixManager
{
public:
	MatrixManager(const MatrixManager&) = delete;
	void operator=(const MatrixManager&) = delete;

	static MatrixManager& getInstance();

	const glm::mat4& getProjectionMatrix() const;
	const glm::mat4& getViewMatrix() const;

	void setProjectionMatrix(const glm::mat4& projectionMatrix);
	void setViewMatrix(const glm::mat4& viewMatrix);

private:
	MatrixManager() {}

	glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix;
};