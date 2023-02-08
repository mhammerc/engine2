#pragma once

#include <filesystem>
#include <optional>

#include "../opengl/opengl.h"
#include "image.h"

class Texture {
 public:
  enum Type {
	Diffuse,
	Specular
  };

  GLuint handle = 0;
  Type type;

  static auto from_file(const std::filesystem::path &path, Type type) -> std::optional<Texture>;


  auto activate_as(int index) -> void;

  ~Texture() noexcept;
  Texture(const Texture &) = delete;
  auto operator=(const Texture &) -> Texture & = delete;
  Texture(Texture &&) noexcept;
  auto operator=(Texture &&) noexcept -> Texture &;
 private:
  explicit Texture(GLuint handle, Type type);
};
