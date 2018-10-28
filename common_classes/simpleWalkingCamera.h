#pragma once

#include <functional>

#include <glm/glm.hpp>

class SimpleWalkingCamera
{
public:
	SimpleWalkingCamera(const glm::vec3& position, const glm::vec3& viewPoint, const glm::vec3& upVector, float moveSpeed = 10.0f, float rotationSpeed = 135.0f);

	/** \brief  Sets movement speed of camera (how fast camera moves forward / backward).
	*   \param moveSpeed Movement speed (distance travelled per second)
	*/
	void setMoveSpeed(float moveSpeed);

	/** \brief  Sets rottation speed of camera (how fast camera rotates).
	*   \param degreesPerSecond How much degrees per second should camera be rotating
	*/
	void setRotationSpeed(float degreesPerSecond);

	/** \brief  Sets keys to control camera with
	*   \param forwardKeyCode     Key to move forward
	*   \param backwardKeyCode    Key to move backward
	*   \param rotateLeftKeyCode  Key to rotate left
	*   \param rotateRightKeyCode Key to rotate right
	*/
	void setControls(int forwardKeyCode, int backwardKeyCode, int rotateLeftKeyCode, int rotateRightKeyCode);

	/** \brief  Gets the current view matrix, depending on position and viewpoint of camera.
	*   \return 4x4 view matrix.
	*/
	glm::mat4 getViewMatrix() const;

	/** \brief  Updates camera - reacts on key presses and updates camera's internal state (position, view vector...)
	*   \param keyInputFunc        Function that detects key presses
	*   \param speedCorrectionFunc Function, that corrects floating point value according to the time passed
	*/
	void update(std::function<bool(int)> keyInputFunc, std::function<float(float)> speedCorrectionFunc);

private:

	/** \brief  Moves camera by specified distance (positive = forward, negative = backward)
	*   \param distance Distance to travel by
	*/
	void moveBy(float distance);

	/** \brief  Rotates camera view by specified angle.
	*   \param angleInDegrees Angle to rotate by, in degrees
	*/
	void rotateBy(float angleInDegrees);

	/** \brief  Gets normalized view vector.
	*   \return Normalized view vector.
	*/
	glm::vec3 getNormalizedViewVector() const;

	glm::vec3 _position; ///< Camera's position (eye position)
	glm::vec3 _viewPoint; ///< Viewpoint - where does camera look to
	glm::vec3 _upVector; ///< Up vector of the camera

	float _moveSpeed; ///< How fast the camera is moving forwards and backwards
	float _rotationSpeed; ///< How fast the camera rotates around Y-axis (in degrees per second)
	int _forwardKeyCode; ///< Key to move forward
	int _backwardKeyCode; ///< Key to move backward
	int _rotateLeftKeyCode; ///< Key to rotate left
	int _rotateRightKeyCode; ///< Key to rotate right
};