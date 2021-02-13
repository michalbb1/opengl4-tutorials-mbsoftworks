// STL
#include <algorithm>
#include <stdexcept>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// Project
#include "sphere.h"

namespace static_meshes_3D {

Sphere::Sphere(float radius, int numSlices, int numStacks, bool withPositions, bool withTextureCoordinates, bool withNormals)
    : StaticMeshIndexed3D(withPositions, withTextureCoordinates, withNormals)
    , _radius(radius)
    , _numSlices(numSlices)
    , _numStacks(numStacks)
{
    // Validate input by throwing an exception - such spheres don't even make any sense
    if (numSlices < 3 || numStacks < 2) {
        throw std::runtime_error("Sphere cannot have less than 3 slices and less than 2 stacks!");
    }

    initializeData();
}

void Sphere::render() const
{
    if (!_isInitialized) {
        return;
    }

    glBindVertexArray(_vao);

    glEnable(GL_PRIMITIVE_RESTART);
    glPrimitiveRestartIndex(_primitiveRestartIndex);
    
    // Render north pole
    glDrawElements(GL_TRIANGLES, _numPoleIndices, GL_UNSIGNED_INT, (void*)(sizeof(GLuint)*_northPoleIndexOffset));
    
    // Render body
    glDrawElements(GL_TRIANGLE_STRIP, _numBodyIndices, GL_UNSIGNED_INT, (void*)(sizeof(GLuint)*_bodyIndexOffset));

    // Render south pole 
    glDrawElements(GL_TRIANGLES, _numPoleIndices, GL_UNSIGNED_INT, (void*)(sizeof(GLuint)*_southPoleIndexOffset));

    // Disable primitive restart, we won't need it now
    glDisable(GL_PRIMITIVE_RESTART);
}

void Sphere::renderPoints() const
{
    if (!_isInitialized) {
        return;
    }

    glBindVertexArray(_vao);
    glDrawArrays(GL_POINTS, 0, _numVertices);
}

float Sphere::getRadius() const
{
    return _radius;
}

int Sphere::getNumSlices() const
{
    return _numSlices;
}

int Sphere::getNumStacks() const
{
    return _numStacks;
}

void Sphere::initializeData()
{
    if (_isInitialized) {
        return;
    }

    // Cache count of vertices
    _numVertices = (_numStacks + 1) * (_numSlices + 1);
    
    // Cache number of indices it takes to render body
    const auto numBodyStacks = _numStacks - 2;
    const auto numBodyPrimitiveRestarts = std::max(0, numBodyStacks - 1);
    _numBodyIndices = 2 * numBodyStacks * (_numSlices + 1) + numBodyPrimitiveRestarts;

    // Cache count of pole indices
    _numPoleIndices = _numSlices * 3;

    // Cache offsets for sphere parts
    _northPoleIndexOffset = 0;
    _bodyIndexOffset = _numPoleIndices;
    _southPoleIndexOffset = _bodyIndexOffset + _numBodyIndices;

    // Finally cache total number of indices and primitive restart index
    _numIndices = 2 * _numPoleIndices + _numBodyIndices;
    _primitiveRestartIndex = _numVertices;

    // Generate VAO and VBOs for vertex attributes and indices
    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);
    _vbo.createVBO(getVertexByteSize() * _numVertices);
    _indicesVBO.createVBO(sizeof(GLuint) * _numIndices);

    // Pre-calculate sines / cosines for given number of slices
    const auto sliceAngleStep = 2.0f * glm::pi<float>() / static_cast<float>(_numSlices);
    auto currentSliceAngle = 0.0f;
    std::vector<float> sliceSines, sliceCosines;
    for (auto i = 0; i <= _numSlices; i++)
    {
        sliceSines.push_back(sin(currentSliceAngle));
        sliceCosines.push_back(cos(currentSliceAngle));

        // Update slice angle
        currentSliceAngle += sliceAngleStep;
    }

    // Pre-calculate sines / cosines for given number of stacks
    const auto stackAngleStep = -glm::pi<float>() / static_cast<float>(_numStacks);
    auto currentStackAngle = glm::pi<float>() / 2.0f;
    std::vector<float> stackSines, stackCosines;
    for (auto i = 0; i <= _numStacks; i++)
    {
        stackSines.push_back(sin(currentStackAngle));
        stackCosines.push_back(cos(currentStackAngle));

        // Update stack angle
        currentStackAngle += stackAngleStep;
    }

