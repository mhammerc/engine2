#pragma once

#include <vector>

class VertexBufferObject {
 public:
  static VertexBufferObject from_cube();

  void define_attribute(size_t index, size_t size, size_t offset);

  void bind();
  void unbind();

  void draw();

  ~VertexBufferObject() noexcept;
  VertexBufferObject(const VertexBufferObject &) = delete;
  VertexBufferObject(VertexBufferObject &&) noexcept;

 private:
  std::vector<float> vertices;
  int stride;

  unsigned int handle;

  VertexBufferObject(std::vector<float> &&vertices, int stride);
};