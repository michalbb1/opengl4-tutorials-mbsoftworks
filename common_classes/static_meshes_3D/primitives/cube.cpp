// GLM
#include <glm/glm.hpp>

// Project
#include "cube.h"

namespace static_meshes_3D {

glm::vec3 Cube::vertices[36] =
{
    // Front face
    glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(-0.5f, 0.5f, 0.5f),
    // Back face
    glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.5f, 0.5f, -0.5f),
    // Left face
    glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-0.5f, 0.5f, -0.5f),
    // Right face
    glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f),
    // Top face
    glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(-0.5f, 0.5f, -0.5f),
    // Bottom face
    glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-0.5f, -0.5f, 0.5f),
};

glm::vec2 Cube::textureCoordinates[6] =
{
    glm::vec2(0.0f, 1.0f), glm::vec2(1.0f, 1.0f), glm::vec2(1.0f, 0.0f),
    glm::vec2(1.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 1.0f)
};

glm::vec3 Cube::normals[6] =
{
    glm::vec3(0.0f, 0.0f, 1.0f), // Front face
    glm::vec3(0.0f, 0.0f, -1.0f), // Back face
    glm::vec3(-1.0f, 0.0f, 0.0f), // Left face
    glm::vec3(1.0f, 0.0f, 0.0f), // Right face
    glm::vec3(0.0f, 1.0f, 0.0f), // Top face
    glm::vec3(0.0f, -1.0f, 0.0f), // Bottom face
};

Cube::Cube(bool withPositions, bool withTextureCoordinates, bool withNormals)
    : StaticMesh3D(withPositions, withTextureCoordinates, withNormals)
{
    initializeData();
}

void Cube::render() const
{
    if (!_isInitialized) {
        return;
    }

    glBindVertexArray(_vao);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void Cube::renderPoints() const
{
    if (!_isInitialized) {
        return;
    }

    glBindVertexArray(_vao);
    glDrawArrays(GL_POINTS, 0, 36);
}

void Cube::renderFaces(int facesBitmask) const
{
    if (!_isInitialized) {
        return;
    }

    glBindVertexArray(_vao);

    if (facesBitmask & CUBE_FRONT_FACE) {
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
    if (facesBitmask & CUBE_BACK_FACE) {
        glDrawArrays(GL_TRIANGLES, 6, 6);
    }
    if (facesBitmask & CUBE_LEFT_FACE) {
        glDrawArrays(GL_TRIANGLES, 12, 6);
    }
    if (facesBitmask & CUBE_RIGHT_FACE) {
        glDrawArrays(GL_TRIANGLES, 18, 6);
    }
    if (facesBitmask & CUBE_TOP_FACE) {
        glDrawArrays(GL_TRIANGLES, 24, 6);
    }
    if (facesBitmask & CUBE_BOTTOM_FACE) {  
        glDrawArrays(GL_TRIANGLES, 30, 6);
    }
}

void Cube::initializeData()
{
    if (_isInitialized) {
        return;
    }

    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);

    const auto numVertices = 36;
    const auto vertexByteSize = getVertexByteSize();
    _vbo.createVBO(vertexByteSize * numVertices);
    _vbo.bindVBO();
    
    if (hasPositions())
    {
        _vbo.addRawData(vertices, sizeof(glm::vec3)*numVertices);
    }

    if (hasTextureCoordinates())
    {
        for (auto i = 0; i < 6; i++)
        {
            _vbo.addRawData(textureCoordinates, sizeof(glm::vec2)*6);
        }
    }

    if (hasNormals())
    {
        for (auto i = 0; i < 6; i++)
        {
            _vbo.addRawData(&normals[i], sizeof(glm::vec3), 6);
        }
    }

    _vbo.uploadDataToGPU(GL_STATIC_DRAW);
    setVertexAttributesPointers(numVertices);
    _isInitialized = true;
}

} // namespace static_meshes_3D