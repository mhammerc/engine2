#include "shader_source.h"

#include "../utils/read_file_to_string.h"

#include <array>
#include <spdlog/spdlog.h>
#include <stdexcept>

static auto shader_type(ShaderSource::Type type) -> GLenum {
  GLenum shaderType;

  switch (type) {
	case ShaderSource::FRAGMENT:
	  shaderType = GL_FRAGMENT_SHADER;
	  break;
	case ShaderSource::VERTEX:
	  shaderType = GL_VERTEX_SHADER;
	  break;
	default:
	  throw std::logic_error("not implemented");
	  break;
  }

  return shaderType;
}

ShaderSource::~ShaderSource() noexcept {
  if (_handle != 0) {
	glDeleteShader(_handle);
	_handle = 0;
  }
}

ShaderSource::ShaderSource(ShaderSource &&from) noexcept : _type(from._type),
														   _source(std::move(from._source)),
														   _path(std::move(from._path)),
														   _handle(from._handle) {
  from._handle = 0;
}

auto ShaderSource::operator=(ShaderSource &&from) noexcept -> ShaderSource & {
  _type = from._type;
  _source = from._source;
  _path = from._path;
  _handle = from._handle;

  from._handle = 0;

  return *this;
}

auto ShaderSource::from_file(Type type, const std::filesystem::path &path) -> std::optional<ShaderSource> {
  auto file = read_file_to_string(path);

  if (!file.has_value()) {
	spdlog::error("Could not read file {}", path.c_str());
	return std::nullopt;
  }

  GLuint const handle = glCreateShader(shader_type(type));

  if (handle == 0) {
	return std::nullopt;
  }

  ShaderSource source{};
  source._type = type;
  source._source = *file;
  source._path = path;
  source._handle = handle;

  return source;
}

auto ShaderSource::compile() -> bool {
  const char *string = _source.c_str();
  int const length = static_cast<int>(_source.length());
  glShaderSource(_handle, 1, &string, &length);

  glCompileShader(_handle);

  std::array<char, 1024> log{};
  int log_size = 0;
  glGetShaderInfoLog(_handle, 1024, &log_size, log.data());

  // Logs may contain compilation errors, warning, etc.
  if (log_size > 0) {
	spdlog::warn("Log of ShaderSource {}:\n{}", _path.string(), log.data());
  }

  int success = 0;
  glGetShaderiv(_handle, GL_COMPILE_STATUS, &success);

  if (!success) {
	spdlog::error("Could not create or compile shader {}.", _path.string());

	return false;
  }

  return true;
}

auto ShaderSource::type() -> Type {
  return _type;
}

auto ShaderSource::source() -> const std::string & {
  return _source;
}

auto ShaderSource::path() -> const std::filesystem::path & {
  return _path;
}

auto ShaderSource::handle() -> GLuint {
  return _handle;
}