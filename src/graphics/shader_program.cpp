#include "shader_program.h"

#include <array>

#include "shader_source.h"
#include "spdlog/spdlog.h"

using namespace engine;

ShaderProgram::ShaderProgram(ShaderProgram&& from) noexcept : _handle(from._handle) {
    from._handle = 0;
}

auto ShaderProgram::operator=(ShaderProgram&& from) noexcept -> ShaderProgram& {
    release();

    _handle = from._handle;

    from._handle = 0;

    return *this;
}

ShaderProgram::~ShaderProgram() noexcept {
    release();
}

auto ShaderProgram::release() -> void {
    if (_handle != 0) {
        glDeleteProgram(_handle);
        _handle = 0;
    }
}

auto ShaderProgram::from_name(const std::string& name) -> std::unique_ptr<ShaderProgram> {
    std::filesystem::path folder = "../shaders";

    auto path = folder / name;

    auto vertex = std::filesystem::path(path) += ".vert";
    auto fragment = std::filesystem::path(path) += ".frag";
    auto geometry = std::filesystem::path(path) += ".geo";

    if (std::filesystem::exists(geometry)) {
        return ShaderProgram::from_vertex_and_fragment_and_geometry(name, vertex, fragment, geometry);
    }

    return ShaderProgram::from_vertex_and_fragment(name, vertex, fragment);
}

auto ShaderProgram::from_vertex_and_fragment(
    const std::string& name,
    const std::filesystem::path& vertex_file,
    const std::filesystem::path& fragment_file
) -> std::unique_ptr<ShaderProgram> {
    auto vertex = ShaderSource::from_file(ShaderSource::VERTEX, vertex_file);

    if (!vertex) {
        return nullptr;
    }

    if (!vertex->compile()) {
        return nullptr;
    }

    auto fragment = ShaderSource::from_file(ShaderSource::FRAGMENT, fragment_file);

    if (!fragment) {
        return nullptr;
    }

    if (!fragment->compile()) {
        return nullptr;
    }

    u32 const handle = glCreateProgram();

    glAttachShader(handle, vertex->handle());
    glAttachShader(handle, fragment->handle());

    auto program = std::make_unique<ShaderProgram>(ShaderProgram {});
    program->_name = name;
    program->_handle = handle;

    if (!program->link()) {
        return nullptr;
    }

    return program;
}

auto ShaderProgram::from_vertex_and_fragment_and_geometry(
    const std::string& name,
    const std::filesystem::path& vertex_file,
    const std::filesystem::path& fragment_file,
    const std::filesystem::path& geometry_file
) -> std::unique_ptr<ShaderProgram> {
    auto vertex = ShaderSource::from_file(ShaderSource::VERTEX, vertex_file);

    if (!vertex) {
        return nullptr;
    }

    if (!vertex->compile()) {
        return nullptr;
    }

    auto geometry = ShaderSource::from_file(ShaderSource::GEOMETRY, geometry_file);

    if (!geometry) {
        return nullptr;
    }

    if (!geometry->compile()) {
        return nullptr;
    }

    auto fragment = ShaderSource::from_file(ShaderSource::FRAGMENT, fragment_file);

    if (!fragment) {
        return nullptr;
    }

    if (!fragment->compile()) {
        return nullptr;
    }

    GLuint const handle = glCreateProgram();

    glAttachShader(handle, vertex->handle());
    glAttachShader(handle, geometry->handle());
    glAttachShader(handle, fragment->handle());

    auto program = std::make_unique<ShaderProgram>(ShaderProgram {});
    program->_name = name;
    program->_handle = handle;

    if (!program->link()) {
        return nullptr;
    }

    return program;
}

auto ShaderProgram::link() -> bool {
    glLinkProgram(_handle);

    std::array<char, 1024> log {};
    int log_size = 0;
    glGetProgramInfoLog(_handle, 1024, &log_size, log.data());

    // Logs may contain compilation errors, warning, etc.
    if (log_size > 0) {
        spdlog::warn("Log of ShaderProgram:\n{}", log.data());
    }

    int success = 0;
    glGetProgramiv(_handle, GL_LINK_STATUS, &success);

    if (success == 0) {
        spdlog::error("Could not create or link ShaderProgram.");

        return false;
    }

    return true;
}

