#include "static_mesh_3D.h"

#include <glm/glm.hpp>

namespace static_meshes {

const int StaticMesh3D::POSITION_ATTRIBUTE_INDEX           = 0;
const int StaticMesh3D::TEXTURE_COORDINATE_ATTRIBUTE_INDEX = 1;
const int StaticMesh3D::NORMAL_ATTRIBUTE_INDEX             = 2;

StaticMesh3D::StaticMesh3D(bool withPositions, bool withTextureCoordinates, bool withNormals)
	: _hasPositions(withPositions)
	, _hasTextureCoordinates(withTextureCoordinates)
	, _hasNormals(withNormals) {}

bool StaticMesh3D::hasPositions() const
{
	return _hasPositions;
}

bool StaticMesh3D::hasTextureCoordinates() const
{
	return _hasTextureCoordinates;
}

bool StaticMesh3D::hasNormals() const
{
	return _hasNormals;
}

int StaticMesh3D::getVertexByteSize() const
{
	int result = 0;
	if (hasPositions()) {
		result += sizeof(glm::vec3);
	}
	if (hasTextureCoordinates()) {
		result += sizeof(glm::vec2);
	}
	if (hasNormals()) {
		result += sizeof(glm::vec3);
	}

	return result;
}

} // namespace static_meshes