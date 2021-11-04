// STL
#include <iostream>
#include <cstring>

// Project
#include "vertexBufferObject.h"

void VertexBufferObject::createVBO(size_t reserveSizeBytes)
{
    if (isBufferCreated())
    {
        std::cerr << "This buffer is already created! You need to delete it before re-creating it!" << std::endl;
        return;
    }

    glGenBuffers(1, &bufferID_);
    rawData_.reserve(reserveSizeBytes > 0 ? reserveSizeBytes : 1024);
    std::cout << "Created vertex buffer object with ID " << bufferID_ << " and initial reserved size " << rawData_.capacity() << " bytes" << std::endl;
}

void VertexBufferObject::bindVBO(GLenum bufferType)
{
    if (!isBufferCreated())
    {
        std::cerr << "This buffer is not created yet! You cannot bind it before you create it!" << std::endl;
        return;
    }

    bufferType_ = bufferType;
    glBindBuffer(bufferType_, bufferID_);
}

void VertexBufferObject::addRawData(const void* ptrData, size_t dataSizeBytes, size_t repeat)
{
    const auto bytesToAdd = dataSizeBytes * repeat;
    const auto requiredCapacity = bytesAdded_ + bytesToAdd;

    // If the current capacity isn't sufficient, let's resize the internal vector of raw data
    if (requiredCapacity > rawData_.capacity())
    {
        // Determine new raw data buffer capacity - enlarge by a factor of two until it becomes big enough
        auto newCapacity = rawData_.capacity() * 2;
        while (newCapacity < requiredCapacity) {
            newCapacity *= 2;
        }

        // Reserve new vector with higher capacity (which will replace member rawData_) and copy existing data over
        std::vector<unsigned char> newRawData;
        newRawData.reserve(newCapacity);
        memcpy(newRawData.data(), rawData_.data(), bytesAdded_);
        rawData_ = std::move(newRawData);
    }

    // Add the data now that we are sure that capacity is sufficient
    for (size_t i = 0; i < repeat; i++)
    {
        memcpy(rawData_.data() + bytesAdded_, ptrData, dataSizeBytes);
        bytesAdded_ += dataSizeBytes;
    }
}

void* VertexBufferObject::getRawDataPointer()
{
    return rawData_.data();
}

void VertexBufferObject::uploadDataToGPU(GLenum usageHint)
{
    if (!isBufferCreated())
    {
        std::cerr << "This buffer is not created yet! Call createVBO before uploading data to GPU!" << std::endl;
        return;
    }

    glBufferData(bufferType_, bytesAdded_, rawData_.data(), usageHint);
    uploadedDataSize_ = bytesAdded_;
    bytesAdded_ = 0;
}

void* VertexBufferObject::mapBufferToMemory(GLenum usageHint) const
{
    if (!isDataUploaded()) {
        return nullptr;
    }

    return glMapBuffer(bufferType_, usageHint);
}

void* VertexBufferObject::mapSubBufferToMemory(GLenum usageHint, size_t offset, size_t length) const
{
    if (!isDataUploaded()) {
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
    return isDataUploaded() ? uploadedDataSize_ : bytesAdded_;
}

void VertexBufferObject::deleteVBO()
{
    if (!isBufferCreated()) {
        return;
    }

    std::cout << "Deleting vertex buffer object with ID " << bufferID_ << "..." << std::endl;
    glDeleteBuffers(1, &bufferID_);
    bufferID_ = 0;
    bytesAdded_ = 0;
    uploadedDataSize_ = 0;
}

bool VertexBufferObject::isBufferCreated() const
{
    return bufferID_ != 0;
}

bool VertexBufferObject::isDataUploaded() const
{
    return uploadedDataSize_ > 0;
}
