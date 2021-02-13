#pragma once

// GLAD
#include <glad/glad.h>

/**
 * Wraps OpenGL's uniform buffer object to a convenient higher level class.
 */
class UniformBufferObject
{
public:
    ~UniformBufferObject();

    /**
     * Creates new UBO with specified buffer size.
     *
     * @param byteSize  Size of buffer in bytes
     */
	void createUBO(const size_t byteSize, GLenum usageHint = GL_STREAM_DRAW);

    /**
     * Binds this uniform buffer object (makes current).
     */
	void bindUBO() const;

    /**
     * Sets buffer data with specific size at the given byte offset.
     *
     * @param offset    Byte offset to set data at
     * @param ptrData   Pointer to data to be set
     * @param dataSize  Size of data to be set
     */
    void setBufferData(const size_t offset, const void* ptrData, const size_t dataSize);

    /**
     * Binds buffer base to the given binding point. This means, that the whole buffer
     * from the beginning will represent the data at that binding point.
     *
     * @param bindingPoint  Binding point to bind whole buffer to
     */
    void bindBufferBaseToBindingPoint(const GLuint bindingPoint) const;

    /**
    * Gets OpenGL-assigned buffer ID.
    */
    GLuint getBufferID() const;

    /**
    * Deletes uniform buffer object.
    */
    void deleteUBO();

private:
    GLuint _bufferID{ 0 }; // OpenGL assigned buffer ID
    size_t _byteSize; // Holds buffer size in bytes

	bool _isBufferCreated = false; // Flag telling if the buffer is created
};

/**
 * Stores most common binding blocks used throughout the tutorials.
 */
class UniformBlockBindingPoints
{
public:
    static const int MATRICES{ 0 };
    static const int POINT_LIGHTS{ 1 };
};