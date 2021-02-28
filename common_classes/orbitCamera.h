#pragma once

// GLM
#include <glm/glm.hpp>

/**
 * Implementation of orbit camera using spherical coordinates.
 */
class OrbitCamera
{
public:
	OrbitCamera(const glm::vec3& center, const glm::vec3& upVector, float radius, float minRadius, float azimuthAngle, float polarAngle);

    /**
     * Rotates camera around Y axis. This is done by changing the azimuth angle of orbit camera sphere.
     *
     * @param radians Amount to rotate by in radians
     */
    void rotateAzimuth(const float radians);

    /**
     * Rotates camera around horizontal axis. This is done by changing the polar angle of orbit camera sphere.
     *
     * @param radians Amount to rotate by in radians
     */
    void rotatePolar(const float radians);

    /**
     * Zooms camera by a certain amount. This is done by changing radius of orbit camera sphere.
     * However, radius cannot go below the minRadius threshold, not to "zoom through".
     *
     * @param distance Amount to zoom by
     */
	void zoom(const float distance);

    /**
     * Moves camera horizontally by a given distance.
     *
     * @param distance Distance to move by horizontally
     */
	void moveHorizontal(const float distance);

    /**
     * Moves camera vertically by a given distance.
     *
     * @param distance Distance to move by vertically
     */
	void moveVertical(const float distance);
    
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
     * Gets azimuth angle of the camera (angle around Y axis).
     */
    float getAzimuthAngle() const;

    /**
     * Gets polar angle of the camera (angle around horizontal axis).
     */
    float getPolarAngle() const;

    /**
     * Gets current radius of orbit camera sphere, in other words, controls the zoom of the camera.
     */
    float getRadius() const;

private:
    glm::vec3 center_; // Center of the orbit camera sphere (the point upon which the camera looks)
    glm::vec3 upVector_; // Up vector of the camera
    float radius_; // Radius of the orbit camera sphere
    float _minRadius; // Minimal radius of the orbit camera sphere (cannot fall below this value)
    float azimuthAngle_; // Azimuth angle on the orbit camera sphere
    float polarAngle_; // Polar angle on the orbit camera sphere
};