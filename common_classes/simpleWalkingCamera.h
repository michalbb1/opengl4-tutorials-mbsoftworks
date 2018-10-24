#pragma once

#include <functional>

#include <glm/glm.hpp>

class SimpleWalkingCamera
{
public:
	SimpleWalkingCamera(const glm::vec3& position, const glm::vec3& viewPoint, const glm::vec3& upVector, float moveSpeed = 25.0f, float rotationSpeed = 135.0f);

	void setMoveSpeed(float moveSpeed);
	void setRotationSpeed(float degreesPerSecond);
	void setControls(int forwardKeyCode, int backwardKeyCode, int rotateLeftKeyCode, int rotateRightKeyCode);

	glm::mat4 getViewMatrix() const;
	void update(std::function<bool(int)> keyInputFunct, std::function<float(float)> speedCorrectionFunc);

private:
	void moveBy(float distance);
	void rotateBy(float angleInDegrees);
	glm::vec3 getNormalizedViewVector() const;

	glm::vec3 _position;
	glm::vec3 _viewPoint;
	glm::vec3 _upVector;

	float _moveSpeed;
	float _rotationSpeed;
	int _forwardKeyCode;
	int _backwardKeyCode;
	int _rotateLeftKeyCode;
	int _rotateRightKeyCode;
};