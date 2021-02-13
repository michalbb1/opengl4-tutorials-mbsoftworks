// STL
#include <iostream>

// Project
#include "uniformBufferObject.h"

UniformBufferObject::~UniformBufferObject()
{
    deleteUBO();
}

void UniformBufferObject::createUBO(const size_t byteSize, GLenum usageHint)
{
    if (_isBufferCreated)
    {
        std::cerr << "This buffer is already created! You need to delete it before re-creating it!" << std::endl;
        return;
    }

    // Generate buffer ID, bind it immediately and reserve space for it
    glGenBuffers(1, &_bufferID);
    glBindBuffer(GL_UNIFORM_BUFFER, _bufferID);
    glBufferData(GL_UNIFORM_BUFFER, byteSize, NULL, usageHint);

    // Mark that the buffer has been created and store its size
    _isBufferCreated = true;
    _byteSize = byteSize;
}

void UniformBufferObject::bindUBO() const
{
    if (!_isBufferCreated)
    {
        std::cerr << "Uniform buffer object is not created yet! You cannot bind it before you create it!" << std::endl;
        return;
    }

    glBindBuffer(GL_UNIFORM_BUFFER, _bufferID);
}

void UniformBufferObject::setBufferData(const size_t offset, const void* ptrData, const size_t dataSize)
{
    if (!_isBufferCreated)
    {
        std::cerr << "Could not set data because uniform buffer object is not created yet!" << std::endl;
        return;
    }

    if (offset >= _byteSize)
    {
        std::cerr << "Tried to set data of uniform buffer object at offset " << offset << ", but it's beyond buffer size " << _byteSize << ", will be ignored..." << std::endl;
        return;
    }

    if (offset + dataSize > _byteSize)
    {
        std::cerr << "Could not set data because it would overflow the buffer! Offset: " << offset << ", data size: " << dataSize << std::endl;
        return;
    }

    glBufferSubData(GL_UNIFORM_BUFFER, offset, dataSize, ptrData);
}

void UniformBufferObject::bindBufferBaseToBindingPoint(const GLuint bindingPoint) const
{
    if (!_isBufferCreated)
    {
        std::cerr << "Could not bind buffer base to binding point " << bindingPoint << ", because uniform buffer object is not created yet!" << std::endl;
        return;
    }

    glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, _bufferID);
}

GLuint UniformBufferObject::getBufferID() const
{
    return _bufferID;
}

void UniformBufferObject::deleteUBO()
{
    if (!_isBufferCreated) {
        return;
    }

    std::cout << "Deleting uniform buffer object with ID " << _bufferID << "..." << std::endl;
    glDeleteBuffers(1, &_bufferID);
    _isBufferCreated = false;
}
