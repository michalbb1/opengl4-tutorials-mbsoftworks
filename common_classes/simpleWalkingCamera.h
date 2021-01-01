#pragma once

// STL
#include <functional>

// GLM
#include <glm/glm.hpp>

/**
 * Implements a camera, that can walk around the world and rotate left / right only.
 */
class SimpleWalkingCamera
{
public:
    SimpleWalkingCamera(const glm::vec3& position, const glm::vec3& viewPoint, const glm::vec3& upVector, float moveSpeed = 10.0f, float rotationSpeed = 135.0f);

    /**
     * Sets movement speed of camera (how fast camera moves forward / backward).
     * @param moveSpeed  Movement speed of the camera(distance travelled per second)
     */
    void setMoveSpeed(float moveSpeed);

    /**
     * Sets rotation speed of camera (how fast camera rotates).
     * 
     * @param degreesPerSecond How many degrees per second should camera be rotating
     */
    void setRotationSpeed(float degreesPerSecond);

    /**
     * Sets keys to control camera with.
     * 
     * @param forwardKeyCode      Key to move forward
     * @param backwardKeyCode     Key to move backward
     * @param rotateLeftKeyCode   Key to rotate left
     * @param rotateRightKeyCode  Key to rotate right
     */
    void setControls(int forwardKeyCode, int backwardKeyCode, int rotateLeftKeyCode, int rotateRightKeyCode);

    /**
     * Gets the current view matrix calculated from position and viewpoint of camera.
     */
    glm::mat4 getViewMatrix() const;

    /**
     * Gets the current eye position (camera's position).
     */
    glm::vec3 getEye() const;

    /**
     * Gets the point camera looks to.
     */
    glm::vec3 getViewPoint() const;

    /**
     * Gets the up vector of the camera.
     */
    glm::vec3 getUpVector() const;

    /**
     * Updates camera - reacts on key presses and updates camera's internal state (position, view vector...).
     * 
     * @param keyInputFunc         Function that detects key presses
     * @param speedCorrectionFunc  Function, that corrects floating point value according to the time passed
     */
    void update(const std::function<bool(int)>& keyInputFunc, const std::function<float(float)>& speedCorrectionFunc);

private:
    /**
     * Moves camera by specified distance (positive value = move forward, negative value = move backward).
     *
     * @param distance  Distance to move by
     */
    void moveBy(float distance);

    /**
     * Rotates camera view by specified angle to the left or right side.
     *
     * @param angleInDegrees Angle to rotate by (in degrees)
     */
    void rotateBy(float angleInDegrees);

    /**
     * Gets normalized view vector of the camera.
     */
    glm::vec3 getNormalizedViewVector() const;

    glm::vec3 _position; // Camera's position (eye position)
    glm::vec3 _viewPoint; // Viewpoint - where does camera look to
    glm::vec3 _upVector; // Up vector of the camera

    float _moveSpeed; // How fast the camera is moving forwards and backwards
    float _rotationSpeed; // How fast the camera rotates around Y-axis (in degrees per second)
    int _forwardKeyCode; // Key to move forward
    int _backwardKeyCode; // Key to move backward
    int _rotateLeftKeyCode; // Key to rotate left
    int _rotateRightKeyCode; // Key to rotate right
};