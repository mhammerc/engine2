#include <array>

#include "mesh.h"

using namespace engine;

static constexpr std::array<Mesh::Vertex, 4> quad_vertices = {
    {
        {vec3(-1.0F, -1.0F, 0.F), vec3(0.F, 0.F, -1.F), vec2(0.0F, 0.0F)},
        {vec3(1.0F, -1.0F, 0.F), vec3(0.F, 0.F, -1.F), vec2(1.0F, 0.0F)},
        {vec3(1.0F, 1.0F, 0.F), vec3(0.F, 0.F, -1.F), vec2(1.0F, 1.0F)},
        {vec3(-1.0F, 1.0F, 0.F), vec3(0.F, 0.F, -1.F), vec2(0.0F, 1.0F)},
    },
};

// clang-format off
static const std::array<u32, 6> quad_indices =
{
    // lower right triangle
    2, 0, 1,
    // lower left triangle
    3, 0, 2,
};
// clang-format on

// auto Mesh::from_cube() -> std::unique_ptr<Mesh> {}

auto Mesh::from_quad() -> std::unique_ptr<Mesh> {
    auto vertices = std::vector<Mesh::Vertex>(quad_vertices.begin(), quad_vertices.end());
    auto indices = std::vector<u32>(quad_indices.begin(), quad_indices.end());

    auto mesh = std::make_unique<Mesh>("quad", std::move(vertices), std::move(indices));

    return mesh;
}