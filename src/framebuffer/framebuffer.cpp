#include "framebuffer.h"

#include <spdlog/spdlog.h>

#include <utility>

auto FrameBuffer::create(vec2i size) -> std::unique_ptr<FrameBuffer> {
    GLuint handle = 0;
    glGenFramebuffers(1, &handle);

    glBindFramebuffer(GL_FRAMEBUFFER, handle);

    // To complete the framebuffer we need
    // - at least a color buffer is attached
    // - all buffers are complete
    // - each buffers must be of the same size as the framebuffer

    auto color_buffer = Texture::from_empty(Texture::Type::Color, size, 0);
    auto depth_stencil_buffer = Texture::from_empty(Texture::Type::DepthStencil, size, 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_buffer->handle(), 0);

    glFramebufferTexture2D(
        GL_FRAMEBUFFER,
        GL_DEPTH_STENCIL_ATTACHMENT,
        GL_TEXTURE_2D,
        depth_stencil_buffer->handle(),
        0
    );

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        spdlog::error("Could not complete a framebuffer");
        return nullptr;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    auto frame_buffer = std::make_unique<FrameBuffer>(FrameBuffer());
    frame_buffer->_handle = handle;
    frame_buffer->_type = Type::ColorDepthStencil;
    frame_buffer->_size = size;
    frame_buffer->_color = std::move(color_buffer);
    frame_buffer->_depth_stencil = std::move(depth_stencil_buffer);

    return frame_buffer;
}

auto FrameBuffer::resize(vec2i size) -> void {
    if (size == _size) {
        // Already the good size
        return;
    }

    auto new_frame_buffer = FrameBuffer::create(size);

    std::swap(*this, *new_frame_buffer);
}

auto FrameBuffer::bind() -> void {
    glBindFramebuffer(GL_FRAMEBUFFER, _handle);
}

auto FrameBuffer::unbind() -> void {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

auto FrameBuffer::handle() const -> GLuint {
    return _handle;
}

auto FrameBuffer::type() const -> Type {
    return _type;
}

auto FrameBuffer::size() const -> glm::vec2 {
    return _size;
}

auto FrameBuffer::color_texture() const -> Texture* {
    return _color.get();
}

FrameBuffer::FrameBuffer() {}

FrameBuffer::FrameBuffer(FrameBuffer&& from) noexcept :
    _handle(from._handle),
    _type(from._type),
    _size(from._size),
    _color(std::move(from._color)),
    _depth_stencil(std::move(from._depth_stencil)) {
    from._handle = 0;
}

auto FrameBuffer::operator=(FrameBuffer&& from) noexcept -> FrameBuffer& {
    _handle = from._handle;
    _type = from._type;
    _size = from._size;
    _color = std::move(from._color);
    _depth_stencil = std::move(from._depth_stencil);

    from._handle = 0;

    return *this;
}

FrameBuffer::~FrameBuffer() noexcept {
    if (_handle != 0) {
        glDeleteFramebuffers(1, &_handle);
        _handle = 0;
    }
}
