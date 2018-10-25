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

	// Render using triangle strip!
	glm::vec3 plainGroundVertices[4] =
	{
		glm::vec3(-200.0f, 0.0f, -200.0f), // Left-back point
		glm::vec3(-200.0f, 0.0f, 200.0f), // Left-front point
		glm::vec3(200.0f, 0.0f, -200.0f), // Right-back point
		glm::vec3(200.0f, 0.0f, 200.0f) // Right-front point
	};

	glm::vec3 plainGroundColors[4] =
	{
		glm::vec3(0.0f, 0.5f, 0.0f),
		glm::vec3(0.0f, 0.85f, 0.0f),
		glm::vec3(0.0f, 0.35f, 0.25f),
		glm::vec3(0.0f, 0.8f, 0.2f)
	};
}
