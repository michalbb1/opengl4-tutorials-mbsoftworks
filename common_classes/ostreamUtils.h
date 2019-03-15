#pragma once

#include <iostream>
#include <iomanip>
#include <glm/glm.hpp>

namespace ostream_utils {

std::ostream &operator<<(std::ostream &o, const glm::vec3 vec3)
{
	return o << std::setprecision(3) << "[" << vec3.x << ", " << vec3.y << ", " << vec3.z << "]";
}

} // namespace ostream_utils