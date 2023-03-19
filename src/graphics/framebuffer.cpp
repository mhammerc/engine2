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

auto Framebuffer::create(std::string const& name, vec2i size, Format format, Type type)
    -> std::unique_ptr<Framebuffer> {
    GLuint handle = 0;
    glGenFramebuffers(1, &handle);
    glBindFramebuffer(GL_FRAMEBUFFER, handle);

    std::unique_ptr<Texture> color_buffer = nullptr;
    std::unique_ptr<Texture> depth_buffer = nullptr;

    if (format == Format::Color || format == Format::ColorDepthStencil || format == Format::ColorDepth) {
        color_buffer = Texture::from_empty("fb_color", Texture::Type::Texture2D, Texture::Format::Color, size);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_buffer->handle(), 0);
    } else {
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
    }

    if (format == Format::Depth || format == Format::DepthStencil || format == Format::ColorDepthStencil
        || format == Format::ColorDepth) {
        Texture::Format tex_format = Texture::Format::Depth;
        GLenum gl_attachment = GL_DEPTH_ATTACHMENT;

        if (format == Format::DepthStencil || format == Format::ColorDepthStencil) {
            tex_format = Texture::Format::DepthStencil;
            gl_attachment = GL_DEPTH_STENCIL_ATTACHMENT;
        }

        depth_buffer = Texture::from_empty("fb_depth", Texture::Type::Texture2D, tex_format, size);
        glFramebufferTexture2D(GL_FRAMEBUFFER, gl_attachment, GL_TEXTURE_2D, depth_buffer->handle(), 0);
    }

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        spdlog::error("Could not complete a framebuffer");
        return nullptr;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    auto framebuffer = std::make_unique<Framebuffer>(Framebuffer());
    framebuffer->_handle = handle;
    framebuffer->_size = size;
    framebuffer->_format = format;
    framebuffer->_type = type;
    framebuffer->_color = std::move(color_buffer);
    framebuffer->_depth_stencil = std::move(depth_buffer);
    framebuffer->_name = name;

    return framebuffer;
}

auto Framebuffer::resize(vec2i size) -> void {
    if (size == _size) {
        // Already the good size
        return;
    }

    auto new_frame_buffer = Framebuffer::create(_name, size, _format, _type);

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

auto Framebuffer::size() const -> vec2i {
    return _size;
}

auto Framebuffer::format() const -> Format {
    return _format;
}

auto Framebuffer::type() const -> Type {
    return _type;
}

auto Framebuffer::color_texture() const -> Texture* {
    return _color.get();
}

auto Framebuffer::depth_stencil_texture() const -> Texture* {
    return _depth_stencil.get();
}

auto Framebuffer::name() -> std::string& {
    return _name;
}

Framebuffer::Framebuffer(Framebuffer&& from) noexcept :
    _handle(from._handle),
    _size(from._size),
    _format(from._format),
    _type(from._type),
    _name(std::move(from._name)),
    _color(std::move(from._color)),
    _depth_stencil(std::move(from._depth_stencil)) {
    from._handle = 0;
}

auto Framebuffer::operator=(Framebuffer&& from) noexcept -> Framebuffer& {
    release();

    _handle = from._handle;
    _size = from._size;
    _format = from._format;
    _type = from._type;
    _name = std::move(from._name);
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
