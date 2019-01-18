#include "pyramid.h"

#include <glm/glm.hpp>
#include "torus.h"

namespace static_meshes_3D {


void Torus::initializeData()
{
	if (_isInitialized) {
		return;
	}

	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	const int numVertices = _stacks * _slices;;
	int vertexByteSize = getVertexByteSize();
	_vbo.createVBO(vertexByteSize * numVertices);
	_vbo.bindVBO();
	_indicesVBO.createVBO(sizeof(GLint)*numVertices);
	_indicesVBO.bindVBO(GL_ELEMENT_ARRAY_BUFFER);
	
	if (hasPositions())
	{
		std::vector<glm::vec3> vertices;
		float stackAngleStep = glm::radians(360.0f / float(_stacks));
		float sliceAngleStep = glm::radians(360.0f / float(_slices));
		float currentStackAngle = 0.0f;
		float currentSliceAngle = 0.0f;
		for (auto i = 0; i < _stacks; i++)
		{
			currentStackAngle += stackAngleStep;
		}

		_vbo.addData(vertices.data(), sizeof(glm::vec3)*numVertices);
	}

	if (hasTextureCoordinates())
	{
		for (auto i = 0; i < 4; i++)
		{
		//	_vbo.addData(textureCoordinates, sizeof(glm::vec2)*3);
		}
	}

	if (hasNormals())
	{
		for (auto i = 0; i < 4; i++)
		{
			/*
			const auto& posA = vertices[i * 3];
			const auto& posB = vertices[i * 3 + 1];
			const auto& posC = vertices[i * 3 + 2];
			const auto vecA = posB - posA;
			const auto vecB = posC - posA;
			auto cp = glm::normalize(glm::cross(vecA, vecB));
			_vbo.addData(&cp, sizeof(glm::vec3), 3);
			*/
		}
	}

	_vbo.uploadDataToGPU(GL_STATIC_DRAW);
	setVertexAttributesPointers(numVertices);

	_isInitialized = true;
}

Torus::Torus(int stacks, int slices, bool withPositions, bool withTextureCoordinates, bool withNormals)
	: StaticMeshIndexed3D(withPositions, withTextureCoordinates, withNormals)
	, _stacks(stacks)
	, _slices(slices)
{
	initializeData();
}

void Torus::render() const
{
}

} // namespace static_meshes_3D