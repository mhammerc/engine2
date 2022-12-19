#include "shader_program.h"
#include "shader_source.h"
#include "spdlog/spdlog.h"
#include <array>

ShaderProgram::ShaderProgram(ShaderProgram &&from) noexcept {
  this->handle = from.handle;

  from.handle = 0;
}

ShaderProgram::~ShaderProgram() noexcept {
  if (handle) {
	glDeleteProgram(handle);
	handle = 0;
  }
}

std::optional<ShaderProgram> ShaderProgram::from_vertex_and_fragment(const std::filesystem::path &vertex_file,
																	 const std::filesystem::path &fragment_file) {
  auto vertex = ShaderSource::from_file(ShaderSource::VERTEX, vertex_file);

  if (!vertex.has_value()) return std::nullopt;

  if (!vertex->compile()) return std::nullopt;

  auto fragment = ShaderSource::from_file(ShaderSource::FRAGMENT, fragment_file);

  if (!fragment.has_value()) return std::nullopt;

  if (!fragment->compile()) return std::nullopt;

  GLuint handle = glCreateProgram();

  glAttachShader(handle, vertex->handle);
  glAttachShader(handle, fragment->handle);

  glLinkProgram(handle);

  std::array<char, 1024> log{};
  int log_size = 0;
  glGetProgramInfoLog(handle, 1024, &log_size, log.data());

  // Logs may contain compilation errors, warning, etc.
  if (log_size > 0) {
	spdlog::warn("Log of ShaderProgram {} and {}:\n{}", vertex_file.string(), fragment_file.string(), log.data());
  }

  int success = 0;
  glGetProgramiv(handle, GL_LINK_STATUS, &success);

  if (!success) {
	spdlog::error("Could not create or link ShaderProgram {} and {}.", vertex_file.string(), fragment_file.string());

	return std::nullopt;
  }

  ShaderProgram program{};
  program.handle = handle;

  return program;
}

void ShaderProgram::setUniform(const std::string &name, int value) {
  int uniformLocation = glGetUniformLocation(handle, name.c_str());

  if (uniformLocation == -1) {
	spdlog::warn("Uniform '{}' not found when trying to set it.", name);
  }

  glUseProgram(handle);
  glUniform1i(uniformLocation, value);

  GLenum error = glGetError();

  if (error != GL_NO_ERROR) {
	spdlog::error("1 {}", error == GL_INVALID_OPERATION);
  }
}

void ShaderProgram::setUniform(const std::string &name, float value) {
  int uniformLocation = glGetUniformLocation(handle, name.c_str());

  if (uniformLocation == -1) {
	spdlog::warn("Uniform '{}' not found when trying to set it.", name);
  }

  glUseProgram(handle);
  glUniform1f(uniformLocation, value);

  GLenum error = glGetError();

  if (error != GL_NO_ERROR) {
	spdlog::error("2 {}", error == GL_INVALID_OPERATION);
  }
}

void ShaderProgram::setUniform(const std::string &name, glm::vec4 value) {
  int uniformLocation = glGetUniformLocation(handle, name.c_str());

  if (uniformLocation == -1) {
	spdlog::warn("Uniform '{}' not found when trying to set it.", name);
  }

  glUseProgram(handle);
  glUniform4f(uniformLocation, value.x, value.y, value.z, value.w);

  GLenum error = glGetError();

  if (error != GL_NO_ERROR) {
	spdlog::error("3 {}", error == GL_INVALID_OPERATION);
  }
}

void ShaderProgram::setUniform(const std::string &name, glm::mat4 value) {
  int uniformLocation = glGetUniformLocation(handle, name.c_str());

  if (uniformLocation == -1) {
	spdlog::warn("Uniform '{}' not found when trying to set it.", name);
  }

  glUseProgram(handle);
  glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(value));

  GLenum error = glGetError();

  if (error != GL_NO_ERROR) {
	spdlog::error("4 {}", error == GL_INVALID_OPERATION);
  }
}

void ShaderProgram::setUniform(const std::string &name, bool value) {
  if (value) setUniform(name, 1);
  else setUniform(name, 0);
}