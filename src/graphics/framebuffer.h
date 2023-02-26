#pragma once

#include <memory>

#include "../common.h"
#include "texture.h"

namespace engine {

class FrameBuffer {
    // Constructors and destructors
  public:
    virtual ~FrameBuffer() noexcept;

    FrameBuffer(const FrameBuffer&) = delete;
    auto operator=(const FrameBuffer&) -> FrameBuffer& = delete;

    FrameBuffer(FrameBuffer&&) noexcept;
    auto operator=(FrameBuffer&&) noexcept -> FrameBuffer&;

    enum Type { Color, ColorDepthStencil };

    static auto create(vec2i size) -> std::unique_ptr<FrameBuffer>;

    // Methods
    auto resize(vec2i size) -> void;

    auto bind() -> void;
    auto unbind() -> void;

    [[nodiscard]] auto handle() const -> GLuint;
    [[nodiscard]] auto type() const -> Type;
    [[nodiscard]] auto size() const -> glm::vec2;
    [[nodiscard]] auto color_texture() const -> Texture*;

  private:
    explicit FrameBuffer();

    GLuint _handle = 0;
    Type _type = Type::ColorDepthStencil;
    vec2i _size;

    std::unique_ptr<Texture> _color;
    std::unique_ptr<Texture> _depth_stencil;
};

}  // namespace engine