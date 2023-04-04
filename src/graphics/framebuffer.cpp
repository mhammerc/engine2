#include "framebuffer.h"

#include <spdlog/fmt/fmt.h>

#include <algorithm>
#include <array>
#include <ranges>

using namespace engine;

auto Framebuffer::target_buffer_to_opengl(TargetBuffer target_buffer) -> GLenum {
    if (target_buffer == TargetBuffer::Color0) {
        return GL_COLOR_ATTACHMENT0;
    }
    if (target_buffer == TargetBuffer::Color1) {
        return GL_COLOR_ATTACHMENT1;
    }
    if (target_buffer == TargetBuffer::Color2) {
        return GL_COLOR_ATTACHMENT2;
    }
    if (target_buffer == TargetBuffer::Color3) {
        return GL_COLOR_ATTACHMENT3;
    }
    if (target_buffer == TargetBuffer::Color4) {
        return GL_COLOR_ATTACHMENT4;
    }
    if (target_buffer == TargetBuffer::Color5) {
        return GL_COLOR_ATTACHMENT5;
    }
    if (target_buffer == TargetBuffer::Depth) {
        return GL_DEPTH_ATTACHMENT;
    }
    if (target_buffer == TargetBuffer::Stencil) {
        return GL_STENCIL_ATTACHMENT;
    }
    if (target_buffer == TargetBuffer::DepthStencil) {
        return GL_DEPTH_STENCIL_ATTACHMENT;
    }

    ENGINE_CODE_ERROR("missing case");

    return GL_COLOR_ATTACHMENT0;
}

auto Framebuffer::is_color(TargetBuffer target_buffer) -> bool {
    return (
        target_buffer != TargetBuffer::Depth && target_buffer != TargetBuffer::Stencil
        && target_buffer != TargetBuffer::DepthStencil
    );
}

auto Framebuffer::AttachmentDescription::target_buffer_to_opengl() const -> GLenum {
    return Framebuffer::target_buffer_to_opengl(target_buffer);
}

auto Framebuffer::AttachmentDescription::is_color() const -> bool {
    return Framebuffer::is_color(target_buffer);
}

auto Framebuffer::create_with_attachments(
    const std::string& name,
    vec2i size,
    std::span<const AttachmentDescription> attachment_descriptions
) -> std::unique_ptr<Framebuffer> {
    GLuint handle = 0;
    glGenFramebuffers(1, &handle);
    glBindFramebuffer(GL_FRAMEBUFFER, handle);

    std::vector<Attachment> textures;
    textures.reserve(attachment_descriptions.size());

    std::vector<GLenum> draw_attachments;
    draw_attachments.reserve(attachment_descriptions.size());

    // Create the texture for each attachment
    std::transform(
        attachment_descriptions.begin(),
        attachment_descriptions.end(),
        std::back_inserter(textures),
        [&](AttachmentDescription const& attachment) {
            auto texture_name = attachment.name.empty() ? fmt::format("{}_att", name) : attachment.name;

            // Create the texture...
            auto texture = Texture::from_empty(texture_name, attachment.type, attachment.format, size);

            // ... and bind it to the framebuffer
            if (attachment.type == AttachmentDescription::Type::Texture2D) {
                glFramebufferTexture2D(
                    GL_FRAMEBUFFER,
                    attachment.target_buffer_to_opengl(),
                    GL_TEXTURE_2D,
                    texture->handle(),
                    0
                );
            } else if (attachment.type == AttachmentDescription::Type::CubeMap) {
                glFramebufferTexture(GL_FRAMEBUFFER, attachment.target_buffer_to_opengl(), texture->handle(), 0);
            } else {
                ENGINE_CODE_ERROR("missing case");
            }

            if (attachment.is_color()) {
                draw_attachments.push_back(attachment.target_buffer_to_opengl());
            }

            return std::make_pair(attachment.target_buffer, std::move(texture));
        }
    );

    glDrawBuffers(static_cast<GLsizei>(draw_attachments.size()), draw_attachments.data());
    glReadBuffer(GL_NONE);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        spdlog::error("Could not complete a framebuffer");
        glDeleteFramebuffers(1, &handle);
        return nullptr;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    auto framebuffer = std::make_unique<Framebuffer>(Framebuffer());
    framebuffer->_handle = handle;
    framebuffer->_name = name;
    framebuffer->_size = size;
    framebuffer->_attachments = std::move(textures);

    return framebuffer;
}

auto Framebuffer::create_with_color_and_depth(std::string const& name, vec2i size) -> std::unique_ptr<Framebuffer> {
    using Format = AttachmentDescription::Format;
    using Type = AttachmentDescription::Type;
    using TargetBuffer = AttachmentDescription::TargetBuffer;

    std::array<AttachmentDescription, 2> attachments_descriptions {{
        {Format::RGBA16F, Type::Texture2D, TargetBuffer::Color0, "Color"},
        {Format::Depth, Type::Texture2D, TargetBuffer::Depth, "Depth"},
    }};

    auto framebuffer = Framebuffer::create_with_attachments(name, size, attachments_descriptions);
    return framebuffer;
}

