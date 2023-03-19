#include "framebuffer.h"

#include <utility>

#include "spdlog/spdlog.h"

using namespace engine;

/**
 * To create a complete framebuffer we need:
 *  - at least a color buffer is attached or `glDrawBuffer(GL_NONE); glReadBuffer(GL_NONE);`
 *    - the latter will make the framebuffer render only depth/stencil
 *  - all buffers are complete
 *    - Looks like each buffer must be a square or smth.
 *  - each buffers must be of the same size as the framebuffer
 */

auto Framebuffer::create(vec2i size, Type type) -> std::unique_ptr<Framebuffer> {
    GLuint handle = 0;
    glGenFramebuffers(1, &handle);

    glBindFramebuffer(GL_FRAMEBUFFER, handle);

    std::unique_ptr<Texture> color_buffer = nullptr;
    if (type == Type::ColorDepthStencil) {
        color_buffer = Texture::from_empty("fb_color", Texture::Type::Texture2D, Texture::Format::Color, size);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_buffer->handle(), 0);
    } else {
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
    }

    auto depth_stencil_buffer =
        Texture::from_empty("fb_depth", Texture::Type::Texture2D, Texture::Format::DepthStencil, size);
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

    auto frame_buffer = std::make_unique<Framebuffer>(Framebuffer());
    frame_buffer->_handle = handle;
    frame_buffer->_type = type;
    frame_buffer->_size = size;
    frame_buffer->_color = std::move(color_buffer);
    frame_buffer->_depth_stencil = std::move(depth_stencil_buffer);

    return frame_buffer;
}

auto Framebuffer::resize(vec2i size) -> void {
    if (size == _size) {
        // Already the good size
        return;
    }

    auto new_frame_buffer = Framebuffer::create(size, _type);

    std::swap(*this, *new_frame_buffer);
}

auto Framebuffer::bind() -> void {
    glBindFramebuffer(GL_FRAMEBUFFER, _handle);

    glViewport(0, 0, _size.x, _size.y);
}

auto Framebuffer::unbind() -> void {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

auto Framebuffer::handle() const -> GLuint {
    return _handle;
}

auto Framebuffer::type() const -> Type {
    return _type;
}

auto Framebuffer::size() const -> vec2i {
    return _size;
}

auto Framebuffer::color_texture() const -> Texture* {
    return _color.get();
}

auto Framebuffer::depth_stencil_texture() const -> Texture* {
    return _depth_stencil.get();
}

Framebuffer::Framebuffer(Framebuffer&& from) noexcept :
    _handle(from._handle),
    _type(from._type),
    _size(from._size),
    _color(std::move(from._color)),
    _depth_stencil(std::move(from._depth_stencil)) {
    from._handle = 0;
}

auto Framebuffer::operator=(Framebuffer&& from) noexcept -> Framebuffer& {
    release();

    _handle = from._handle;
    _type = from._type;
    _size = from._size;
    _color = std::move(from._color);
    _depth_stencil = std::move(from._depth_stencil);

    from._handle = 0;

    return *this;
}

Framebuffer::~Framebuffer() noexcept {
    release();
}

auto Framebuffer::release() -> void {
    if (_handle != 0) {
        glDeleteFramebuffers(1, &_handle);
        _handle = 0;
    }
}
