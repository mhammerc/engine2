#include <stdexcept>
#include <array>
#include <spdlog/spdlog.h>

#include "../utils/read_file_to_string.h"
#include "shader_source.h"

static GLenum shader_type(ShaderSource::Type type) {
  GLenum shaderType;

  switch (type) {
	case ShaderSource::FRAGMENT: shaderType = GL_FRAGMENT_SHADER;
	  break;
	case ShaderSource::VERTEX: shaderType = GL_VERTEX_SHADER;
	  break;
	default: throw std::logic_error("not implemented");
	  break;
  }

  return shaderType;
}

ShaderSource::~ShaderSource() noexcept {
  if (handle != 0) {
	glDeleteShader(handle);
	handle = 0;
  }
}

ShaderSource::ShaderSource(ShaderSource &&from) noexcept {
  this->type = from.type;
  this->source = from.source;
  this->path = from.path;
  this->handle = from.handle;

  from.handle = 0;
}

std::optional<ShaderSource> ShaderSource::from_file(Type type, const std::filesystem::path &path) {
  auto file = read_file_to_string(path);

  if (!file.has_value()) {
	spdlog::error("Could not read file {}", path.c_str());
	return std::nullopt;
  }

  GLuint handle = glCreateShader(shader_type(type));

  if (handle == 0) return std::nullopt;

  ShaderSource source{};
  source.type = type;
  source.source = *file;
  source.path = path;
  source.handle = handle;

  return std::make_optional<ShaderSource>(std::move(source));
}

bool ShaderSource::compile() {
  const char *string = source.c_str();
  int length = static_cast<int>(source.length());
  glShaderSource(handle, 1, &string, &length);

  glCompileShader(handle);

  std::array<char, 1024> log{};
  int log_size = 0;
  glGetShaderInfoLog(handle, 1024, &log_size, log.data());

  // Logs may contain compilation errors, warning, etc.
  if (log_size > 0) {
	spdlog::warn("Log of ShaderSource {}:\n{}", path.string(), log.data());
  }

  int success = 0;
  glGetShaderiv(handle, GL_COMPILE_STATUS, &success);

  if (!success) {
	spdlog::error("Could not create or compile shader {}.", path.string());

	return false;
  }

  return true;
}