#pragma once

#include <glm/glm.hpp>

/**
  Manages all the commonly needed matrices.
*/
class MatrixManager
{
public:
	MatrixManager(const MatrixManager&) = delete;
	void operator=(const MatrixManager&) = delete;

	/** \brief  Gets the one and only instance of the matrix manager.
	*   \return Matrix manager instance.
	*/
	static MatrixManager& getInstance();

	/** \brief  Gets projection matrix.
	*   \return Projection matrix.
	*/
	const glm::mat4& getProjectionMatrix() const;

	/** \brief  Gets orthographic projection matrix.
	*   \return Orthographic projection matrix.
	*/
	const glm::mat4& getOrthoProjectionMatrix() const;

	/** \brief  Gets view matrix.
	*   \return View matrix.
	*/
	const glm::mat4& getViewMatrix() const;

	/** \brief  Sets projection matrix.
	*   \param  projectionMatrix New projection matrix
	*/
	void setProjectionMatrix(const glm::mat4& projectionMatrix);

	/** \brief  Sets orthographic projection matrix.
	*   \param  orthoProjectionMatrix New orthographic projection matrix
	*/
	void setOrthoProjectionMatrix(const glm::mat4& orthoProjectionMatrix);

	/** \brief  Sets view matrix.
	*   \param  viewMatrix New view matrix
	*/
	void setViewMatrix(const glm::mat4& viewMatrix);

private:
	MatrixManager() {}

	glm::mat4 _projectionMatrix; //!< Current projection matrix
	glm::mat4 _orthoProjectionMatrix; //!< Current orthographic projection matrix
	glm::mat4 _viewMatrix; //!< Current view matrix
};