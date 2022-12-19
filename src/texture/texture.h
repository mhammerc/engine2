#pragma once

#include <filesystem>
#include <optional>

#include "../opengl/opengl.h"
#include "image.h"

class Texture {
 public:
  static auto from_file(const std::filesystem::path &path) -> std::optional<Texture>;

  GLuint handle = 0;

  void activate_as(int index);

  ~Texture() noexcept;
  Texture(const Texture &) = delete;
  Texture(Texture &&) noexcept;
 private:
  explicit Texture(GLuint handle);
};
