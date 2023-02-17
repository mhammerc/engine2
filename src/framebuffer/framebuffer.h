#pragma once

#include <memory>

#include "../opengl/opengl.h"
#include "../texture/texture.h"

class FrameBuffer {
    // Constructors and destructors
  public:
    virtual ~FrameBuffer() noexcept;

    FrameBuffer(const FrameBuffer&) = delete;
    auto operator=(const FrameBuffer&) -> FrameBuffer& = delete;

    FrameBuffer(FrameBuffer&&) noexcept;
    auto operator=(FrameBuffer&&) noexcept -> FrameBuffer&;

    enum Type { Color, ColorDepthStencil };

    static auto create(int width, int height) -> std::unique_ptr<FrameBuffer>;

    // Methods
    auto resize(int width, int height) -> void;

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
    glm::vec2 _size;

    std::unique_ptr<Texture> _color;
    std::unique_ptr<Texture> _depth_stencil;
};