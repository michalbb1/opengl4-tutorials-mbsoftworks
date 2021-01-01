#pragma once

// Project
#include "../vertexBufferObject.h"

namespace static_meshes_2D {

/**
 * Represents generic 2D static mesh.
 */
class StaticMesh2D
{
public:
    static const int POSITION_ATTRIBUTE_INDEX; // Vertex attribute index of vertex position (0)
    static const int TEXTURE_COORDINATE_ATTRIBUTE_INDEX; // Vertex attribute index of texture coordinate (1)

    StaticMesh2D(bool withPositions, bool withTextureCoordinates);
    virtual ~StaticMesh2D();

    /**
     * Renders static mesh.
     */
    virtual void render() const = 0;

    /**
     * Deletes static mesh data.
     */
    void deleteMesh();

    /**
     * Checks, if static mesh has vertex positions.
     */
    bool hasPositions() const;

    /**
     * Checks, if static mesh has texture coordinates.
     */
    bool hasTextureCoordinates() const;

    /**
     * Gets byte size of one vertex (depending on present vertex attributes).
     */
    int getVertexByteSize() const;

protected:
    bool _hasPositions = false; // Flag telling, if we have vertex positions
    bool _hasTextureCoordinates = false; // Flag telling, if we have texture coordinates

    bool _isInitialized = false; // Is mesh initialized flag
    GLuint _vao = 0; // VAO ID from OpenGL
    VertexBufferObject _vbo; // Our VBO wrapper class

    /**
     * Initializes vertex data.
     */
    virtual void initializeData() = 0;

    /**
    * Sets vertex attribute pointers in a standard way.
    *
    * @param numVertices  Number of vertices present in the buffer
    */
    void setVertexAttributesPointers(int numVertices);
};

}; // namespace static_meshes_2D