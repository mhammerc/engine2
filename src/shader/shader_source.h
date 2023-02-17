#pragma once

#include <filesystem>
#include <optional>
#include <string>

#include "../opengl/opengl.h"

struct ShaderSource {
    enum Type { UNKNOWN, FRAGMENT, VERTEX, GEOMETRY };

    static auto from_file(Type type, const std::filesystem::path& path) -> std::optional<ShaderSource>;

    ~ShaderSource() noexcept;

    ShaderSource(const ShaderSource&) = delete;
    auto operator=(const ShaderSource&) -> ShaderSource& = delete;

    ShaderSource(ShaderSource&&) noexcept;
    auto operator=(ShaderSource&&) noexcept -> ShaderSource&;

    /**
   * @return true on success, false otherwise.
   */
    auto compile() -> bool;

    auto type() -> Type;
    auto source() -> const std::string&;
    auto path() -> const std::filesystem::path&;
    auto handle() -> GLuint;

  private:
    ShaderSource() = default;

    Type _type = Type::UNKNOWN;
    std::string _source;
    std::filesystem::path _path;

    GLuint _handle = 0;
};