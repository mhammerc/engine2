#pragma once

#include <memory>

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

    enum Type { ColorDepthStencil, Depth };

    static auto create(vec2i size, Type type) -> std::unique_ptr<Framebuffer>;

    // Methods
    auto resize(vec2i size) -> void;

    auto bind() -> void;
    auto unbind() -> void;

    [[nodiscard]] auto handle() const -> GLuint;
    [[nodiscard]] auto type() const -> Type;
    [[nodiscard]] auto size() const -> vec2i;
    [[nodiscard]] auto color_texture() const -> Texture*;
    [[nodiscard]] auto depth_stencil_texture() const -> Texture*;

  private:
    explicit Framebuffer() = default;
    auto release() -> void;

    GLuint _handle = 0;
    Type _type = Type::ColorDepthStencil;
    vec2i _size = {0, 0};

    std::unique_ptr<Texture> _color;
    std::unique_ptr<Texture> _depth_stencil;
};

}  // namespace engine