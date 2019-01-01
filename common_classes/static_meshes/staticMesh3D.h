#pragma once

#include "../vertexBufferObject.h"

namespace static_meshes {

class StaticMesh3D
{
public:
	StaticMesh3D(bool withPositions, bool withTextureCoordinates, bool withNormals);

	static const int POSITION_ATTRIBUTE_INDEX;
	static const int TEXTURE_COORDINATE_ATTRIBUTE_INDEX;
	static const int NORMAL_ATTRIBUTE_INDEX;

	virtual void render() const = 0;
	void deleteMesh();

	bool hasPositions() const;
	bool hasTextureCoordinates() const;
	bool hasNormals() const;
	int getVertexByteSize() const;

protected:
	bool _hasPositions = false;
	bool _hasTextureCoordinates = false;
	bool _hasNormals = false;

	bool _isInitialized = false;
	GLuint _vao = 0;
	VertexBufferObject _vbo;

	virtual void initializeData() = 0;
	void setVertexAttributesPointers(int numVertices);
};

}; // namespace static_meshes