// STL
#include <iostream>
#include <cstring>

// Project
#include "vertexBufferObject.h"

void VertexBufferObject::createVBO(size_t reserveSizeBytes)
{
    if (_isBufferCreated)
    {
        std::cerr << "This buffer is already created! You need to delete it before re-creating it!" << std::endl;
        return;
    }

    glGenBuffers(1, &_bufferID);
    _rawData.reserve(reserveSizeBytes > 0 ? reserveSizeBytes : 1024);

    std::cout << "Created vertex buffer object with ID " << _bufferID << " and initial reserved size " << _rawData.capacity() << " bytes" << std::endl;
    _isBufferCreated = true;
}

void VertexBufferObject::bindVBO(GLenum bufferType)
{
    if (!_isBufferCreated)
    {
        std::cerr << "This buffer is not created yet! You cannot bind it before you create it!" << std::endl;
        return;
    }

    _bufferType = bufferType;
    glBindBuffer(_bufferType, _bufferID);
}

void VertexBufferObject::addRawData(const void* ptrData, size_t dataSize, int repeat)
{
    const auto bytesToAdd = dataSize * repeat;
    if (_bytesAdded + bytesToAdd > _rawData.capacity())
    {
        const auto newCapacity = _rawData.capacity() * 2;
        std::vector<unsigned char> newRawData;
        newRawData.reserve(newCapacity);
        memcpy(newRawData.data(), _rawData.data(), _bytesAdded);
        _rawData = std::move(newRawData);
    }

    for (int i = 0; i < repeat; i++)
    {
        memcpy(_rawData.data() + _bytesAdded, ptrData, dataSize);
        _bytesAdded += dataSize;
    }
}

void* VertexBufferObject::getRawDataPointer()
{
    return _rawData.data();
}

void VertexBufferObject::uploadDataToGPU(GLenum usageHint)
{
    if (!_isBufferCreated)
    {
        std::cerr << "This buffer is not created yet! Call createVBO before uploading data to GPU!" << std::endl;
        return;
    }

    glBufferData(_bufferType, _bytesAdded, _rawData.data(), usageHint);
    _isDataUploaded = true;
    _uploadedDataSize = _bytesAdded;
    _bytesAdded = 0;
}

void* VertexBufferObject::mapBufferToMemory(GLenum usageHint)
{
    if (!_isDataUploaded)
    {
        return nullptr;
    }

    return glMapBuffer(_bufferType, usageHint);
}

void* VertexBufferObject::mapSubBufferToMemory(GLenum usageHint, size_t offset, size_t length)
{
    if (!_isDataUploaded)
    {
        return nullptr;
    }

    return glMapBufferRange(_bufferType, offset, length, usageHint);
}

void VertexBufferObject::unmapBuffer()
{
    glUnmapBuffer(_bufferType);
}

GLuint VertexBufferObject::getBufferID()
{
    return _bufferID;
}

size_t VertexBufferObject::getBufferSize()
{
    return _isDataUploaded ? _uploadedDataSize : _bytesAdded;
}

void VertexBufferObject::deleteVBO()
{
    if (!_isBufferCreated) {
        return;
    }

    std::cout << "Deleting vertex buffer object with ID " << _bufferID << "..." << std::endl;
    glDeleteBuffers(1, &_bufferID);
    _isDataUploaded = false;
    _isBufferCreated = false;
}
