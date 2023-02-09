#pragma once

#include "../opengl/opengl.h"
#include "../texture/texture.h"

#include <memory>

class FrameBuffer {
  // Constructors and destructors
 public:
  virtual ~FrameBuffer() noexcept;

  FrameBuffer(const FrameBuffer &) = delete;
  auto operator=(const FrameBuffer &) -> FrameBuffer & = delete;

  FrameBuffer(FrameBuffer &&) noexcept;
  auto operator=(FrameBuffer &&) noexcept -> FrameBuffer &;

  enum Type {
	Color,
	ColorDepthStencil
  };

  static auto create() -> std::unique_ptr<FrameBuffer>;

  // Methods
  auto resize() -> void;

  auto bind() -> void;
  auto unbind() -> void;

  [[nodiscard]] auto handle() const -> GLuint;
  [[nodiscard]] auto type() const -> Type;
  [[nodiscard]] auto color_texture() const -> Texture *;

 private:
  explicit FrameBuffer();

  GLuint _handle = 0;
  Type _type = Type::ColorDepthStencil;

  std::unique_ptr<Texture> _color;
  std::unique_ptr<Texture> _depth_stencil;
};