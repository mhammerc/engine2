#pragma once

#include <memory>
#include <string>

#include "../common.h"
#include "texture.h"

namespace engine {

class Framebuffer {
  public:
    virtual ~Framebuffer() noexcept;

    Framebuffer(const Framebuffer&) = delete;
    auto operator=(const Framebuffer&) -> Framebuffer& = delete;

    Framebuffer(Framebuffer&&) noexcept;
    auto operator=(Framebuffer&&) noexcept -> Framebuffer&;

    enum Format { Color, Depth, DepthStencil, ColorDepthStencil, ColorDepth };
    using Type = Texture::Type;

    static auto create(std::string const& name, vec2i size, Format format, Type type) -> std::unique_ptr<Framebuffer>;

    // Methods
    auto resize(vec2i size) -> void;

    auto bind() -> void;
    auto unbind() -> void;

    [[nodiscard]] auto handle() const -> GLuint;
    [[nodiscard]] auto size() const -> vec2i;
    [[nodiscard]] auto format() const -> Format;
    [[nodiscard]] auto type() const -> Type;
    [[nodiscard]] auto color_texture() const -> Texture*;
    [[nodiscard]] auto depth_stencil_texture() const -> Texture*;
    [[nodiscard]] auto name() -> std::string&;

  private:
    explicit Framebuffer() = default;
    auto release() -> void;

    GLuint _handle = 0;

    vec2i _size = {0, 0};
    Format _format = Format::ColorDepthStencil;
    Type _type = Type::Texture2D;

    std::string _name;

    std::unique_ptr<Texture> _color;
    std::unique_ptr<Texture> _depth_stencil;
};

}  // namespace engine