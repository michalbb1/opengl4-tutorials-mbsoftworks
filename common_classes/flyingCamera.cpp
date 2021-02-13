// GLFW
#include <GLFW/glfw3.h>

// GLM
#include <glm/gtc/matrix_transform.hpp>

// Project
#include "flyingCamera.h"

FlyingCamera::FlyingCamera(const glm::vec3& position, const glm::vec3& viewPoint, const glm::vec3& upVector, float moveSpeed, float mouseSensitivity)
    : _position(position)
    , _viewPoint(viewPoint)
    , _upVector(upVector)
    , _moveSpeed(moveSpeed)
    , _mouseSensitivity(mouseSensitivity)
{
    // Standard WSAD controls, as you are used to from games :)
    setControls(GLFW_KEY_W, GLFW_KEY_S, GLFW_KEY_A, GLFW_KEY_D);
}

void FlyingCamera::setMoveSpeed(float moveSpeed)
{
    _moveSpeed = moveSpeed;
}

void FlyingCamera::setMouseSensitivity(float mouseSensitivity)
{
    _mouseSensitivity = mouseSensitivity;
}

void FlyingCamera::setControls(int forwardKeyCode, int backwardKeyCode, int strafeLeftKeyCode, int strafeRightKeyCode)
{
    _forwardKeyCode = forwardKeyCode;
    _backwardKeyCode = backwardKeyCode;
    _strafeLeftKeyCode = strafeLeftKeyCode;
    _strafeRightKeyCode = strafeRightKeyCode;
}

void FlyingCamera::setWindowCenterPosition(const glm::i32vec2& windowCenterPosition)
{
    _windowCenterPosition = windowCenterPosition;
}

glm::mat4 FlyingCamera::getViewMatrix() const
{
    return glm::lookAt(_position, _viewPoint, _upVector);
}

glm::vec3 FlyingCamera::getEye() const
{
    return _position;
}

glm::vec3 FlyingCamera::getViewPoint() const
{
    return _viewPoint;
}

glm::vec3 FlyingCamera::getUpVector() const
{
    return _upVector;
}

void FlyingCamera::update(const std::function<bool(int)>& keyInputFunc,
    const std::function<glm::i32vec2()>& getCursorPosFunc,
    const std::function<void(const glm::i32vec2&)>& setCursorPosFunc,
    const std::function<float(float)>& speedCorrectionFunc)
{
    if (keyInputFunc(_forwardKeyCode)) {
        moveBy(speedCorrectionFunc(_moveSpeed));
    }

    if (keyInputFunc(_backwardKeyCode)) {
        moveBy(-speedCorrectionFunc(_moveSpeed));
    }

    if (keyInputFunc(_strafeLeftKeyCode)) {
        strafeBy(-speedCorrectionFunc(_moveSpeed));
    }

    if (keyInputFunc(_strafeRightKeyCode)) {
        strafeBy(speedCorrectionFunc(_moveSpeed));
    }

    const auto curMousePosition = getCursorPosFunc();
    const auto delta = _windowCenterPosition - curMousePosition;

    if (delta.x != 0) {
        rotateLeftRight(static_cast<float>(delta.x) * _mouseSensitivity);
    }

    if (delta.y != 0) {
        rotateUpDown(static_cast<float>(delta.y) * _mouseSensitivity);
    }

    setCursorPosFunc(_windowCenterPosition);
}

void FlyingCamera::moveBy(float distance)
{
    glm::vec3 vOffset = getNormalizedViewVector();
    vOffset *= distance;
    _position += vOffset;
    _viewPoint += vOffset;
}

void FlyingCamera::strafeBy(float distance)
{
    glm::vec3 strafeVector = glm::normalize(glm::cross(getNormalizedViewVector(), _upVector));
    strafeVector = glm::normalize(strafeVector);
    strafeVector *= distance;

    _position += strafeVector;
    _viewPoint += strafeVector;
}

void FlyingCamera::rotateLeftRight(float angleInDegrees)
{
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(angleInDegrees), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::vec4 rotatedViewVector = rotationMatrix * glm::vec4(getNormalizedViewVector(), 0.0f);
    _viewPoint = _position + glm::vec3(rotatedViewVector);
}

void FlyingCamera::rotateUpDown(float angleInDegrees)
{
    const glm::vec3 viewVector = getNormalizedViewVector();
    const glm::vec3 viewVectorNoY = glm::normalize(glm::vec3(viewVector.x, 0.0f, viewVector.z));

    float currentAngleDegrees = glm::degrees(acos(glm::dot(viewVectorNoY, viewVector)));
    if (viewVector.y < 0.0f) {
        currentAngleDegrees = -currentAngleDegrees;
    }

    float newAngleDegrees = currentAngleDegrees + angleInDegrees;
    if (newAngleDegrees > -85.0f && newAngleDegrees < 85.0f)
    {
        glm::vec3 rotationAxis = glm::cross(getNormalizedViewVector(), _upVector);
        rotationAxis = glm::normalize(rotationAxis);

        glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(angleInDegrees), rotationAxis);
        glm::vec4 rotatedViewVector = rotationMatrix * glm::vec4(getNormalizedViewVector(), 0.0f);

        _viewPoint = _position + glm::vec3(rotatedViewVector);
    }
}

glm::vec3 FlyingCamera::getNormalizedViewVector() const
{
    return glm::normalize(_viewPoint - _position);
}
