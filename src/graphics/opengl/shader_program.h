#pragma once

#include <filesystem>
#include <memory>
#include <string>

#include "../../common.h"

class ShaderProgram {
  public:
    static auto from_vertex_and_fragment(const std::filesystem::path& vertex, const std::filesystem::path& fragment)
        -> std::unique_ptr<ShaderProgram>;
    static auto from_vertex_and_fragment_and_geometry(
        const std::filesystem::path& vertex,
        const std::filesystem::path& fragment,
        const std::filesystem::path& geometry
    ) -> std::unique_ptr<ShaderProgram>;

    ~ShaderProgram() noexcept;

    ShaderProgram(const ShaderProgram&) = delete;
    auto operator=(const ShaderProgram&) -> ShaderProgram& = delete;

    ShaderProgram(ShaderProgram&&) noexcept;
    auto operator=(ShaderProgram&&) noexcept -> ShaderProgram&;

  public:
    void set_uniform(const std::string& name, float value);
    void set_uniform(const std::string& name, i32 value);
    void set_uniform(const std::string& name, u32 value);
    void set_uniform(const std::string& name, bool value);
    void set_uniform(const std::string& name, vec4 value);
    void set_uniform(const std::string& name, vec3 value);
    void set_uniform(const std::string& name, vec2 value);
    void set_uniform(const std::string& name, mat4 value);
    void set_uniform(const std::string& name, mat3 value);

    auto bind() -> void;
    auto unbind() -> void;

  private:
    ShaderProgram() = default;
    auto release() -> void;

  private:
    u32 _handle = 0;

  private:
    auto link() -> bool;
};