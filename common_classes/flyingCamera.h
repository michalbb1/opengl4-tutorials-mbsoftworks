#pragma once

#include <functional>

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

class FlyingCamera
{
public:
	FlyingCamera(const glm::vec3& position, const glm::vec3& viewPoint, const glm::vec3& upVector, float moveSpeed = 10.0f, float mouseSensitivity = 0.15f);

	/** \brief  Sets movement speed of camera (how fast camera moves forward / backward and strafes left / right).
	*   \param moveSpeed Movement speed (distance travelled per second)
	*/
	void setMoveSpeed(float moveSpeed);

	/** \brief  Sets mouse sensitivity, or the speed, with which you rotate view with mouse.
	*   \param mouseSensitivity Sensitivity, in degrees per pixel - how miuch degrees do you turn when you move mouse by one pixel
	*/
	void setMouseSensitivity(float mouseSensitivity);

	/** \brief  Sets keys to control camera with
	*   \param forwardKeyCode     Key to move forward
	*   \param backwardKeyCode    Key to move backward
	*   \param strafeLeftKeyCode  Key to strafe left
	*   \param strafeRightKeyCode Key to strafe right
	*/
	void setControls(int forwardKeyCode, int backwardKeyCode, int strafeLeftKeyCode, int strafeRightKeyCode);

	/** \brief  Sets center of window position, in pixels. This is required for camera rotation, that resets cursor position constantly.
	*   \param windowCenterPosition Center of the window
	*/
	void setWindowCenterPosition(const glm::i32vec2& windowCenterPosition);

	/** \brief  Gets the current view matrix, depending on position and viewpoint of camera.
	*   \return 4x4 view matrix.
	*/
	glm::mat4 getViewMatrix() const;

	/** \brief  Gets the current eye position (camera's position).
	*   \return Eye position.
	*/
	glm::vec3 getEye() const;

	/** \brief  Gets the point camera looks to.
	*   \return Viewpoint position.
	*/
	glm::vec3 getViewPoint() const;
	
	/** \brief  Gets the up vector of the camera.
	*   \return Up vector of the camera.
	*/
	glm::vec3 getUpVector() const;

	/** \brief  Updates camera - reacts on key presses and updates camera's internal state (position, view vector...)
	*   \param keyInputFunc        Function that detects key presses
	*   \param getCursorPosFunc    Function that retrieves current cursor position
	*   \param setCursorPosFunc    Function that sets current cursor position
	*   \param speedCorrectionFunc Function, that corrects floating point value according to the time passed
	*/
	void update(const std::function<bool(int)>& keyInputFunc,
		const std::function<glm::i32vec2()>& getCursorPosFunc,
		const std::function<void(const glm::i32vec2&)>& setCursorPosFunc,
		const std::function<float(float)>& speedCorrectionFunc);

private:

	/** \brief  Moves camera by specified distance (positive = forward, negative = backward)
	*   \param distance Distance to travel by
	*/
	void moveBy(float distance);

	/** \brief  Strafes camera by specified distance (positive = right, negative = left)
	*   \param distance Distance to strafe by
	*/
	void strafeBy(float distance);

	/** \brief  Rotates camera view by specified angle to the left or right side.
	*   \param angleInDegrees Angle to rotate by, in degrees
	*/
	void rotateLeftRight(float angleInDegrees);

	/** \brief  Rotates camera view by specified angle up or down.
	*   \param angleInDegrees Angle to rotate by, in degrees
	*/
	void rotateUpDown(float angleInDegrees);

	/** \brief  Gets normalized view vector.
	*   \return Normalized view vector.
	*/
	glm::vec3 getNormalizedViewVector() const;

	glm::vec3 _position; //!< Camera's position (eye position)
	glm::vec3 _viewPoint; //!< Viewpoint - where does camera look to
	glm::vec3 _upVector; //!< Up vector of the camera

	glm::i32vec2 _windowCenterPosition; //!< Center of the window (to restore cursor position every frame to)
	float _mouseSensitivity; //!< Mouse sensitivity in degrees per pixel

	float _moveSpeed; //!< How fast the camera is moving forwards and backwards
	int _forwardKeyCode; //!< Key to move forward
	int _backwardKeyCode; //!< Key to move backward
	int _strafeLeftKeyCode; //!< Key to rotate left
	int _strafeRightKeyCode; //!< Key to rotate right
};