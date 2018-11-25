#include <iostream>

#include "vertexBufferObject.h"

void VertexBufferObject::createVBO(uint32_t reserveSizeBytes)
{
	if (_isBufferCreated)
	{
		std::cout << "This buffer is already created! You need to delete it before re-creating it!" << std::endl;
		return;
	}

	glGenBuffers(1, &_bufferID);
	if (reserveSizeBytes > 0)
	{
		_rawData.reserve(reserveSizeBytes);
	}

	_isBufferCreated = true;
}

void VertexBufferObject::bindVBO(GLenum bufferType)
{
	if (!_isBufferCreated)
	{
		std::cout << "This buffer is not created yet! You cannot bind it before you create it!" << std::endl;
		return;
	}

	_bufferType = bufferType;
	glBindBuffer(_bufferType, _bufferID);
}

void VertexBufferObject::addData(void* ptrData, uint32_t dataSize, int repeat)
{
	for (int i = 0; i < repeat; i++) {
		_rawData.insert(_rawData.end(), (unsigned char*)ptrData, (unsigned char*)ptrData + dataSize);
	}
}

void* VertexBufferObject::getCurrentDataPointer()
{
	if (_isDataUploaded || _rawData.size() == 0)
	{
		return nullptr;
	}

	return (void*)_rawData[0];
}

void VertexBufferObject::uploadDataToGPU(GLenum usageHint)
{
	if (!_isBufferCreated)
	{
		std::cout << "This buffer is not created yet! Call createVBO before uploading data to GPU!" << std::endl;
		return;
	}

	glBufferData(_bufferType, _rawData.size(), &_rawData[0], usageHint);
	_isDataUploaded = true;
	_uploadedDataSize = (uint32_t)_rawData.size();
	_rawData.clear();
}

void* VertexBufferObject::mapBufferToMemory(GLenum usageHint)
{
	if (!_isDataUploaded)
	{
		return nullptr;
	}

	return glMapBuffer(_bufferType, usageHint);
}

void* VertexBufferObject::mapSubBufferToMemory(GLenum usageHint, uint32_t offset, uint32_t length)
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

uint32_t VertexBufferObject::getBufferSize()
{
	return _isDataUploaded ? _uploadedDataSize : (uint32_t)_rawData.size();
}

void VertexBufferObject::deleteVBO()
{
	if (_isBufferCreated)
	{
		glDeleteBuffers(1, &_bufferID);
		_isDataUploaded = false;
		_isBufferCreated = false;
	}
	
	_rawData.clear();
}
