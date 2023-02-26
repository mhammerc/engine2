#include "shader_source.h"

#include <array>
#include <stdexcept>

#include "../../utils/read_file_to_string.h"
#include "spdlog/spdlog.h"

using namespace engine;

static auto shader_type(ShaderSource::Type type) -> GLenum {
    GLenum shaderType = 0;

    switch (type) {
        case ShaderSource::FRAGMENT:
            shaderType = GL_FRAGMENT_SHADER;
            break;
        case ShaderSource::VERTEX:
            shaderType = GL_VERTEX_SHADER;
            break;
        case ShaderSource::GEOMETRY:
            shaderType = GL_GEOMETRY_SHADER;
            break;
        default:
            throw std::logic_error("not implemented");
            break;
    }

    return shaderType;
}

ShaderSource::~ShaderSource() noexcept {
    release();
}

auto ShaderSource::release() -> void {
    if (_handle != 0) {
        glDeleteShader(_handle);
        _handle = 0;
    }
}

ShaderSource::ShaderSource(ShaderSource&& from) noexcept :
    _type(from._type),
    _source(std::move(from._source)),
    _path(std::move(from._path)),
    _handle(from._handle) {
    from._handle = 0;
}

auto ShaderSource::operator=(ShaderSource&& from) noexcept -> ShaderSource& {
    release();

    _type = from._type;
    _source = from._source;
    _path = from._path;
    _handle = from._handle;

    from._handle = 0;

    return *this;
}

auto ShaderSource::from_file(Type type, const std::filesystem::path& path) -> std::unique_ptr<ShaderSource> {
    auto file = read_file_to_string(path);

    if (!file.has_value()) {
        spdlog::error("Could not read file {}", path.c_str());
        return nullptr;
    }

    u32 handle = glCreateShader(shader_type(type));

    if (handle == 0) {
        return nullptr;
    }

    auto source = std::make_unique<ShaderSource>(ShaderSource());
    source->_type = type;
    source->_source = std::move(*file);
    source->_path = path;
    source->_handle = handle;

    return source;
}

auto ShaderSource::compile() -> bool {
    const char* string = _source.c_str();
    int const length = static_cast<int>(_source.length());
    glShaderSource(_handle, 1, &string, &length);

    glCompileShader(_handle);

    std::array<char, 1024> log {};
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

auto ShaderSource::type() const -> Type {
    return _type;
}

auto ShaderSource::source() const -> std::string {
    return _source;
}

auto ShaderSource::path() const -> std::filesystem::path {
    return _path;
}

auto ShaderSource::handle() const -> u32 {
    return _handle;
}