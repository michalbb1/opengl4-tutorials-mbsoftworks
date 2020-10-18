#pragma once

// GLAD
#include <glad/glad.h>

// Project
#include "shaderStruct.h"

namespace shader_structs {

struct PointLight : ShaderStruct
{
    PointLight(const glm::vec3& position, const glm::vec3& color, const float ambientFactor,
        const float constantAttenuation, const float linearAttenuation, const float exponentialAttenuation,
        const bool isOn = true);

	//* \brief Sets point light in a shader program. */
	void setUniform(ShaderProgram& shaderProgram, const std::string& uniformName) const override;

    //* \brief Gets data size of the structure (in bytes) according to std140 layout rules */
    static GLsizeiptr getDataSizeStd140();
    void* getDataPointer() const override;

	//* \brief Returns a point light that is off and does not contribute at all. */
	static const PointLight& none();

    glm::vec3 position; //!< Position of the point light
    float __DUMMY_PADDING0__; //!< This is just needed because of std140 layout padding rules

	glm::vec3 color; //!< Color of the point light
	float ambientFactor; //!< Ambient factor (how much this light contributes to the global lighting in any case)

    float constantAttenuation; //!< Constant attenuation factor of light with rising distance
    float linearAttenuation; //!< Lienar attenuation factor of light with rising distance
    float exponentialAttenuation; //!< Constant attenuation factor of light with rising distance
    GLint isOn; //!< Flag telling, if the light is on
};

} // namespace shader_structs