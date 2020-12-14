#pragma once

// GLM
#include <glm/glm.hpp>

/**
 * Singleton class that manages and keeps track of most commonly used matrices in the program.
 */
class MatrixManager
{
public:
    /**
     * Gets the one and only instance of the matrix manager.
     */
    static MatrixManager& getInstance();

    /**
     * Gets projection matrix.
     */
    const glm::mat4& getProjectionMatrix() const;

    /**
     * Gets orthographic projection matrix.
     */
    const glm::mat4& getOrthoProjectionMatrix() const;

    /**
     * Gets view matrix.
     */
    const glm::mat4& getViewMatrix() const;

    /**
     * Sets projection matrix.
     *
     * @param projectionMatrix  New projection matrix
     */
    void setProjectionMatrix(const glm::mat4& projectionMatrix);

    /**
     * Sets orthographic projection matrix.
     *
     * @param orthoProjectionMatrix  New orthographic projection matrix
     */
    void setOrthoProjectionMatrix(const glm::mat4& orthoProjectionMatrix);

    /**
     * Sets view matrix.
     *
     * @param viewMatrix  New view matrix
     */
    void setViewMatrix(const glm::mat4& viewMatrix);

private:
    MatrixManager() {} // Private constructor to make class singleton
    MatrixManager(const MatrixManager&) = delete; // No copy constructor allowed
    void operator=(const MatrixManager&) = delete; // No copy assignment allowed

    glm::mat4 _projectionMatrix; // Current projection matrix
    glm::mat4 _orthoProjectionMatrix; // Current orthographic projection matrix
    glm::mat4 _viewMatrix; // Current view matrix
};