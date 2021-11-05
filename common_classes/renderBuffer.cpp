// STL
#include <iostream>

// Project
#include "renderBuffer.h"

RenderBuffer::~RenderBuffer()
{
    deleteRenderBuffer();
}

bool RenderBuffer::create(GLenum internalFormat, GLsizei width, GLsizei height)
{
    glGenRenderbuffers(1, &renderBufferID_);
    if(renderBufferID_ == 0)
    {
        std::cerr << "Unable to create renderbuffer with internal format " << internalFormat
            << " and dimensions [" << width << ", " << height << "]!" << std::endl;
        return false;
    }

    std::cout << "Created renderbuffer with ID #" << renderBufferID_ << std::endl;

    // Bind newly created renderbuffer and set its storage attributes
    glBindRenderbuffer(GL_RENDERBUFFER, renderBufferID_);
    glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, width, height);

    // Cache the attributes as member variables
    internalFormat_ = internalFormat;
    width_ = width;
    height_ = height;

    return true;
}

bool RenderBuffer::resize(GLsizei newWidth, GLsizei newHeight)
{
    // If the renderbuffer has not been created yet, we can't even resize it
    if(renderBufferID_ == 0) {
        return false;
    }

    // Just remember the internal format, delete and recreate the buffer with new width and height
    const auto internalFormat = internalFormat_;
    deleteRenderBuffer();
    return create(internalFormat, newWidth, newHeight);
}

void RenderBuffer::deleteRenderBuffer()
{
    if(renderBufferID_ == 0) {
        return;
    }

    std::cout << "Deleting renderbuffer with ID #" << renderBufferID_ << std::endl;
    glDeleteRenderbuffers(1, &renderBufferID_);
    renderBufferID_ = 0;
    width_ = 0;
    height_ = 0;
    depthBits_ = -1;
    stencilBits_ = -1;
}

GLuint RenderBuffer::getID() const
{
    return renderBufferID_;
}

GLenum RenderBuffer::getInternalFormat() const
{
    return internalFormat_;
}

GLsizei RenderBuffer::getWidth() const
{
    return width_;
}

GLsizei RenderBuffer::getHeight() const
{
    return height_;
}

GLint RenderBuffer::getDepthBits()
{
    if(depthBits_ != -1 || renderBufferID_ == 0) {
        return depthBits_;
    }

    // Query how many bits are used for depth and cache the result
    glBindRenderbuffer(GL_RENDERBUFFER, renderBufferID_);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_DEPTH_SIZE, &depthBits_);
    return depthBits_;
}

GLint RenderBuffer::getStencilBits()
{
    if (stencilBits_ != -1 || renderBufferID_ == 0) {
        return stencilBits_;
    }

    // Query how many bits are used for stencil and cache the result
    glBindRenderbuffer(GL_RENDERBUFFER, renderBufferID_);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_STENCIL_SIZE, &stencilBits_);
    return stencilBits_;
}
