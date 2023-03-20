#include "framebuffer.h"

#include <utility>

#include "spdlog/spdlog.h"

using namespace engine;

static auto color_format_to_texture_format(Framebuffer::ColorFormat format) -> Texture::Format {
    if (format == Framebuffer::ColorFormat::RGB) {
        return Texture::Format::RGB;
    }

    if (format == Framebuffer::ColorFormat::SRGB) {
        return Texture::Format::SRGB;
    }

    ENGINE_CODE_ERROR("missing case");

    return Texture::Format::RGB;
}

static auto depth_format_to_texture_format(Framebuffer::DepthFormat format) -> Texture::Format {
    if (format == Framebuffer::DepthFormat::Depth) {
        return Texture::Format::Depth;
    }

    if (format == Framebuffer::DepthFormat::DepthStencil) {
        return Texture::Format::DepthStencil;
    }

    ENGINE_CODE_ERROR("missing case");

    return Texture::Format::Depth;
}

/**
 * To create a complete framebuffer we need:
 *  - at least a color buffer is attached or `glDrawBuffer(GL_NONE); glReadBuffer(GL_NONE);`
 *    - the latter will make the framebuffer render only depth/stencil
 *  - all buffers are complete
 *    - Looks like each buffer must be a square or smth.
 *  - each buffers must be of the same size as the framebuffer
 */

auto Framebuffer::create(std::string const& name, vec2i size, Content content, Type type, DepthFormat depth_format)
    -> std::unique_ptr<Framebuffer> {
    return Framebuffer::create(name, size, content, type, ColorFormat::RGB, depth_format);
}

auto Framebuffer::create(
    std::string const& name,
    vec2i size,
    Content content,
    Type type,
    ColorFormat color_format,
    DepthFormat depth_format
) -> std::unique_ptr<Framebuffer> {
    GLuint handle = 0;
    glGenFramebuffers(1, &handle);
    glBindFramebuffer(GL_FRAMEBUFFER, handle);

    std::unique_ptr<Texture> color_buffer = nullptr;
    std::unique_ptr<Texture> depth_buffer = nullptr;

    if (content == Content::Color || content == Content::ColorAndDepthStencil) {
        auto texture_name = fmt::format("{}_fb_color", name);
        color_buffer = Texture::from_empty(texture_name, type, color_format_to_texture_format(color_format), size);

        if (type == Type::Texture2D) {
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_buffer->handle(), 0);
        } else {
            glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, color_buffer->handle(), 0);
        }
    } else {
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
    }

    if (content == Content::DepthStencil || content == Content::ColorAndDepthStencil) {
        GLenum gl_attachment = GL_DEPTH_ATTACHMENT;

        if (depth_format == DepthFormat::DepthStencil) {
            gl_attachment = GL_DEPTH_STENCIL_ATTACHMENT;
        }

        depth_buffer = Texture::from_empty("fb_depth", type, depth_format_to_texture_format(depth_format), size);

        if (type == Type::Texture2D) {
            glFramebufferTexture2D(GL_FRAMEBUFFER, gl_attachment, GL_TEXTURE_2D, depth_buffer->handle(), 0);
        } else {
            glFramebufferTexture(GL_FRAMEBUFFER, gl_attachment, depth_buffer->handle(), 0);
        }
    }

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        spdlog::error("Could not complete a framebuffer");
        return nullptr;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    auto framebuffer = std::make_unique<Framebuffer>(Framebuffer());
    framebuffer->_handle = handle;
    framebuffer->_size = size;
    framebuffer->_content = content;
    framebuffer->_type = type;
    framebuffer->_color_format = color_format;
    framebuffer->_depth_format = depth_format;
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

    auto new_frame_buffer = Framebuffer::create(_name, size, _content, _type, _color_format, _depth_format);

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

auto Framebuffer::content() const -> Content {
    return _content;
}

auto Framebuffer::type() const -> Type {
    return _type;
}

auto Framebuffer::color_format() const -> ColorFormat {
    return _color_format;
}

auto Framebuffer::depth_format() const -> DepthFormat {
    return _depth_format;
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
    _content(from._content),
    _type(from._type),
    _color_format(from._color_format),
    _depth_format(from._depth_format),
    _name(std::move(from._name)),
    _color(std::move(from._color)),
    _depth_stencil(std::move(from._depth_stencil)) {
    from._handle = 0;
}

auto Framebuffer::operator=(Framebuffer&& from) noexcept -> Framebuffer& {
    release();

    _handle = from._handle;
    _size = from._size;
    _content = from._content;
    _type = from._type;
    _color_format = from._color_format;
    _depth_format = from._depth_format;
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