auto Framebuffer::create_with_color(std::string const& name, vec2i size) -> std::unique_ptr<Framebuffer> {
    using Format = AttachmentDescription::Format;
    using Type = AttachmentDescription::Type;
    using TargetBuffer = AttachmentDescription::TargetBuffer;

    std::array<AttachmentDescription, 1> attachments_descriptions {{
        {Format::RGB, Type::Texture2D, TargetBuffer::Color0, "Color"},
    }};

    auto framebuffer = Framebuffer::create_with_attachments(name, size, attachments_descriptions);
    return framebuffer;
}

auto Framebuffer::create_with_depth(std::string const& name, vec2i size) -> std::unique_ptr<Framebuffer> {
    using Format = AttachmentDescription::Format;
    using Type = AttachmentDescription::Type;
    using TargetBuffer = AttachmentDescription::TargetBuffer;

    std::array<AttachmentDescription, 1> attachments_descriptions {{
        {Format::Depth, Type::Texture2D, TargetBuffer::Depth, "Depth"},
    }};

    auto framebuffer = Framebuffer::create_with_attachments(name, size, attachments_descriptions);
    return framebuffer;
}

auto Framebuffer::resize(vec2i size) -> void {
    if (size == _size) {
        // Already the good size
        return;
    }

    std::vector<AttachmentDescription> attachments_descriptions;
    attachments_descriptions.reserve(_attachments.size());

    std::transform(
        _attachments.begin(),
        _attachments.end(),
        std::back_inserter(attachments_descriptions),
        [](Attachment const& attachment) {
            return Framebuffer::AttachmentDescription {
                attachment.second->format(),
                attachment.second->type(),
                attachment.first,
                attachment.second->name(),
            };
        }
    );

    auto new_framebuffer = Framebuffer::create_with_attachments(_name, size, attachments_descriptions);

    std::swap(*this, *new_framebuffer);
}

auto Framebuffer::bind() -> void {
    glBindFramebuffer(GL_FRAMEBUFFER, _handle);

    glViewport(0, 0, _size.x, _size.y);

    auto _color_attachments = _attachments
        | std::views::transform([](auto const& attachment) { return attachment.first; })
        | std::views::filter(&Framebuffer::is_color) | std::views::transform(&Framebuffer::target_buffer_to_opengl);
    auto color_attachments = std::vector(_color_attachments.begin(), _color_attachments.end());

    glDrawBuffers(static_cast<GLsizei>(color_attachments.size()), color_attachments.data());
}

auto Framebuffer::unbind() -> void {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

auto Framebuffer::clear() -> void {
    bind();
    glClearColor(0.F, 0.F, 0.F, 1.F);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    unbind();
}

auto Framebuffer::handle() const -> GLuint {
    return _handle;
}

auto Framebuffer::size() const -> vec2i {
    return _size;
}

auto Framebuffer::name() -> std::string& {
    return _name;
}

auto Framebuffer::attachments() const -> std::span<const Attachment> {
    return _attachments;
}

auto Framebuffer::color() const -> Texture* {
    auto it = std::find_if(_attachments.begin(), _attachments.end(), [](Attachment const& attachment) {
        return attachment.first != TargetBuffer::Depth && attachment.first != TargetBuffer::Stencil
            && attachment.first != TargetBuffer::DepthStencil;
    });

    if (it == _attachments.end()) {
        return nullptr;
    }

    return it->second.get();
}

auto Framebuffer::depth() const -> Texture* {
    auto it = std::find_if(_attachments.begin(), _attachments.end(), [](Attachment const& attachment) {
        return attachment.first == TargetBuffer::Depth || attachment.first == TargetBuffer::Stencil
            || attachment.first == TargetBuffer::DepthStencil;
    });

    if (it == _attachments.end()) {
        return nullptr;
    }

    return it->second.get();
}

auto Framebuffer::release() -> void {
    if (_handle != 0) {
        glDeleteFramebuffers(1, &_handle);
        _handle = 0;
    }
}

Framebuffer::~Framebuffer() noexcept {
    release();
}

Framebuffer::Framebuffer(Framebuffer&& from) noexcept :
    _handle(from._handle),
    _name(std::move(from._name)),
    _size(from._size),
    _attachments(std::move(from._attachments)) {
    from._handle = 0;
}

auto Framebuffer::operator=(Framebuffer&& from) noexcept -> Framebuffer& {
    release();

    _handle = from._handle;
    _name = std::move(from._name);
    _size = from._size;
    _attachments = std::move(from._attachments);

    from._handle = 0;

    return *this;
}