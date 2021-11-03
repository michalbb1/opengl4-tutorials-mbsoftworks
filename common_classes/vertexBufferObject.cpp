// STL
#include <iostream>
#include <cstring>

// Project
#include "vertexBufferObject.h"

void VertexBufferObject::createVBO(size_t reserveSizeBytes)
{
    if (isBufferCreated_)
    {
        std::cerr << "This buffer is already created! You need to delete it before re-creating it!" << std::endl;
        return;
    }

    glGenBuffers(1, &bufferID_);
    rawData_.reserve(reserveSizeBytes > 0 ? reserveSizeBytes : 1024);

    std::cout << "Created vertex buffer object with ID " << bufferID_ << " and initial reserved size " << rawData_.capacity() << " bytes" << std::endl;
    isBufferCreated_ = true;
}

void VertexBufferObject::bindVBO(GLenum bufferType)
{
    if (!isBufferCreated_)
    {
        std::cerr << "This buffer is not created yet! You cannot bind it before you create it!" << std::endl;
        return;
    }

    bufferType_ = bufferType;
    glBindBuffer(bufferType_, bufferID_);
}

void VertexBufferObject::addRawData(const void* ptrData, size_t dataSize, int repeat)
{
    const auto bytesToAdd = dataSize * repeat;
    const auto requiredCapacity = bytesAdded_ + bytesToAdd;
    if (requiredCapacity > rawData_.capacity())
    {
        auto newCapacity = rawData_.capacity() * 2;
        while (newCapacity < requiredCapacity) {
            newCapacity *= 2;
        }

        std::vector<unsigned char> newRawData;
        newRawData.reserve(newCapacity);
        memcpy(newRawData.data(), rawData_.data(), bytesAdded_);
        rawData_ = std::move(newRawData);
    }

    for (int i = 0; i < repeat; i++)
    {
        memcpy(rawData_.data() + bytesAdded_, ptrData, dataSize);
        bytesAdded_ += dataSize;
    }
}

void* VertexBufferObject::getRawDataPointer()
{
    return rawData_.data();
}

void VertexBufferObject::uploadDataToGPU(GLenum usageHint)
{
    if (!isBufferCreated_)
    {
        std::cerr << "This buffer is not created yet! Call createVBO before uploading data to GPU!" << std::endl;
        return;
    }

    glBufferData(bufferType_, bytesAdded_, rawData_.data(), usageHint);
    isDataUploaded_ = true;
    uploadedDataSize_ = bytesAdded_;
    bytesAdded_ = 0;
}

void* VertexBufferObject::mapBufferToMemory(GLenum usageHint) const
{
    if (!isDataUploaded_) {
        return nullptr;
    }

    return glMapBuffer(bufferType_, usageHint);
}

void* VertexBufferObject::mapSubBufferToMemory(GLenum usageHint, size_t offset, size_t length) const
{
    if (!isDataUploaded_) {
        return nullptr;
    }

    return glMapBufferRange(bufferType_, offset, length, usageHint);
}

void VertexBufferObject::unmapBuffer() const
{
    glUnmapBuffer(bufferType_);
}

GLuint VertexBufferObject::getBufferID() const
{
    return bufferID_;
}

size_t VertexBufferObject::getBufferSize()
{
    return isDataUploaded_ ? uploadedDataSize_ : bytesAdded_;
}

void VertexBufferObject::deleteVBO()
{
    if (!isBufferCreated_) {
        return;
    }

    std::cout << "Deleting vertex buffer object with ID " << bufferID_ << "..." << std::endl;
    glDeleteBuffers(1, &bufferID_);
    isDataUploaded_ = false;
    isBufferCreated_ = false;
}
