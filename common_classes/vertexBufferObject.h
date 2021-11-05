#pragma once

// STL
#include <vector>

// GLAD
#include <glad/glad.h>

/**
 * Wraps OpenGL's vertex buffer object to a convenient higher level class.
 */
class VertexBufferObject
{
public:
    /**
     * Creates a new VBO, with optional reserved buffer size.
     *
     * @param reserveSizeBytes  Buffer size reservation, in bytes (so that no memory allocations happen while preparing buffer data)
     */
    void createVBO(size_t reserveSizeBytes = 0);

    /**
     * Binds this vertex buffer object (makes current).
     *
     * @param bufferType  Type of the bound buffer (usually GL_ARRAY_BUFFER, but can be also GL_ELEMENT_BUFFER for instance)
     */
    void bindVBO(GLenum bufferType = GL_ARRAY_BUFFER);

    /**
     * Adds raw data to the in-memory buffer, before they get uploaded.
     *
     * @param ptrData        Pointer to the raw data (arbitrary type)
     * @param dataSizeBytes  Size of the added data (in bytes)
     * @param repeat         How many times to repeat same data in the buffer (default is 1)
     */
    void addRawData(const void* ptrData, size_t dataSizeBytes, size_t repeat = 1);

    /**
     * Adds arbitrary data to the in-memory buffer, before they get uploaded.
     *
     * @param ptrObj  Pointer to trivial object (sizeof which can be determined with sizeof)
     * @param repeat  How many times to repeat same data in the buffer (default is 1)
     */
    template<typename T>
    void addData(const T& ptrObj, size_t repeat = 1)
    {
        addRawData(&ptrObj, static_cast<size_t>(sizeof(T)), repeat);
    }

    /**
     * Gets pointer to the raw data from in-memory buffer (only before uploading them).
     */
    void* getRawDataPointer();

    /**
     * Uploads gathered data to the GPU memory. Now the VBO is ready to be used.
     *
     * @param usageHint  Hint for OpenGL, how is the data intended to be used (GL_STATIC_DRAW, GL_DYNAMIC_DRAW)
     */
    void uploadDataToGPU(GLenum usageHint);

    /**
     * Maps buffer data to a memory pointer.
     *
     * @param usageHint  Hint for OpenGL, how is the data intended to be used (GL_STATIC_DRAW, GL_DYNAMIC_DRAW)
     *
     * @return Pointer to the mapped data, or nullptr, if something fails.
     */
    void* mapBufferToMemory(GLenum usageHint) const;

    /**
     * Maps buffer sub-data to a memory pointer.
     *
     * @param usageHint  Hint for OpenGL, how is the data intended to be used (GL_READ_ONLY, GL_WRITE_ONLY...`)
     * @param offset     Byte offset in buffer where to start
     * @param length     Byte length of the mapped data
     *
     * @return Pointer to the mapped data, or nullptr, if something fails.
     */
    void* mapSubBufferToMemory(GLenum usageHint, size_t offset, size_t length) const;

    /**
     * Unmaps buffer from memory (must have been mapped previously).
     */
    void unmapBuffer() const;

    /**
     * Gets OpenGL-assigned buffer ID.
     */
    GLuint getBufferID() const;

    /**
     * Gets buffer size (in bytes).
     */
    size_t getBufferSize();

    /**
     * Deletes VBO and frees memory and internal structures.
     */
    void deleteVBO();

private:
    GLuint bufferID_{ 0 }; // OpenGL assigned buffer ID
    GLenum bufferType_{ 0 }; // Buffer type (GL_ARRAY_BUFFER, GL_ELEMENT_BUFFER...)

    std::vector<unsigned char> rawData_; // In-memory raw data buffer, used to gather the data for VBO
    size_t bytesAdded_{ 0 }; // Number of bytes added to the buffer so far
    size_t uploadedDataSize_{ 0 }; // Holds buffer data size after uploading to GPU (if it's not null, then data have been uploaded)

    /**
     * Checks if the buffer has been created and has OpenGL-assigned ID.
     */
    bool isBufferCreated() const;

    /**
     * Checks if the the data has been uploaded to the buffer already.
     */
    bool isDataUploaded() const;
};