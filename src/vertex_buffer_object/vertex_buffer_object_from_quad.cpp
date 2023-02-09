#include "vertex_buffer_object.h"

#include <array>

//static float _vertices[] = {
static const std::array<float, 24> static_vertices = {
	-1.0F, -1.0F, 0.0F, 0.0F,
	1.0F, -1.0F, 1.0F, 0.0F,
	1.0F, 1.0F, 1.0F, 1.0F,
	1.0F, 1.0F, 1.0F, 1.0F,
	-1.0F, 1.0F, 0.0F, 1.0F,
	-1.0F, -1.0F, 0.0F, 0.0F};

static const int static_stride = 4;

auto VertexBufferObject::from_quad() -> VertexBufferObject {
  std::vector<float> _vertices(static_vertices.begin(), static_vertices.end());

  auto vbo = VertexBufferObject(std::move(_vertices), static_stride);

  // vertices coordinates
  vbo.define_attribute(0, 2, 0);

  // uv coordinates
  vbo.define_attribute(1, 2, 2);

  return vbo;
}