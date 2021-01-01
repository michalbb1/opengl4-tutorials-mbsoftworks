// GLM
#include <glm/glm.hpp>

// Project
#include "quad.h"

namespace static_meshes_2D {

glm::vec2 Quad::vertices[4] =
{
    glm::vec2(0.0f, 1.0f), // Top-left point
    glm::vec2(0.0f, 0.0f), // Bottom-left point
    glm::vec2(1.0f, 1.0f), // Top-right point
    glm::vec2(1.0f, 0.0f) // Bottom-right point
};

Quad::Quad(bool withPositions, bool withTextureCoordinates)
    : StaticMesh2D(withPositions, withTextureCoordinates)
{
    initializeData();
}

void Quad::render() const
{
    if (!_isInitialized) {
        return;
    }

    glBindVertexArray(_vao);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void Quad::initializeData()
{
    if (_isInitialized) {
        return;
    }

    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);

    const int numVertices = 4;
    int vertexByteSize = getVertexByteSize();	
    _vbo.createVBO(vertexByteSize * numVertices);
    _vbo.bindVBO();
    
    if (hasPositions())
    {
        _vbo.addRawData(vertices, sizeof(glm::vec2)*numVertices);
    }

    if (hasTextureCoordinates())
    {
        _vbo.addRawData(vertices, sizeof(glm::vec2)*numVertices);
    }

    _vbo.uploadDataToGPU(GL_STATIC_DRAW);
    setVertexAttributesPointers(numVertices);
    _isInitialized = true;
}

} // namespace static_meshes_2D