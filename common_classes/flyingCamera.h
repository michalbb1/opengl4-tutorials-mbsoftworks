#pragma once

// STL
#include <functional>

// GLM
#include <glm/glm.hpp>

/**
 * Implements a camera, that can fly around the world freely.
 * It cannot rotate more than 90 degrees up / down (so it's not sutiable for space flight simulators).
 */
class FlyingCamera
{
public:
    FlyingCamera(const glm::vec3& position, const glm::vec3& viewPoint, const glm::vec3& upVector, float moveSpeed = 10.0f, float mouseSensitivity = 0.15f);

    /**
     * Sets movement speed of camera (how fast camera moves forward / backward and strafes left / right).
     * 
     * @param moveSpeed  Movement speed (distance travelled per second)
     */
    void setMoveSpeed(float moveSpeed);

    /**
     * Sets mouse sensitivity, or the speed, with which you rotate view with mouse.
     *
     * @param mouseSensitivity  Sensitivity in degrees per pixel - how many degrees does camera turn for each pixel moved
     */
    void setMouseSensitivity(float mouseSensitivity);

    /**
     * Sets keys to control camera with.
     *
     * @param forwardKeyCode      Key code to move forward
     * @param backwardKeyCode     Key code to move backward
     * @param strafeLeftKeyCode   Key code to strafe left
     * @param strafeRightKeyCode  Key code to strafe right
     */
    void setControls(int forwardKeyCode, int backwardKeyCode, int strafeLeftKeyCode, int strafeRightKeyCode);

    /**
     * Sets center of window position, in pixels.
     * This is required for camera rotation, that resets cursor position constantly.
     *
     * @param windowCenterPosition  Center of the window
     */
    void setWindowCenterPosition(const glm::i32vec2& windowCenterPosition);

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
	 * Gets normalized view vector of the camera.
	 */
	glm::vec3 getNormalizedViewVector() const;

    /**
     * Updates camera - reacts on key presses and updates camera's internal state (position, view vector...).
     *
     * @param keyInputFunc         Function that detects key presses
     * @param getCursorPosFunc     Function that retrieves current cursor position
     * @param setCursorPosFunc     Function that sets current cursor position
     * @param speedCorrectionFunc  Function that corrects floating point value according to the time passed
     */
    void update(const std::function<bool(int)>& keyInputFunc,
        const std::function<glm::i32vec2()>& getCursorPosFunc,
        const std::function<void(const glm::i32vec2&)>& setCursorPosFunc,
        const std::function<float(float)>& speedCorrectionFunc);

private:
    /**
     * Moves camera by specified distance (positive value = move forward, negative value = move backward).
     *
     * @param distance  Distance to move by
     */
    void moveBy(float distance);

    /**
     * Strafes camera by specified distance (positive value = strafe right, negative value = strafe left).
     *
     * @param distance Distance to strafe by
     */
    void strafeBy(float distance);

    /**
     * Rotates camera view by specified angle to the left or right side.
     *
     * @param angleInDegrees Angle to rotate by (in degrees)
     */
    void rotateLeftRight(float angleInDegrees);

    /**
     * Rotates camera view by specified angle up or down.
     *
     * @param angleInDegrees Angle to rotate by (in degrees)
     */
    void rotateUpDown(float angleInDegrees);

    glm::vec3 _position; // Camera's position (eye position)
    glm::vec3 _viewPoint; // Viewpoint - where does camera look to
    glm::vec3 _upVector; // Up vector of the camera

    glm::i32vec2 _windowCenterPosition; // Center of the window (to restore cursor position every frame to)
    float _mouseSensitivity; // Mouse sensitivity in degrees per pixel

    float _moveSpeed; // Speed of camera (both for going forward / backward and for strafing left / right)
    int _forwardKeyCode; // Key code to move forward
    int _backwardKeyCode; // Key code to move backward
    int _strafeLeftKeyCode; // Key code to rotate left
    int _strafeRightKeyCode; // Key code to rotate right
};