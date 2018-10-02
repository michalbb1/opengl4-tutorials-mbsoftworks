#include "staticGeometry.h"

namespace static_geometry
{
	glm::vec3 cubeVertices[36] =
	{
		// Front face
		glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(-0.5f, 0.5f, 0.5f),
		// Back face
		glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.5f, 0.5f, -0.5f),
		// Left face
		glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-0.5f, 0.5f, -0.5f),
		// Right face
		glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f),
		// Top face
		glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(-0.5f, 0.5f, -0.5f),
		// Bottom face
		glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-0.5f, -0.5f, 0.5f),
	};

	glm::vec3 cubeFaceColors[6] =
	{
		glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)
	};

	glm::vec3 pyramidVertices[12] =
	{
		// Front face
		glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0.5f, -0.5f, 0.5f),
		// Back face
		glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(-0.5f, -0.5f, -0.5f),
		// Left face
		glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-0.5f, -0.5f, 0.5f),
		// Right face
		glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.5f, -0.5f, -0.5f)
	};

	glm::vec3 pyramidFaceColors[3] =
	{
		glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.25f, 0.0f, 0.5f), glm::vec3(0.0f, 1.0f, 0.0f)
	};
}
