#pragma once

// STL
#include <memory>
#include <vector>

// Project
#include "renderBuffer.h"
#include "texture.h"

/**
 * Wraps OpenGL framebuffer into a convenient class. Provides support
 * for several operations with framebuffer such as copying data
 * from / to default framebuffer.
 *
 * There are also several static methods (wrapped in a static class Default)
 * to work with default framebuffer conveniently.
 */
class FrameBuffer
{
public:
    ~FrameBuffer();

    bool createFrameBufferWithColorAndDepthWithDefaultScreenSize();
    bool createFrameBufferWithColorAndDepth(GLsizei width, GLsizei height);

    /**
     * Binds frame buffer as either read framebuffer, draw framebuffer
     * or both.
     */
    void bindAsBothReadAndDraw() const;
    void bindAsRead() const;
    void bindAsDraw() const;

    /**
     * Resizes framebuffer with new width and height while keeping
     * its original attachments.
     *
     * @param newWidth   New framebuffer width (in pixels)
     * @param newHeight  New framebuffer height (in pixels)
     *
     * @return True if framebuffer has been resized successfully or false otherwise.
     */
    bool resize(GLsizei newWidth, GLsizei newHeight);

    /**
     * Sets viewport to cover whole framebuffer.
     */
    void setFullViewport() const;

    /**
     * Copies color values to the default framebuffer.
     */
    void copyColorToDefaultFrameBuffer() const;

    /**
     * Copies depth from the default framebuffer.
     */
    void copyDepthFromDefaultFrameBuffer() const;

    /**
     * Gets width of the framebuffer in pixels.
     */
    GLsizei getWidth() const;

    /**
     * Gets height of the framebuffer in pixels.
     */
    GLsizei getHeight() const;

    /**
     * Queries, caches and returns how many bits are used to represent depth in this framebuffer.
     * This value will be successfully queried only if framebuffer contains depth component.
     */
    GLint getDepthBits();

    /**
     * Queries, caches and returns how many bits are used to represent stencil in this framebuffer.
     * This value will be successfully queried only if framebuffer contains stencil component.
     */
    GLint getStencilBits();

    /**
     * Reads color value from the currently bound read framebuffer (GL_READ_FRAMEBUFFER).
     *
     * @return GLubyte vector containing color RGBA values.
     */
    static std::vector<GLubyte> readColorValue(int x, int y);

    /**
     * Deletes all OpenGL objects created within this instance of framebuffer
     * and resets class to its initial state.
     */
    void deleteFrameBuffer();

    Texture* getTexture() const;

    /**
     * Static subclass for working with default window framebuffer.
     */
    class Default
    {
    public:
        /**
         * Binds default frame buffer as either read framebuffer, draw framebuffer
         * or both.
         */
        static void bindAsBothReadAndDraw();
        static void bindAsRead();
        static void bindAsDraw();

        /**
         * Queries and returns how many bits are used to represent depth in default framebuffer.
         */
        static GLint getDepthBits();

        /**
         * Queries and returns how many bits are used to represent stencil in default framebuffer.
         */
        static GLint getStencilBits();

        /**
         * Sets viewport of default framebuffer to take whole screen.
         */
        static void setFullViewport();
    };

    /**
     * Subclass that builds a framebuffer object. Because of variety of options,
     * it's a lot more practical to create and configure framebuffer like this.
     */
    class Builder
    {
    public:
        /**
         * Creates and binds new framebuffer.
         *
         * @param width   New framebuffer width (in pixels)
         * @param height  New framebuffer height (in pixels)
         */
        Builder& createAndBind(GLsizei width, GLsizei height);

        /**
         * Adds color attachment with specified format.
         *
         * @param internalFormat  Internal format of the color data (e.g. GL_RGB8)
         */
        Builder& withColorAttachment(GLenum internalFormat);

        /**
         * Adds texture color attachment to the framebuffer with specified format.
         *
         * @param internalFormat  Internal format of the texture data (e.g. GL_RGB)
         */
        Builder& withTextureColorAttachment(GLenum internalFormat);

        /**
         * Adds depth attachment to the framebuffer.
         *
         * @param internalFormat  Internal format of the depth data
         */
        Builder& withDepthAttachment(GLenum internalFormat);

        /**
         * Finishes building and returns unique pointer to built framebuffer.
         */
        std::unique_ptr<FrameBuffer> finishAndGetUnique();

        /**
         * Finishes building and returns shared pointer to built framebuffer.
         */
        std::shared_ptr<FrameBuffer> finishAndGetShared();

    private:
        std::unique_ptr<FrameBuffer> frameBuffer_; // Holds framebuffer that is being built
    };

private:
    friend class Builder; // Builder has access to the private methods below

    bool createAndBind(GLsizei width, GLsizei height);
    bool withColorAttachment(GLenum internalFormat);
    bool withDepthAttachment(GLenum internalFormat);
    bool withTextureColorAttachment(GLenum internalFormat);
    bool finishInitialization() const;

    GLuint frameBufferID_{ 0 }; // ID of the framebuffer generated by OpenGL
    std::unique_ptr<RenderBuffer> colorRenderBuffer_; // Here we hold color renderbuffer
    std::unique_ptr<RenderBuffer> depthRenderBuffer_; // Here we hold depth renderbuffer
    std::unique_ptr<Texture> texture_; // If we use framebuffer to render to a texture, this is the texture

    GLsizei width_{ 0 }; // Width of the framebuffer in pixels
    GLsizei height_{ 0 }; // Height of the framebuffer in pixels
    GLint depthBits_{ -1 }; // Cached number of bits for depth
    GLint stencilBits_{ -1 }; // Cached number of bits for stencil

    /**
     * Internal method that deletes only framebuffer object, but keeps renderbuffers and texture.
     */
    void deleteOnlyFrameBuffer();
};