    // Generate sphere vertex positions
    if (hasPositions())
    {
        for (auto i = 0; i <= _numStacks; i++)
        {
            for (auto j = 0; j <= _numSlices; j++)
            {
                const auto x = _radius * stackCosines[i] * sliceCosines[j];
                const auto y = _radius * stackSines[i];
                const auto z = _radius * stackCosines[i] * sliceSines[j];
                _vbo.addData(glm::vec3(x, y, z));
            }
        }
    }

    // Generate sphere texture coordinates
      if (hasTextureCoordinates())
    {
        for (auto i = 0; i <= _numStacks; i++)
        {
            for (auto j = 0; j <= _numSlices; j++)
            {
                const auto x = stackCosines[i] * sliceCosines[j];
                const auto y = stackSines[i];
                const auto z = stackCosines[i] * sliceSines[j];

                // There are many options out there to generate sphere texture coordinates
                // I have commented out some others here that work, some better, some worse
                // You can try them all out :)

                // float u = atan2(x, z) / (2.0f * glm::pi<float>());
                // float v = asin(y) / glm::pi<float>();

                // float u = 0.5f + asin(x) / glm::pi<float>();
                // float v = 0.5f + asin(y) / glm::pi<float>();

                // float u = 0.5f + x * 0.5f;
                // float v = 0.5f + y * 0.5f;

                const auto u = 1.0f - static_cast<float>(j) / _numSlices;
                const auto v = 1.0f - static_cast<float>(i) / _numStacks;
                _vbo.addData(glm::vec2(u, v));
            }
        }
    }

    // Generate sphere normals
    if (hasNormals())
    {
        for (auto i = 0; i <= _numStacks; i++)
        {
            for (auto j = 0; j <= _numSlices; j++)
            {
                const auto x = stackCosines[i] * sliceCosines[j];
                const auto y = stackSines[i];
                const auto z = stackCosines[i] * sliceSines[j];
                _vbo.addData(glm::vec3(x, y, z));
            }
        }
    }

    // Now that we have all vertex data, generate indices for north pole (triangles)
    for (auto i = 0; i < _numSlices; i++)
    {
        GLuint sliceIndex = i;
        GLuint nextSliceIndex = sliceIndex + _numSlices + 1;
        _indicesVBO.addData(static_cast<GLuint>(sliceIndex));
        _indicesVBO.addData(static_cast<GLuint>(nextSliceIndex));
        _indicesVBO.addData(static_cast<GLuint>(nextSliceIndex+1));
    }

    // Then for body (triangle strip)
    GLuint currentVertexIndex = _numSlices + 1;
    for (auto i = 0; i < numBodyStacks; i++)
    {
        // Primitive restart triangle strip from second body stack on
        if (i > 0)
        {
            _indicesVBO.addData(_primitiveRestartIndex);
        }

        for (auto j = 0; j <= _numSlices; j++)
        {
            GLuint sliceIndex = currentVertexIndex + j;
            GLuint nextSliceIndex = currentVertexIndex + _numSlices + 1 + j;
            _indicesVBO.addData(sliceIndex);
            _indicesVBO.addData(nextSliceIndex);
        }

        currentVertexIndex += _numSlices+1;
    }

    // And finally south pole (triangles again)
    GLuint beforeLastStackIndexOffset = _numVertices - 2*(_numSlices + 1);
    for (auto i = 0; i < _numSlices; i++)
    {
        GLuint sliceIndex = beforeLastStackIndexOffset + i;
        GLuint nextSliceIndex = sliceIndex + _numSlices + 1;
        _indicesVBO.addData(static_cast<GLuint>(sliceIndex));
        _indicesVBO.addData(static_cast<GLuint>(sliceIndex + 1));
        _indicesVBO.addData(static_cast<GLuint>(nextSliceIndex));
    }
    
    _vbo.bindVBO();
    _vbo.uploadDataToGPU(GL_STATIC_DRAW);
    setVertexAttributesPointers(_numVertices);

    _indicesVBO.bindVBO(GL_ELEMENT_ARRAY_BUFFER);
    _indicesVBO.uploadDataToGPU(GL_STATIC_DRAW);

    _isInitialized = true;
}

} // namespace static_meshes_3D