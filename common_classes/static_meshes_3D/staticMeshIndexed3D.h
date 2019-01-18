#pragma once

#include "staticMesh3D.h"

namespace static_meshes_3D {

/**
	Represents generic 3D static mesh rendered with indexed rendering.
*/
class StaticMeshIndexed3D : public StaticMesh3D
{
public:
	StaticMeshIndexed3D(bool withPositions, bool withTextureCoordinates, bool withNormals);
	virtual ~StaticMeshIndexed3D();

protected:
	VertexBufferObject _indicesVBO; //!< Our VBO wrapper class holding indices data
};

}; // namespace static_meshes_3D