#pragma once

#include <memory>
#include <span>
#include <string>
#include <tuple>
#include <vector>

#include "../common.h"
#include "texture/texture.h"

namespace engine {

class Framebuffer {
  public:
    enum class TargetBuffer { Color0, Color1, Color2, Color3, Color4, Color5, Depth, Stencil, DepthStencil };
    using Attachment = std::pair<TargetBuffer, std::unique_ptr<Texture>>;

    /**
     * FramebufferAttachment represent one attachment that can be created/attached to
     * a framebuffer.
     */
    struct AttachmentDescription {
        using Format = Texture::Format;
        using Type = Texture::Type;
        using TargetBuffer = Framebuffer::TargetBuffer;

        Format format;
        Type type;
        TargetBuffer target_buffer;

        [[nodiscard]] auto target_buffer_to_opengl() const -> GLenum;
        [[nodiscard]] auto is_color() const -> bool;
    };

    /**
     * Create a framebuffer with arbitrary attachments.
     */
    static auto create_with_attachments(
        std::string const& name,
        vec2i size,
        std::span<AttachmentDescription> attachment_descriptions
    ) -> std::unique_ptr<Framebuffer>;

    /**
     * Create a framebuffer with a usual color attachment and depth attachment.
     *
     * This function is just an easier alias of `create_with_attachments`.
     */
    static auto create_with_color_and_depth(std::string const& name, vec2i size) -> std::unique_ptr<Framebuffer>;

    /**
     * Create a framebuffer with a usual color attachment only.
     *
     * This function is just an easier alias of `create_with_attachments`.
     */
    static auto create_with_color(std::string const& name, vec2i size) -> std::unique_ptr<Framebuffer>;

    /**
     * Create a framebuffer with a depth attachment only.
     *
     * This function is just an easier alias of `create_with_attachments`.
     */
    static auto create_with_depth(std::string const& name, vec2i size) -> std::unique_ptr<Framebuffer>;

  public:
    virtual ~Framebuffer() noexcept;

    Framebuffer(const Framebuffer&) = delete;
    auto operator=(const Framebuffer&) -> Framebuffer& = delete;

    Framebuffer(Framebuffer&&) noexcept;
    auto operator=(Framebuffer&&) noexcept -> Framebuffer&;

  public:
    auto resize(vec2i size) -> void;

    auto bind() -> void;
    auto unbind() -> void;

    [[nodiscard]] auto handle() const -> GLuint;
    [[nodiscard]] auto size() const -> vec2i;
    [[nodiscard]] auto name() -> std::string&;

    /**
     * Return the first color attachment, if any.
     *
     * Do not use this function on framebuffers that have multiple color attachments as it is unpredictable which one
     * will be returned. 
     */
    [[nodiscard]] auto color() const -> Texture*;

    /**
     * Return the first depth attachment, if any.
     */
    [[nodiscard]] auto depth() const -> Texture*;

  private:
    explicit Framebuffer() = default;
    auto release() -> void;

  private:
    GLuint _handle = 0;

    std::string _name;
    vec2i _size = {0, 0};

    std::vector<Attachment> _attachments;
};

}  // namespace engine