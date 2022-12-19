#pragma once

#include <string>
#include <filesystem>
#include <optional>
#include "../opengl/opengl.h"

class ShaderProgram {
 public:
  static std::optional<ShaderProgram> from_vertex_and_fragment(const std::filesystem::path &vertex,
															   const std::filesystem::path &fragment);

  ~ShaderProgram() noexcept;
  ShaderProgram(const ShaderProgram &) = delete;
  ShaderProgram(ShaderProgram &&) noexcept;

  GLuint handle = 0;

  void setUniform(const std::string &name, float value);
  void setUniform(const std::string &name, int value);
  void setUniform(const std::string &name, bool value);
  void setUniform(const std::string &name, glm::vec4 value);
  void setUniform(const std::string &name, glm::mat4 value);

 private:
  ShaderProgram() = default;
};