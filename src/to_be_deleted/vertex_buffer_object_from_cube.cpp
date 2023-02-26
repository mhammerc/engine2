#include <array>

#include "vertex_buffer_object.h"

//static float _vertices[] = {
static const std::array<float, 288> static_vertices = {
    -0.5F, -0.5F, -0.5F, 0.0F, 0.0F, 0.0F,  0.0F,  -1.0F, 0.5F,  -0.5F, -0.5F, 1.0F, 0.0F, 0.0F,  0.0F,  -1.0F,
    0.5F,  0.5F,  -0.5F, 1.0F, 1.0F, 0.0F,  0.0F,  -1.0F, 0.5F,  0.5F,  -0.5F, 1.0F, 1.0F, 0.0F,  0.0F,  -1.0F,
    -0.5F, 0.5F,  -0.5F, 0.0F, 1.0F, 0.0F,  0.0F,  -1.0F, -0.5F, -0.5F, -0.5F, 0.0F, 0.0F, 0.0F,  0.0F,  -1.0F,

    -0.5F, -0.5F, 0.5F,  0.0F, 0.0F, 0.0F,  0.0F,  1.0F,  0.5F,  -0.5F, 0.5F,  1.0F, 0.0F, 0.0F,  0.0F,  1.0F,
    0.5F,  0.5F,  0.5F,  1.0F, 1.0F, 0.0F,  0.0F,  1.0F,  0.5F,  0.5F,  0.5F,  1.0F, 1.0F, 0.0F,  0.0F,  1.0F,
    -0.5F, 0.5F,  0.5F,  0.0F, 1.0F, 0.0F,  0.0F,  1.0F,  -0.5F, -0.5F, 0.5F,  0.0F, 0.0F, 0.0F,  0.0F,  1.0F,

    -0.5F, 0.5F,  0.5F,  1.0F, 0.0F, -1.0F, 0.0F,  0.0F,  -0.5F, 0.5F,  -0.5F, 1.0F, 1.0F, -1.0F, 0.0F,  0.0F,
    -0.5F, -0.5F, -0.5F, 0.0F, 1.0F, -1.0F, 0.0F,  0.0F,  -0.5F, -0.5F, -0.5F, 0.0F, 1.0F, -1.0F, 0.0F,  0.0F,
    -0.5F, -0.5F, 0.5F,  0.0F, 0.0F, -1.0F, 0.0F,  0.0F,  -0.5F, 0.5F,  0.5F,  1.0F, 0.0F, -1.0F, 0.0F,  0.0F,

    0.5F,  0.5F,  0.5F,  1.0F, 0.0F, 1.0F,  0.0F,  0.0F,  0.5F,  0.5F,  -0.5F, 1.0F, 1.0F, 1.0F,  0.0F,  0.0F,
    0.5F,  -0.5F, -0.5F, 0.0F, 1.0F, 1.0F,  0.0F,  0.0F,  0.5F,  -0.5F, -0.5F, 0.0F, 1.0F, 1.0F,  0.0F,  0.0F,
    0.5F,  -0.5F, 0.5F,  0.0F, 0.0F, 1.0F,  0.0F,  0.0F,  0.5F,  0.5F,  0.5F,  1.0F, 0.0F, 1.0F,  0.0F,  0.0F,

    -0.5F, -0.5F, -0.5F, 0.0F, 1.0F, 0.0F,  -1.0F, 0.0F,  0.5F,  -0.5F, -0.5F, 1.0F, 1.0F, 0.0F,  -1.0F, 0.0F,
    0.5F,  -0.5F, 0.5F,  1.0F, 0.0F, 0.0F,  -1.0F, 0.0F,  0.5F,  -0.5F, 0.5F,  1.0F, 0.0F, 0.0F,  -1.0F, 0.0F,
    -0.5F, -0.5F, 0.5F,  0.0F, 0.0F, 0.0F,  -1.0F, 0.0F,  -0.5F, -0.5F, -0.5F, 0.0F, 1.0F, 0.0F,  -1.0F, 0.0F,

    -0.5F, 0.5F,  -0.5F, 0.0F, 1.0F, 0.0F,  1.0F,  0.0F,  0.5F,  0.5F,  -0.5F, 1.0F, 1.0F, 0.0F,  1.0F,  0.0F,
    0.5F,  0.5F,  0.5F,  1.0F, 0.0F, 0.0F,  1.0F,  0.0F,  0.5F,  0.5F,  0.5F,  1.0F, 0.0F, 0.0F,  1.0F,  0.0F,
    -0.5F, 0.5F,  0.5F,  0.0F, 0.0F, 0.0F,  1.0F,  0.0F,  -0.5F, 0.5F,  -0.5F, 0.0F, 1.F,  0.0F,  1.0F,  0.0F,
};

const static int static_stride = 8;

auto VertexBufferObject::from_cube() -> VertexBufferObject {
    std::vector<float> _vertices(static_vertices.begin(), static_vertices.end());

    auto vbo = VertexBufferObject(std::move(_vertices), static_stride);

    // vertices coordinates
    vbo.define_attribute(0, 3, 0);

    // uv coordinates
    vbo.define_attribute(1, 2, 3);

    // normal coordinate
    vbo.define_attribute(2, 3, 5);

    return vbo;
}