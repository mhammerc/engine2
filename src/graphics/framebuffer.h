#pragma once

#include <memory>
#include <string>

#include "../common.h"
#include "texture/texture.h"

namespace engine {

class Framebuffer {
  public:
    virtual ~Framebuffer() noexcept;

    Framebuffer(const Framebuffer&) = delete;
    auto operator=(const Framebuffer&) -> Framebuffer& = delete;

    Framebuffer(Framebuffer&&) noexcept;
    auto operator=(Framebuffer&&) noexcept -> Framebuffer&;

    enum class Content { Color, DepthStencil, ColorAndDepthStencil };
    using Type = Texture::Type;
    enum class ColorFormat { RGB, SRGB };
    enum class DepthFormat { Depth, DepthStencil };

    static auto create(
        std::string const& name,
        vec2i size,
        Content content,
        Type type,
        ColorFormat color_format = ColorFormat::RGB,
        DepthFormat depth_format = DepthFormat::Depth
    ) -> std::unique_ptr<Framebuffer>;

    // Just an overload for easier creation of depth-only framebuffers
    static auto create(std::string const& name, vec2i size, Content content, Type type, DepthFormat depth_format)
        -> std::unique_ptr<Framebuffer>;

    // Methods
    auto resize(vec2i size) -> void;

    auto bind() -> void;
    auto unbind() -> void;

    [[nodiscard]] auto handle() const -> GLuint;
    [[nodiscard]] auto size() const -> vec2i;
    [[nodiscard]] auto content() const -> Content;
    [[nodiscard]] auto type() const -> Type;
    [[nodiscard]] auto color_format() const -> ColorFormat;
    [[nodiscard]] auto depth_format() const -> DepthFormat;
    [[nodiscard]] auto color_texture() const -> Texture*;
    [[nodiscard]] auto depth_stencil_texture() const -> Texture*;
    [[nodiscard]] auto name() -> std::string&;

  private:
    explicit Framebuffer() = default;
    auto release() -> void;

    GLuint _handle = 0;

    vec2i _size = {0, 0};
    Content _content = Content::ColorAndDepthStencil;
    Type _type = Type::Texture2D;
    ColorFormat _color_format = ColorFormat::RGB;
    DepthFormat _depth_format = DepthFormat::Depth;

    std::string _name;

    std::unique_ptr<Texture> _color;
    std::unique_ptr<Texture> _depth_stencil;
};

}  // namespace engine