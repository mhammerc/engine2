#pragma once

#include <string>
#include <filesystem>
#include <optional>
#include "../opengl/opengl.h"

struct ShaderSource {
  enum Type {
	UNKNOWN,
	FRAGMENT,
	VERTEX
  };

  static std::optional<ShaderSource> from_file(Type type, const std::filesystem::path &path);

  ~ShaderSource() noexcept;
  ShaderSource(const ShaderSource &) = delete;
  ShaderSource(ShaderSource &&) noexcept;

  /**
   * @return true on success, false otherwise.
   */
  bool compile();

  Type type = Type::UNKNOWN;
  std::string source;
  std::filesystem::path path;

  GLuint handle = 0;

 private:
  ShaderSource() = default;
};