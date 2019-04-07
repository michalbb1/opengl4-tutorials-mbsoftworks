#include "pyramid.h"

#include <glm/glm.hpp>
#include "torus.h"
#include <string>

namespace static_meshes_3D {

Torus::Torus(int stacks, int slices, float radius, float tubeRadius, bool withPositions, bool withTextureCoordinates, bool withNormals)
	: StaticMeshIndexed3D(withPositions, withTextureCoordinates, withNormals)
	, _mainSegments(stacks)
	, _tubeSegments(slices)
	, _mainRadius(radius)
	, _tubeRadius(tubeRadius)
{
	initializeData();
}

void Torus::initializeData()
{
	if (_isInitialized) {
		return;
	}

	// Calculate and cache counts of vertices and indices
	_numVertices = (_mainSegments+1)*(_tubeSegments+1);
	_primitiveRestartIndex = _numVertices;
	_numIndices = (_mainSegments * 2 * (_tubeSegments + 1)) + _mainSegments - 1;

	// Generate VAO and VBOs for vertex attributes and indices
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);
	_vbo.createVBO(getVertexByteSize() * _numVertices);
	_indicesVBO.createVBO(sizeof(GLuint)*_numIndices);

	// Precalculate steps in radians for main segment and tube segment
	auto mainSegmentAngleStep = glm::radians(360.0f / float(_mainSegments));
	auto tubeSegmentAngleStep = glm::radians(360.0f / float(_tubeSegments));

	if (hasPositions())
	{
		auto currentMainSegmentAngle = 0.0f;
		for (auto i = 0; i <= _mainSegments; i++)
		{
			// Calculate sine and cosine of main segment angle
			auto sinMainSegment = sin(currentMainSegmentAngle);
			auto cosMainSegment = cos(currentMainSegmentAngle);
			auto currentTubeSegmentAngle = 0.0f;
			for (auto j = 0; j <= _tubeSegments; j++)
			{
				// Calculate sine and cosine of tube segment angle
				auto sinTubeSegment = sin(currentTubeSegmentAngle);
				auto cosTubeSegment = cos(currentTubeSegmentAngle);

				// Calculate vertex position on the surface of torus
				auto surfacePosition = glm::vec3(
					(_mainRadius + _tubeRadius * cosTubeSegment)*cosMainSegment,
					(_mainRadius + _tubeRadius * cosTubeSegment)*sinMainSegment,
					_tubeRadius*sinTubeSegment);
				
				_vbo.addData(&surfacePosition, sizeof(glm::vec3));

				// Update current tube angle
				currentTubeSegmentAngle += tubeSegmentAngleStep;
			}

			// Update main segment angle
			currentMainSegmentAngle += mainSegmentAngleStep;
		}
	}

	if (hasTextureCoordinates())
	{
		// Precalculate steps in texture coordinates for main segment and tube segment
		// I have decided to map the texture twice around main segments and once around tube segmens
		auto mainSegmentTextureStep = 2.0f  / float(_mainSegments);
		auto tubeSegmentTextureStep = 1.0f / float(_tubeSegments);

		auto currentMainSegmentTexCoordV = 0.0f;
		for (auto i = 0; i <= _mainSegments; i++)
		{
			auto currentTubeSegmentTexCoordU = 0.0f;
			for (auto j = 0; j <= _tubeSegments; j++)
			{
				// Calculate texture coordinate and add it to the buffer
				auto textureCoordinate = glm::vec2(currentTubeSegmentTexCoordU, currentMainSegmentTexCoordV);
				_vbo.addData(&textureCoordinate, sizeof(glm::vec2));
				// Update texture coordinate of tube segment
				currentTubeSegmentTexCoordU += tubeSegmentTextureStep;
			}

			// Update texture coordinate of main segment
			currentMainSegmentTexCoordV += mainSegmentTextureStep;
		}
	}

	if (hasNormals())
	{
		auto currentMainSegmentAngle = 0.0f;
		for (auto i = 0; i <= _mainSegments; i++)
		{
			// Calculate sine and cosine of main segment angle
			auto sinMainSegment = sin(currentMainSegmentAngle);
			auto cosMainSegment = cos(currentMainSegmentAngle);
			auto currentTubeSegmentAngle = 0.0f;
			for (auto j = 0; j <= _tubeSegments; j++)
			{
				// Calculate sine and cosine of tube segment angle
				auto sinTubeSegment = sin(currentTubeSegmentAngle);
				auto cosTubeSegment = cos(currentTubeSegmentAngle);

				auto normal = glm::vec3(
					cosMainSegment*cosTubeSegment,
					sinMainSegment*cosTubeSegment,
					sinTubeSegment
				);
				_vbo.addData(&normal, sizeof(glm::vec3));

				// Update current tube angle
				currentTubeSegmentAngle += tubeSegmentAngleStep;
			}

			// Update main segment angle
			currentMainSegmentAngle += mainSegmentAngleStep;
		}
	}

	// Finally, generate indices for rendering
	GLuint currentVertexOffset = 0;
	for (auto i = 0; i < _mainSegments; i++)
	{
		for (auto j = 0; j <= _tubeSegments; j++)
		{
			GLuint vertexIndexA = currentVertexOffset;
			_indicesVBO.addData(&vertexIndexA, sizeof(GLuint));
			GLuint vertexIndexB = currentVertexOffset + _tubeSegments + 1;
			_indicesVBO.addData(&vertexIndexB, sizeof(GLuint));
			currentVertexOffset++;
		}

		// Don't restart primitive, if it's last segment, rendering ends here anyway
		if (i != _mainSegments - 1) {
			_indicesVBO.addData(&_primitiveRestartIndex, sizeof(GLuint));
		}
	}

	_vbo.bindVBO();
	_vbo.uploadDataToGPU(GL_STATIC_DRAW);
	setVertexAttributesPointers(_numVertices);

	_indicesVBO.bindVBO(GL_ELEMENT_ARRAY_BUFFER);
	_indicesVBO.uploadDataToGPU(GL_STATIC_DRAW);

	_isInitialized = true;
}

void Torus::render() const
{
	if (!_isInitialized) {
		return;
	}

	glBindVertexArray(_vao);
	// Enable primitive restart, because we're rendering several triangle strips (for each main segment)
	glEnable(GL_PRIMITIVE_RESTART);
	glPrimitiveRestartIndex(_primitiveRestartIndex);

	// Render torus using precalculated indices
	glDrawElements(GL_TRIANGLE_STRIP, _numIndices, GL_UNSIGNED_INT, 0);

	// Disable primitive restart, we won't need it now
	glDisable(GL_PRIMITIVE_RESTART);
}

void Torus::renderPoints() const
{
	if (!_isInitialized) {
		return;
	}

	glBindVertexArray(_vao);

	// Render torus points only
	glDrawArrays(GL_POINTS, 0, _numVertices);
}

float Torus::getMainRadius() const
{
	return _mainRadius;
}

float Torus::getTubeRadius() const
{
	return _tubeRadius;
}

} // namespace static_meshes_3D