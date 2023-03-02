#include <array>

#include "mesh.h"

using namespace engine;

static const std::array<Mesh::Vertex, 36> cube_vertices = {
    {
        {vec3(-1.0F, 1.0F, -1.0F), vec3(0.F), vec2(0.F)},  {vec3(-1.0F, -1.0F, -1.0F), vec3(0.F), vec2(0.F)},
        {vec3(1.0F, -1.0F, -1.0F), vec3(0.F), vec2(0.F)},  {vec3(1.0F, -1.0F, -1.0F), vec3(0.F), vec2(0.F)},
        {vec3(1.0F, 1.0F, -1.0F), vec3(0.F), vec2(0.F)},   {vec3(-1.0F, 1.0F, -1.0F), vec3(0.F), vec2(0.F)},

        {vec3(-1.0F, -1.0F, 1.0F), vec3(0.F), vec2(0.F)},  {vec3(-1.0F, -1.0F, -1.0F), vec3(0.F), vec2(0.F)},
        {vec3(-1.0F, 1.0F, -1.0F), vec3(0.F), vec2(0.F)},  {vec3(-1.0F, 1.0F, -1.0F), vec3(0.F), vec2(0.F)},
        {vec3(-1.0F, 1.0F, 1.0F), vec3(0.F), vec2(0.F)},   {vec3(-1.0F, -1.0F, 1.0F), vec3(0.F), vec2(0.F)},

        {vec3(1.0F, -1.0F, -1.0F), vec3(0.F), vec2(0.F)},  {vec3(1.0F, -1.0F, 1.0F), vec3(0.F), vec2(0.F)},
        {vec3(1.0F, 1.0F, 1.0F), vec3(0.F), vec2(0.F)},    {vec3(1.0F, 1.0F, 1.0F), vec3(0.F), vec2(0.F)},
        {vec3(1.0F, 1.0F, -1.0F), vec3(0.F), vec2(0.F)},   {vec3(1.0F, -1.0F, -1.0F), vec3(0.F), vec2(0.F)},

        {vec3(-1.0F, -1.0F, 1.0F), vec3(0.F), vec2(0.F)},  {vec3(-1.0F, 1.0F, 1.0F), vec3(0.F), vec2(0.F)},
        {vec3(1.0F, 1.0F, 1.0F), vec3(0.F), vec2(0.F)},    {vec3(1.0F, 1.0F, 1.0F), vec3(0.F), vec2(0.F)},
        {vec3(1.0F, -1.0F, 1.0F), vec3(0.F), vec2(0.F)},   {vec3(-1.0F, -1.0F, 1.0F), vec3(0.F), vec2(0.F)},

        {vec3(-1.0F, 1.0F, -1.0F), vec3(0.F), vec2(0.F)},  {vec3(1.0F, 1.0F, -1.0F), vec3(0.F), vec2(0.F)},
        {vec3(1.0F, 1.0F, 1.0F), vec3(0.F), vec2(0.F)},    {vec3(1.0F, 1.0F, 1.0F), vec3(0.F), vec2(0.F)},
        {vec3(-1.0F, 1.0F, 1.0F), vec3(0.F), vec2(0.F)},   {vec3(-1.0F, 1.0F, -1.0F), vec3(0.F), vec2(0.F)},

        {vec3(-1.0F, -1.0F, -1.0F), vec3(0.F), vec2(0.F)}, {vec3(-1.0F, -1.0F, 1.0F), vec3(0.F), vec2(0.F)},
        {vec3(1.0F, -1.0F, -1.0F), vec3(0.F), vec2(0.F)},  {vec3(1.0F, -1.0F, -1.0F), vec3(0.F), vec2(0.F)},
        {vec3(-1.0F, -1.0F, 1.0F), vec3(0.F), vec2(0.F)},  {vec3(1.0F, -1.0F, 1.0F), vec3(0.F), vec2(0.F)},
    },
};

static const std::array<u32, 36> cube_indices = {
    0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 16, 17,
    18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35,
};

auto Mesh::from_cube() -> std::unique_ptr<Mesh> {
    auto vertices = std::vector<Mesh::Vertex>(cube_vertices.begin(), cube_vertices.end());
    auto indices = std::vector<u32>(cube_indices.begin(), cube_indices.end());

    auto mesh = std::make_unique<Mesh>(std::move(vertices), std::move(indices));

    return mesh;
}