#include "staticMeshIndexed3D.h"

namespace static_meshes_3D {

StaticMeshIndexed3D::StaticMeshIndexed3D(bool withPositions, bool withTextureCoordinates, bool withNormals)
	: StaticMesh3D(withPositions, withTextureCoordinates, withNormals) {}

StaticMeshIndexed3D::~StaticMeshIndexed3D()
{
	if (_isInitialized) {
		_indicesVBO.deleteVBO();
	}
}

} // namespace static_meshes_3D