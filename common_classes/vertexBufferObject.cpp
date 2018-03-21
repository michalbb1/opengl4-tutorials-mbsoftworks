#include "vertexBufferObject.h"

void VertexBufferObject::createVBO(uint32_t reserveSize)
{
	glGenBuffers(1, &_bufferID);
	if(reserveSize > 0)
		_rawData.reserve(reserveSize);
}

void VertexBufferObject::bindVBO(GLenum bufferType)
{
	_bufferType = bufferType;
	glBindBuffer(_bufferType, _bufferID);
}

void VertexBufferObject::addData(void* ptrData, uint32_t dataSize)
{
	_rawData.insert(_rawData.end(), (unsigned char*)ptrData, (unsigned char*)ptrData + dataSize);
}

void* VertexBufferObject::getCurrentDataPointer()
{
	if (_isDataUploaded || _rawData.size() == 0)
		return nullptr;

	return (void*)_rawData[0];
}

void VertexBufferObject::uploadDataToGPU(GLenum usageHint)
{
	glBufferData(_bufferType, _rawData.size(), &_rawData[0], usageHint);
	_isDataUploaded = true;
	_uploadedDataSize = (uint32_t)_rawData.size();
	_rawData.clear();
}

void* VertexBufferObject::mapBufferToMemory(GLenum usageHint)
{
	if (!_isDataUploaded)
		return nullptr;

	return glMapBuffer(_bufferType, usageHint);
}

void* VertexBufferObject::mapSubBufferToMemory(GLenum usageHint, uint32_t offset, uint32_t length)
{
	if (!_isDataUploaded)
		return nullptr;

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
	if (!_isDataUploaded)
		return (uint32_t)_rawData.size();

	return _uploadedDataSize;
}

void VertexBufferObject::deleteVBO()
{
	glDeleteBuffers(1, &_bufferID);
	_isDataUploaded = false;
	_rawData.clear();
}
