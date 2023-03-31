#pragma once

#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include "../common.h"

namespace engine {

/**
 * A `Mesh` represent a set of vertices and indices.
 *
 * On creation, a vertex array object (VAO) is created alongside its buffers.
 *
 * You can retrieve the VAO for drawing yourself or using the draw() function.
 */
class Mesh {
  public:
    struct Vertex {
        vec3 position;
        vec3 normal;
        vec2 tex_coords;
        vec3 tangent = vec3(0, 0, 1);
    };

  public:
    /**
     * Generate a cube with vertex ranging from [-0.5;]
     */
    [[nodiscard]] static auto from_cube() -> std::unique_ptr<Mesh>;

    [[nodiscard]] static auto from_quad() -> std::unique_ptr<Mesh>;

    Mesh(std::string name, std::vector<Vertex>&& vertices, std::vector<u32>&& indices);
    ~Mesh() noexcept;

    Mesh(const Mesh&) = delete;
    auto operator=(const Mesh&) -> Mesh& = delete;

    Mesh(Mesh&&) noexcept;
    auto operator=(Mesh&&) noexcept -> Mesh&;

  public:
    [[nodiscard]] auto name() const -> std::string_view const;
    [[nodiscard]] auto vao() const -> u32;
    auto draw() const -> void;

  private:
    auto release() -> void;
    auto upload_buffers() -> void;

  private:
    std::string _name;
    std::vector<Vertex> _vertices;
    std::vector<u32> _indices;

    u32 _vao = 0;
    u32 _vbo = 0;
    u32 _ebo = 0;
};

}  // namespace engine