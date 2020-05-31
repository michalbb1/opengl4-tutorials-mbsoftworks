#pragma once

// STL
#include <vector>

#include <glad\glad.h>

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

	/** \brief Adds raw data to the in-memory buffer, before they get uploaded.
	*   \param ptrData  Pointer to the raw data (arbitrary type)
	*   \param dataSize Size of the added data (in bytes)
	*   \param repeat How many times to repeat same data in the buffer (default is 1)
	*/
	void addRawData(const void* ptrData, uint32_t dataSizeBytes, int repeat = 1);

	/** \brief Adds arbitrary data to the in-memory buffer, before they get uploaded.
	*   \param ptrData Data to be added
	*   \param repeat  How many times to repeat same data in the buffer (default is 1)
	*/
	template<typename T>
	void addData(const T& obj, int repeat = 1)
	{
		addRawData(&obj, sizeof(T), repeat);
	}

	/** \brief Gets pointer to the data from in-memory buffer (only before uploading them).
	*   \return Pointer to the raw data.
	*/
	void* getRawDataPointer();

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

	//* \brief Deletes VBO and frees memory and internal structures.
	void deleteVBO();

private:
	GLuint _bufferID = 0; //! OpenGL assigned buffer ID
	int _bufferType; //! Buffer type (GL_ARRAY_BUFFER, GL_ELEMENT_BUFFER...)

	std::vector<unsigned char> _rawData; //! In-memory raw data buffer, used to gather the data for VBO.
	size_t _bytesAdded = 0; //! Number of bytes added to the buffer so far
	uint32_t _uploadedDataSize; //! Holds buffer data size after uploading to GPU

	bool _isBufferCreated = false;
	bool _isDataUploaded = false; //! Flag telling, if data has been uploaded to GPU already.
};