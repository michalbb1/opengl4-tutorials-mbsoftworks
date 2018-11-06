#pragma once

#include <glad\glad.h>

#include <vector>

/**
  Wraps OpenGL's vertex buffer object to a higher level class.
*/

class VertexBufferObject
{
public:

	/** \brief Creates a new VBO, with optional reserved buffer size.
	*   \param size Buffer size reservation, in bytes (so that memory allocations don't take place while adding data)
	*/
	void createVBO(uint32_t reserveSizeBytes = 0);

	/** \brief Binds this vertex buffer object (makes current).
	*   \param bufferType Type of the bound buffer (usually GL_ARRAY_BUFFER, but can be also GL_ELEMENT_BUFFER for instance)
	*/
	void bindVBO(GLenum bufferType = GL_ARRAY_BUFFER);

	/** \brief Adds data to the in-memory buffer, before they get uploaded.
	*   \param ptrData  Pointer to the data (arbitrary type)
	*   \param dataSize Size of the added data (in bytes)
	*/
	void addData(void* ptrData, uint32_t dataSizeBytes);

	/** \brief Gets pointer to the data from in-memory buffer (only before uploading them).
	*   \return Pointer to the raw data.
	*/
	void* getCurrentDataPointer();

	/** \brief Uploads gathered data to the GPU memory. Now the VBO is ready to be used.
	*   \param usageHint Hint for OpenGL, how is the data intended to be used (GL_STATIC_DRAW, GL_DYNAMIC_DRAW)
	*/
	void uploadDataToGPU(GLenum usageHint);

	/** \brief Maps buffer data to a memory pointer.
	*   \param usageHint Hint for OpenGL, how is the data intended to be used (GL_STATIC_DRAW, GL_DYNAMIC_DRAW)
	*   \return Pointer to the mapped data, or nullptr, if something fails.
	*/
	void* mapBufferToMemory(GLenum usageHint);

	/** \brief Maps buffer sub-data to a memory pointer.
	*   \param  usageHint Hint for OpenGL, how is the data intended to be used (GL_READ_ONLY, GL_WRITE_ONLY...`)
	*   \param  offset    Byte offset in buffer, where to start
	*   \param  length    Byte length of the mapped data
	*   \return Pointer to the mapped data, or nullptr, if something fails.
	*/
	void* mapSubBufferToMemory(GLenum usageHint, uint32_t offset, uint32_t length);

	//* \brief Unmaps buffer (must have been mapped previously).
	void unmapBuffer();

	/** \brief Gets OpenGL-assigned buffer ID.
	*   \return Buffer ID.
	*/
	GLuint getBufferID();

	/** \brief Gets buffer size, in bytes.
	*   \return Buffer size in bytes.
	*/
	uint32_t getBufferSize();

	//* \brief Deletes VBO and fress memory and internal structures.
	void deleteVBO();

private:
	GLuint _bufferID = 0; //! OpenGL assigned buffer ID
	int _bufferType; //! Buffer type (GL_ARRAY_BUFFER, GL_ELEMENT_BUFFER...)
	uint32_t _uploadedDataSize; //! Holds buffer data size after uploading to GPU
	std::vector<unsigned char> _rawData; //! In-memory raw data buffer, used to gather the data for VBO.

	bool _isBufferCreated = false;
	bool _isDataUploaded = false; //! Flag telling, if data has been uploaded to GPU already.
};