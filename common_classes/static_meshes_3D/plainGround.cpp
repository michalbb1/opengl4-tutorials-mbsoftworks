// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Project
#include "plainGround.h"

#include "../shaderProgramManager.h"

namespace static_meshes_3D {

glm::vec3 PlainGround::vertices[4] =
{
    glm::vec3(-200.0f, 0.0f, -200.0f), // Left-back point
    glm::vec3(-200.0f, 0.0f, 200.0f), // Left-front point
    glm::vec3(200.0f, 0.0f, -200.0f), // Right-back point
    glm::vec3(200.0f, 0.0f, 200.0f) // Right-front point
};

glm::vec2 PlainGround::textureCoordinates[4] =
{
    glm::vec2(0.0f, 20.0f),
    glm::vec2(0.0f, 0.0f),
    glm::vec2(20.0f, 20.0f),
    glm::vec2(20.0f, 0.0f)
};

PlainGround::PlainGround(bool withPositions, bool withTextureCoordinates, bool withNormals) : StaticMesh3D(withPositions, withTextureCoordinates, withNormals)
{
    initializeData();
}

void PlainGround::render() const
{
    if (!_isInitialized) {
        return;
    }

    glBindVertexArray(_vao);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void PlainGround::initializeData()
{
    if (_isInitialized) {
        return;
    }

    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);

    const auto numVertices = 4;
    const auto vertexByteSize = getVertexByteSize();
    _vbo.createVBO(vertexByteSize * numVertices);
    _vbo.bindVBO();

    if (hasPositions()) {
        _vbo.addRawData(vertices, sizeof(glm::vec3)*numVertices);
    }

    if (hasTextureCoordinates()) {
        _vbo.addRawData(textureCoordinates, sizeof(glm::vec2)*numVertices);
    }

    if (hasNormals()) {
        glm::vec3 normal(0.0f, 1.0f, 0.0f);
        _vbo.addRawData(&normal, sizeof(glm::vec3), numVertices);
    }

    _vbo.uploadDataToGPU(GL_STATIC_DRAW);
    setVertexAttributesPointers(numVertices);
    _isInitialized = true;
}

} // namespace static_meshes_3D