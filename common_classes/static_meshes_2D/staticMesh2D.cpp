// GLM
#include <glm/glm.hpp>

// Project
#include "staticMesh2D.h"

namespace static_meshes_2D {

const int StaticMesh2D::POSITION_ATTRIBUTE_INDEX           = 0;
const int StaticMesh2D::TEXTURE_COORDINATE_ATTRIBUTE_INDEX = 1;

StaticMesh2D::StaticMesh2D(bool withPositions, bool withTextureCoordinates)
    : _hasPositions(withPositions)
    , _hasTextureCoordinates(withTextureCoordinates) {}

StaticMesh2D::~StaticMesh2D()
{
    deleteMesh();
}

void StaticMesh2D::deleteMesh()
{
    if (!_isInitialized) {
        return;
    }

    glDeleteVertexArrays(1, &_vao);
    _vbo.deleteVBO();

    _isInitialized = false;
}

bool StaticMesh2D::hasPositions() const
{
    return _hasPositions;
}

bool StaticMesh2D::hasTextureCoordinates() const
{
    return _hasTextureCoordinates;
}

int StaticMesh2D::getVertexByteSize() const
{
    int result = 0;
    if (hasPositions()) {
        result += sizeof(glm::vec3);
    }
    if (hasTextureCoordinates()) {
        result += sizeof(glm::vec2);
    }

    return result;
}

void StaticMesh2D::setVertexAttributesPointers(int numVertices)
{
    uint64_t offset = 0;
    if (hasPositions())
    {
        glEnableVertexAttribArray(POSITION_ATTRIBUTE_INDEX);
        glVertexAttribPointer(POSITION_ATTRIBUTE_INDEX, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), reinterpret_cast<void*>(offset));

        offset += sizeof(glm::vec2)*numVertices;
    }

    if (hasTextureCoordinates())
    {
        glEnableVertexAttribArray(TEXTURE_COORDINATE_ATTRIBUTE_INDEX);
        glVertexAttribPointer(TEXTURE_COORDINATE_ATTRIBUTE_INDEX, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), reinterpret_cast<void*>(offset));

        offset += sizeof(glm::vec2)*numVertices;
    }
}

} // namespace static_meshes_2D