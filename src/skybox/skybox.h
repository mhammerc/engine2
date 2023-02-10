#pragma once

#include "../opengl/opengl.h"
#include "../shader/shader_program.h"

#include <array>
#include <filesystem>
#include <memory>

class Skybox {
 public:
  // Constructors
  ~Skybox() noexcept;

  Skybox(const Skybox &) = delete;
  auto operator=(const Skybox &) -> Skybox & = delete;

  Skybox(Skybox &&) noexcept;
  auto operator=(Skybox &&) noexcept -> Skybox &;

  // Methods
  static auto from_files(const std::array<std::filesystem::path, 6> &files) -> std::unique_ptr<Skybox>;

  auto draw(glm::mat4 projection, glm::mat4 view) -> void;
  auto activate_cubemap_as(int index) -> void;

 private:
  Skybox() = default;

  GLuint _texture = 0;
  GLuint _VAO = 0;
  GLuint _VBO = 0;
  std::array<std::filesystem::path, 6> _files;
  std::unique_ptr<ShaderProgram> _shader;
};