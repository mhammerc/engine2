#pragma once

#include "../opengl/opengl.h"
#include "image.h"

#include <filesystem>
#include <optional>

class Texture {
 public:
  enum Type {
	Diffuse,
	Specular
  };

  static auto from_file(const std::filesystem::path &path, Type type) -> std::optional<Texture>;

  auto activate_as(int index) -> void;

  ~Texture() noexcept;

  Texture(const Texture &) = delete;
  auto operator=(const Texture &) -> Texture & = delete;

  Texture(Texture &&) noexcept;
  auto operator=(Texture &&) noexcept -> Texture &;

  [[nodiscard]] auto handle() const -> GLuint;
  [[nodiscard]] auto type() const -> Type;

 private:
  explicit Texture(GLuint handle, Type type);

  GLuint _handle = 0;
  Type _type;
};
