#pragma once

#include <filesystem>
#include <memory>
#include <string>
#include <string_view>

#include "../common.h"

namespace engine {

class ShaderProgram {
  public:
    static auto from_name(std::string const& name) -> std::unique_ptr<ShaderProgram>;

    static auto from_vertex_and_fragment(
        const std::string& name,
        const std::filesystem::path& vertex,
        const std::filesystem::path& fragment
    ) -> std::unique_ptr<ShaderProgram>;

    static auto from_vertex_and_fragment_and_geometry(
        const std::string& name,
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

    /**
     * Return `true` if the shader successfully recompile and relink.
     */
    auto reload() -> bool;

    [[nodiscard]] auto name() const -> const std::string_view;

  private:
    ShaderProgram() = default;
    auto release() -> void;

  private:
    u32 _handle = 0;
    std::string _name;

  private:
    auto link() -> bool;
};

}  // namespace engine