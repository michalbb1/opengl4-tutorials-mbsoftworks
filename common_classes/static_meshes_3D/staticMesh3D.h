#pragma once

// Project
#include "../vertexBufferObject.h"

namespace static_meshes_3D {

/**
 * Represents generic 3D static mesh.
 */
class StaticMesh3D
{
public:
	static const int POSITION_ATTRIBUTE_INDEX; // Vertex attribute index of vertex position (0)
	static const int TEXTURE_COORDINATE_ATTRIBUTE_INDEX; // Vertex attribute index of texture coordinate (1)
	static const int NORMAL_ATTRIBUTE_INDEX; // Vertex attribute index of vertex normal (2)

	StaticMesh3D(bool withPositions, bool withTextureCoordinates, bool withNormals);
	virtual ~StaticMesh3D();

	/**
	 * Renders static mesh.
	 */
	virtual void render() const = 0;

	/**
	 * Renders static mesh as points only. Default implementation does nothing,
	 * because different meshes have different logic for rendering points).
	 */
	virtual void renderPoints() const {}

	/**
	 * Deletes static mesh data.
	 */
	virtual void deleteMesh();

	/**
	 * Checks, if static mesh has vertex positions.
	 */
	bool hasPositions() const;

	/**
	 * Checks, if static mesh has texture coordinates.
	 */
	bool hasTextureCoordinates() const;

	/**
	 * Checks, if static mesh has vertex normals.
	 */
	bool hasNormals() const;

	/**
	 * Gets byte size of one vertex (depending on present vertex attributes).
	 */
	int getVertexByteSize() const;

protected:
	bool _hasPositions = false; // Flag telling, if we have vertex positions
	bool _hasTextureCoordinates = false; // Flag telling, if we have texture coordinates
	bool _hasNormals = false; // Flag telling, if we have vertex normals

	bool _isInitialized = false; // Is mesh initialized flag
	GLuint _vao = 0; // VAO ID from OpenGL
	VertexBufferObject _vbo; // Our VBO wrapper class holding static mesh data

	/**
	 * Initializes vertex data. Default implementation does nothing as its not needed for all classes
	 */
	virtual void initializeData() {}

	/**
	* Sets vertex attribute pointers in a standard way.
	*
	* @param numVertices  Number of vertices present in the buffer
	*/
	void setVertexAttributesPointers(int numVertices);
};

}; // namespace static_meshes_3D