// STL
#include <glm/gtc/matrix_transform.hpp>

// Project
#include "orbitCamera.h"

OrbitCamera::OrbitCamera(const glm::vec3& center, const glm::vec3& upVector, float radius, float minRadius, float polarAngle, float azimuthAngle)
	: center_(center)
	, upVector_(upVector)
	, radius_(radius)
    , _minRadius(minRadius)
	, polarAngle_(polarAngle)
	, azimuthAngle_(azimuthAngle)
{
}

void OrbitCamera::rotatePolar(const float radians)
{
    polarAngle_ += radians;

    // Keep polar angle within range <0..2PI) - it's not necessary, just to have it nicely output
    const auto fullCircle = 2.0f*glm::pi<float>();
    polarAngle_ = fmodf(polarAngle_, fullCircle);
    if (polarAngle_ < 0.0f) {
        polarAngle_ = fullCircle + polarAngle_;
    }
}

void OrbitCamera::rotateAzimuth(const float radians)
{
    azimuthAngle_ += radians;

    // Check if the angle hasn't exceeded quarter of a circle to prevent flip, add a bit of epsilon like 0.001 radians
    const auto azimuthCap = glm::pi<float>() / 2.0f - 0.001f;
    if (azimuthAngle_ > azimuthCap) {
        azimuthAngle_ = azimuthCap;
    }

    if (azimuthAngle_ < -azimuthCap) {
        azimuthAngle_ = -azimuthCap;
    }
}

void OrbitCamera::zoom(const float by)
{
    radius_ -= by;
    if (radius_ < _minRadius) {
        radius_ = _minRadius;
    }
}

void OrbitCamera::moveHorizontal(const float by)
{
    const auto position = getEye();
    glm::vec3 viewVector = glm::normalize(center_ - position);
    glm::vec3 strafeVector = glm::normalize(glm::cross(viewVector, upVector_));
    center_ += strafeVector * by;
}

void OrbitCamera::moveVertical(const float by)
{
    center_ += upVector_ * by;
}

glm::mat4 OrbitCamera::getViewMatrix() const
{
	// https://stackoverflow.com/questions/19740463/lookat-function-im-going-crazy
	const auto position = getEye();

    const auto f = getNormalizedViewVector();
    auto u = glm::normalize(upVector_);
    const auto s = glm::normalize(cross(f, u));
	u = cross(s, f);

	glm::mat4 resultViewMatrix(1.0f);
	resultViewMatrix[0][0] = s.x;
	resultViewMatrix[1][0] = s.y;
	resultViewMatrix[2][0] = s.z;
	resultViewMatrix[0][1] = u.x;
	resultViewMatrix[1][1] = u.y;
	resultViewMatrix[2][1] = u.z;
	resultViewMatrix[0][2] = -f.x;
	resultViewMatrix[1][2] = -f.y;
	resultViewMatrix[2][2] = -f.z;
	resultViewMatrix[3][0] = -glm::dot(s, position);
	resultViewMatrix[3][1] = -glm::dot(u, position);
	resultViewMatrix[3][2] = glm::dot(f, position);
	return resultViewMatrix;
}

glm::vec3 OrbitCamera::getEye() const
{
    // Calculate sines / cosines of angles
    const auto sinePolar = sin(polarAngle_);
    const auto cosinePolar = cos(polarAngle_);
    const auto sineAzimuth = sin(azimuthAngle_);
    const auto cosineAzimuth = cos(azimuthAngle_);

    // Calculate eye position out of them
    const auto t = radius_ * cosineAzimuth;
    const auto y = center_.y + radius_ * sineAzimuth;
    const auto x = center_.x + t * cosinePolar;
    const auto z = center_.z + t * sinePolar;

    return glm::vec3(x, y, z);
}

glm::vec3 OrbitCamera::getViewPoint() const
{
    return center_;
}

glm::vec3 OrbitCamera::getUpVector() const
{
    return upVector_;
}

glm::vec3 OrbitCamera::getNormalizedViewVector() const
{
    return glm::normalize(getViewPoint() - getEye());
}

float OrbitCamera::getPolarAngle() const
{
    return polarAngle_;
}

float OrbitCamera::getAzimuthAngle() const
{
    return azimuthAngle_;
}

float OrbitCamera::getRadius() const
{
    return radius_;
}
