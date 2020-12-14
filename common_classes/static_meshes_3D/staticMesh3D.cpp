// GLM
#include <glm/glm.hpp>

// Project
#include "staticMesh3D.h"

namespace static_meshes_3D {

const int StaticMesh3D::POSITION_ATTRIBUTE_INDEX           = 0;
const int StaticMesh3D::TEXTURE_COORDINATE_ATTRIBUTE_INDEX = 1;
const int StaticMesh3D::NORMAL_ATTRIBUTE_INDEX             = 2;

StaticMesh3D::StaticMesh3D(bool withPositions, bool withTextureCoordinates, bool withNormals)
    : _hasPositions(withPositions)
    , _hasTextureCoordinates(withTextureCoordinates)
    , _hasNormals(withNormals) {}

StaticMesh3D::~StaticMesh3D()
{
    deleteMesh();
}

void StaticMesh3D::deleteMesh()
{
    if (!_isInitialized) {
        return;
    }

    glDeleteVertexArrays(1, &_vao);
    _vbo.deleteVBO();

    _isInitialized = false;
}

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

void StaticMesh3D::setVertexAttributesPointers(int numVertices)
{
    uint64_t offset = 0;
    if (hasPositions())
    {
        glEnableVertexAttribArray(POSITION_ATTRIBUTE_INDEX);
        glVertexAttribPointer(POSITION_ATTRIBUTE_INDEX, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), reinterpret_cast<void*>(offset));

        offset += sizeof(glm::vec3)*numVertices;
    }

    if (hasTextureCoordinates())
    {
        glEnableVertexAttribArray(TEXTURE_COORDINATE_ATTRIBUTE_INDEX);
        glVertexAttribPointer(TEXTURE_COORDINATE_ATTRIBUTE_INDEX, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), reinterpret_cast<void*>(offset));

        offset += sizeof(glm::vec2)*numVertices;
    }

    if (hasNormals())
    {
        glEnableVertexAttribArray(NORMAL_ATTRIBUTE_INDEX);
        glVertexAttribPointer(NORMAL_ATTRIBUTE_INDEX, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), reinterpret_cast<void*>(offset));

        offset += sizeof(glm::vec3)*numVertices;
    }
}

} // namespace static_meshes_3D