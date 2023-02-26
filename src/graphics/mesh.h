#pragma once

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
    };

  public:
    Mesh(std::vector<Vertex>&& vertices, std::vector<u32>&& indices);
    ~Mesh() noexcept;

    Mesh(const Mesh&) = delete;
    auto operator=(const Mesh&) -> Mesh& = delete;

    Mesh(Mesh&&) noexcept;
    auto operator=(Mesh&&) noexcept -> Mesh&;

  public:
    [[nodiscard]] auto vao() const -> u32;
    auto draw() const -> void;

  private:
    auto release() -> void;
    auto upload_buffers() -> void;

  private:
    std::vector<Vertex> _vertices;
    std::vector<u32> _indices;

    u32 _vao = 0;
    u32 _vbo = 0;
    u32 _ebo = 0;
};

}  // namespace engine