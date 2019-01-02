#pragma once

#include "../vertexBufferObject.h"

namespace static_meshes_2D {

/**
	Represents generic 2D static mesh.
*/
class StaticMesh2D
{
public:
	StaticMesh2D(bool withPositions, bool withTextureCoordinates);
	~StaticMesh2D();

	static const int POSITION_ATTRIBUTE_INDEX; //!< Vertex attribute index of vertex position (0)
	static const int TEXTURE_COORDINATE_ATTRIBUTE_INDEX; //!< Vertex attribute index of texture coordinate (1)

	/** \brief  Renders static mesh. */
	virtual void render() const = 0;

	/** \brief  Deletes static mesh data. */
	void deleteMesh();

	/** \brief  Checks, if static mesh has vertex positions.
	*   \return True if it has or false otherwise.
	*/
	bool hasPositions() const;

	/** \brief  Checks, if static mesh has texture coordinates.
	*   \return True if it has or false otherwise.
	*/
	bool hasTextureCoordinates() const;

	/** \brief  Checks, if static mesh has vertex normals.
	*   \return True if it has or false otherwise.
	*/
	int getVertexByteSize() const;

protected:
	bool _hasPositions = false; //!< Flag telling, if we have vertex positions
	bool _hasTextureCoordinates = false; //!< Flag telling, if we have texture coordinates

	bool _isInitialized = false; //!< Is mesh initialized flag
	GLuint _vao = 0; //!< VAO ID from OpenGL
	VertexBufferObject _vbo; //!< Our VBO wrapper class

	/** \brief  Initializes vertex data. */
	virtual void initializeData() = 0;

	/** \brief  Sets vertex attribute pointers in a standard way. */
	void setVertexAttributesPointers(int numVertices);
};

}; // namespace static_meshes_2D