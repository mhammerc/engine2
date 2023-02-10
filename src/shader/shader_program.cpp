#include "shader_program.h"

#include "shader_source.h"

#include <array>
#include <spdlog/spdlog.h>

ShaderProgram::ShaderProgram(ShaderProgram &&from) noexcept : handle(from.handle) {
  from.handle = 0;
}

auto ShaderProgram::operator=(ShaderProgram &&from) noexcept -> ShaderProgram & {
  handle = from.handle;

  from.handle = 0;

  return *this;
}

ShaderProgram::~ShaderProgram() noexcept {
  if (handle != 0) {
	glDeleteProgram(handle);
	handle = 0;
  }
}

auto ShaderProgram::from_vertex_and_fragment(const std::filesystem::path &vertex_file,
											 const std::filesystem::path &fragment_file) -> std::unique_ptr<ShaderProgram> {
  auto vertex = ShaderSource::from_file(ShaderSource::VERTEX, vertex_file);

  if (!vertex.has_value()) {
	return nullptr;
  }

  if (!vertex->compile()) {
	return nullptr;
  }

  auto fragment = ShaderSource::from_file(ShaderSource::FRAGMENT, fragment_file);

  if (!fragment.has_value()) {
	return nullptr;
  }

  if (!fragment->compile()) {
	return nullptr;
  }

  GLuint const handle = glCreateProgram();

  glAttachShader(handle, vertex->handle());
  glAttachShader(handle, fragment->handle());

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

  if (success == 0) {
	spdlog::error("Could not create or link ShaderProgram {} and {}.", vertex_file.string(), fragment_file.string());

	return nullptr;
  }

  auto program = std::make_unique<ShaderProgram>(ShaderProgram{});
  program->handle = handle;

  return program;
}

void ShaderProgram::setUniform(const std::string &name, int value) {
  int const uniformLocation = glGetUniformLocation(handle, name.c_str());

  if (uniformLocation == -1) {
	spdlog::warn("Uniform '{}' not found when trying to set it.", name);
	return;
  }

  glUseProgram(handle);
  glUniform1i(uniformLocation, value);

  GLenum const error = glGetError();

  if (error != GL_NO_ERROR) {
	spdlog::error("1 {}", error == GL_INVALID_OPERATION);
  }
}

void ShaderProgram::setUniform(const std::string &name, float value) {
  int const uniformLocation = glGetUniformLocation(handle, name.c_str());

  if (uniformLocation == -1) {
	spdlog::warn("Uniform '{}' not found when trying to set it.", name);
	return;
  }

  glUseProgram(handle);
  glUniform1f(uniformLocation, value);

  GLenum const error = glGetError();

  if (error != GL_NO_ERROR) {
	spdlog::error("2 {}", error == GL_INVALID_OPERATION);
  }
}

void ShaderProgram::setUniform(const std::string &name, glm::vec4 value) {
  int const uniformLocation = glGetUniformLocation(handle, name.c_str());

  if (uniformLocation == -1) {
	spdlog::warn("Uniform '{}' not found when trying to set it.", name);
	return;
  }

  glUseProgram(handle);
  glUniform4f(uniformLocation, value.x, value.y, value.z, value.w);

  GLenum const error = glGetError();

  if (error != GL_NO_ERROR) {
	spdlog::error("3 {}", error == GL_INVALID_OPERATION);
  }
}

void ShaderProgram::setUniform(const std::string &name, glm::vec3 value) {
  int const uniformLocation = glGetUniformLocation(handle, name.c_str());

  if (uniformLocation == -1) {
	spdlog::warn("Uniform '{}' not found when trying to set it.", name);
	return;
  }

  glUseProgram(handle);
  glUniform3f(uniformLocation, value.x, value.y, value.z);

  GLenum const error = glGetError();

  if (error != GL_NO_ERROR) {
	spdlog::error("3 {}", error == GL_INVALID_OPERATION);
  }
}

void ShaderProgram::setUniform(const std::string &name, glm::mat4 value) {
  int const uniformLocation = glGetUniformLocation(handle, name.c_str());

  if (uniformLocation == -1) {
	spdlog::warn("Uniform '{}' not found when trying to set it.", name);
	return;
  }

  glUseProgram(handle);
  glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(value));

  GLenum const error = glGetError();

  if (error != GL_NO_ERROR) {
	spdlog::error("4 {}", error == GL_INVALID_OPERATION);
  }
}

void ShaderProgram::setUniform(const std::string &name, glm::mat3 value) {
  int const uniformLocation = glGetUniformLocation(handle, name.c_str());

  if (uniformLocation == -1) {
	spdlog::warn("Uniform '{}' not found when trying to set it.", name);
	return;
  }

  glUseProgram(handle);
  glUniformMatrix3fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(value));

  GLenum const error = glGetError();

  if (error != GL_NO_ERROR) {
	spdlog::error("4 {}", error == GL_INVALID_OPERATION);
  }
}

void ShaderProgram::setUniform(const std::string &name, bool value) {
  if (value) {
	setUniform(name, 1);
  } else {
	setUniform(name, 0);
  }
}

auto ShaderProgram::bind() -> void {
  glUseProgram(handle);
}

auto ShaderProgram::unbind() -> void {
  glUseProgram(0);
}