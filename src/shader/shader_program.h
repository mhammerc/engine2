#pragma once

#include "../opengl/opengl.h"

#include <filesystem>
#include <optional>
#include <string>

class ShaderProgram {
 public:
  static auto from_vertex_and_fragment(const std::filesystem::path &vertex,
									   const std::filesystem::path &fragment) -> std::optional<ShaderProgram>;

  ~ShaderProgram() noexcept;

  ShaderProgram(const ShaderProgram &) = delete;
  auto operator=(const ShaderProgram &) -> ShaderProgram & = delete;

  ShaderProgram(ShaderProgram &&) noexcept;
  auto operator=(ShaderProgram &&) noexcept -> ShaderProgram &;

  void setUniform(const std::string &name, float value);
  void setUniform(const std::string &name, int value);
  void setUniform(const std::string &name, bool value);
  void setUniform(const std::string &name, glm::vec4 value);
  void setUniform(const std::string &name, glm::vec3 value);
  void setUniform(const std::string &name, glm::mat4 value);
  void setUniform(const std::string &name, glm::mat3 value);

  auto bind() -> void;
  auto unbind() -> void;

 private:
  ShaderProgram() = default;

  GLuint handle = 0;
};