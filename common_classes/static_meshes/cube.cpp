#include "cube.h"

#include <glm/glm.hpp>

namespace static_meshes {

glm::vec3 Cube::vertices[36] =
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

glm::vec2 Cube::textureCoordinates[6] =
{
	glm::vec2(0.0f, 1.0f), glm::vec2(1.0f, 1.0f), glm::vec2(1.0f, 0.0f),
	glm::vec2(1.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 1.0f)
};

glm::vec3 Cube::normals[6] =
{
	glm::vec3(0.0f, 0.0f, 1.0f), // Front face
	glm::vec3(0.0f, 0.0f, -1.0f), // Back face
	glm::vec3(-1.0f, 0.0f, 0.0f), // Left face
	glm::vec3(1.0f, 0.0f, 0.0f), // Right face
	glm::vec3(0.0f, 1.0f, 0.0f), // Top face
	glm::vec3(0.0f, -1.0f, 0.0f), // Bottom face
};

Cube::Cube(bool withPositions, bool withTextureCoordinates, bool withNormals)
	: StaticMesh3D(withPositions, withTextureCoordinates, withNormals)
{
	initializeData();
}

void Cube::render()
{
	glBindVertexArray(_vao);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void Cube::release()
{
}

void Cube::initializeData()
{
	glCreateVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	const int numVertices = 36;
	int vertexByteSize = getVertexByteSize();	
	_vbo.createVBO(vertexByteSize * numVertices);
	_vbo.bindVBO();
	
	if (hasPositions())
	{
		for (auto i = 0; i < numVertices; i++)
		{
			_vbo.addData(&vertices[i], sizeof(glm::vec3));
		}
	}

	if (hasTextureCoordinates())
	{
		for (auto i = 0; i < numVertices; i++)
		{
			_vbo.addData(&textureCoordinates[i % 6], sizeof(glm::vec2));
		}
	}

	if (hasNormals())
	{
		for (auto i = 0; i < numVertices; i++)
		{
			_vbo.addData(&normals[i % 6], sizeof(glm::vec3));
		}
	}

	_vbo.uploadDataToGPU(GL_STATIC_DRAW);

	uint32_t offset = 0;
	if (hasPositions())
	{
		glEnableVertexAttribArray(POSITION_ATTRIBUTE_INDEX);
		glVertexAttribPointer(POSITION_ATTRIBUTE_INDEX, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)offset);

		offset += sizeof(glm::vec3)*numVertices;
	}

	if (hasTextureCoordinates())
	{
		glEnableVertexAttribArray(TEXTURE_COORDINATE_ATTRIBUTE_INDEX);
		glVertexAttribPointer(TEXTURE_COORDINATE_ATTRIBUTE_INDEX, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)offset);

		offset += sizeof(glm::vec2)*numVertices;
	}

	if (hasNormals())
	{
		glEnableVertexAttribArray(NORMAL_ATTRIBUTE_INDEX);
		glVertexAttribPointer(NORMAL_ATTRIBUTE_INDEX, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)offset);

		offset += sizeof(glm::vec3)*numVertices;
	}
}

} // namespace static_meshes