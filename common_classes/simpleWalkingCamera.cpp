#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include "simpleWalkingCamera.h"

SimpleWalkingCamera::SimpleWalkingCamera(const glm::vec3& position, const glm::vec3& viewPoint, const glm::vec3& upVector, float moveSpeed, float rotationSpeed)
	: _position(position)
	, _viewPoint(viewPoint)
	, _upVector(upVector)
	, _moveSpeed(moveSpeed)
	, _rotationSpeed(rotationSpeed)
{
	// Standard WSAD controls, as you are used to from games :)
	_forwardKeyCode     = GLFW_KEY_W;
	_backwardKeyCode    = GLFW_KEY_S;
	_rotateLeftKeyCode  = GLFW_KEY_A;
	_rotateRightKeyCode = GLFW_KEY_D;
}

void SimpleWalkingCamera::setMoveSpeed(float moveSpeed)
{
	_moveSpeed = moveSpeed;
}

void SimpleWalkingCamera::setControls(int forwardKeyCode, int backwardKeyCode, int rotateLeftKeyCode, int rotateRightKeyCode)
{
	_forwardKeyCode = forwardKeyCode;
	_backwardKeyCode = backwardKeyCode;
	_rotateLeftKeyCode = rotateLeftKeyCode;
	_rotateRightKeyCode = rotateRightKeyCode;
}

glm::mat4 SimpleWalkingCamera::getViewMatrix() const
{
	return glm::lookAt(_position, _viewPoint, _upVector);
}

void SimpleWalkingCamera::update(std::function<bool(int)> keyInputFunc, std::function<float(float)> speedCorrectionFunc)
{
	if (keyInputFunc(_forwardKeyCode))
	{
		moveBy(speedCorrectionFunc(_moveSpeed));
	}

	if (keyInputFunc(_forwardKeyCode))
	{
		moveBy(speedCorrectionFunc(_moveSpeed));
	}

	if (keyInputFunc(_forwardKeyCode))
	{
		moveBy(speedCorrectionFunc(_moveSpeed));
	}

	if (keyInputFunc(_forwardKeyCode))
	{
		moveBy(speedCorrectionFunc(_moveSpeed));
	}
}

void SimpleWalkingCamera::moveBy(float distance)
{
	glm::vec3 vOffset = getNormalizedViewVector();
	vOffset *= distance;
	_position += vOffset;
	_viewPoint += vOffset;
}

void SimpleWalkingCamera::rotateBy(float angleInDegrees)
{
	glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(angleInDegrees), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::vec4 rotatedViewVector = rotationMatrix * glm::vec4(getNormalizedViewVector(), 1.0f);
	_viewPoint = _position + glm::vec3(rotatedViewVector.x, rotatedViewVector.y, rotatedViewVector.z);
}

glm::vec3 SimpleWalkingCamera::getNormalizedViewVector() const
{
	return glm::normalize(_viewPoint - _position);
}
