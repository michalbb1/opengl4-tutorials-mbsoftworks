// Project
#include "staticMeshIndexed3D.h"

namespace static_meshes_3D {

StaticMeshIndexed3D::StaticMeshIndexed3D(bool withPositions, bool withTextureCoordinates, bool withNormals)
    : StaticMesh3D(withPositions, withTextureCoordinates, withNormals) {}

StaticMeshIndexed3D::~StaticMeshIndexed3D()
{
    if (_isInitialized) {
        // It's enough to delete indices VBO here, rest of stuff is destructed in super destructor
        _indicesVBO.deleteVBO();
    }
}

void StaticMeshIndexed3D::deleteMesh()
{
    if (_isInitialized) {
        _indicesVBO.deleteVBO();
        StaticMesh3D::deleteMesh();
    }
}

} // namespace static_meshes_3D