void ShaderProgram::set_uniform(const std::string& name, i32 value) {
    int const uniformLocation = glGetUniformLocation(_handle, name.c_str());

    if (uniformLocation == -1) {
        spdlog::warn("Uniform '{}' not found when trying to set it.", name);
        return;
    }

    glProgramUniform1i(_handle, uniformLocation, value);

    GLenum const error = glGetError();

    if (error != GL_NO_ERROR) {
        spdlog::error("1 {}", error == GL_INVALID_OPERATION);
    }
}

void ShaderProgram::set_uniform(const std::string& name, float value) {
    int const uniformLocation = glGetUniformLocation(_handle, name.c_str());

    if (uniformLocation == -1) {
        spdlog::warn("Uniform '{}' not found when trying to set it.", name);
        return;
    }

    glProgramUniform1f(_handle, uniformLocation, value);

    GLenum const error = glGetError();

    if (error != GL_NO_ERROR) {
        spdlog::error("2 {}", error == GL_INVALID_OPERATION);
    }
}

void ShaderProgram::set_uniform(const std::string& name, glm::vec4 value) {
    int const uniformLocation = glGetUniformLocation(_handle, name.c_str());

    if (uniformLocation == -1) {
        spdlog::warn("Uniform '{}' not found when trying to set it.", name);
        return;
    }

    glProgramUniform4f(_handle, uniformLocation, value.x, value.y, value.z, value.w);

    GLenum const error = glGetError();

    if (error != GL_NO_ERROR) {
        spdlog::error("3 {}", error == GL_INVALID_OPERATION);
    }
}

void ShaderProgram::set_uniform(const std::string& name, glm::vec3 value) {
    int const uniformLocation = glGetUniformLocation(_handle, name.c_str());

    if (uniformLocation == -1) {
        spdlog::warn("Uniform '{}' not found when trying to set it.", name);
        return;
    }

    glProgramUniform3f(_handle, uniformLocation, value.x, value.y, value.z);

    GLenum const error = glGetError();

    if (error != GL_NO_ERROR) {
        spdlog::error("3 {}", error == GL_INVALID_OPERATION);
    }
}

void ShaderProgram::set_uniform(const std::string& name, glm::mat4 value) {
    int const uniformLocation = glGetUniformLocation(_handle, name.c_str());

    if (uniformLocation == -1) {
        spdlog::warn("Uniform '{}' not found when trying to set it.", name);
        return;
    }

    glProgramUniformMatrix4fv(_handle, uniformLocation, 1, GL_FALSE, glm::value_ptr(value));

    GLenum const error = glGetError();

    if (error != GL_NO_ERROR) {
        spdlog::error("4 {}", error == GL_INVALID_OPERATION);
    }
}

void ShaderProgram::set_uniform(const std::string& name, glm::mat3 value) {
    int const uniformLocation = glGetUniformLocation(_handle, name.c_str());

    if (uniformLocation == -1) {
        spdlog::warn("Uniform '{}' not found when trying to set it.", name);
        return;
    }

    glProgramUniformMatrix3fv(_handle, uniformLocation, 1, GL_FALSE, glm::value_ptr(value));

    GLenum const error = glGetError();

    if (error != GL_NO_ERROR) {
        spdlog::error("4 {}", error == GL_INVALID_OPERATION);
    }
}

void ShaderProgram::set_uniform(const std::string& name, bool value) {
    if (value) {
        set_uniform(name, 1);
    } else {
        set_uniform(name, 0);
    }
}

auto ShaderProgram::bind() -> void {
    glUseProgram(_handle);
}

auto ShaderProgram::unbind() -> void {
    glUseProgram(0);
}

auto ShaderProgram::name() const -> const std::string_view {
    return _name;
}