#pragma once

#include <cstddef>
#include <vector>

class VertexBufferObject {
  public:
    static auto from_cube() -> VertexBufferObject;
    static auto from_quad() -> VertexBufferObject;

    auto define_attribute(size_t index, size_t size, size_t offset) -> void;

    auto bind() -> void;
    auto unbind() -> void;

    auto draw() -> void;

    ~VertexBufferObject() noexcept;
    VertexBufferObject(const VertexBufferObject&) = delete;
    VertexBufferObject(VertexBufferObject&&) noexcept;
    auto operator=(const VertexBufferObject&) -> VertexBufferObject& = delete;
    auto operator=(VertexBufferObject&&) noexcept -> VertexBufferObject&;

  private:
    std::vector<float> vertices;
    int stride;

    unsigned int handle;

    VertexBufferObject(std::vector<float>&& vertices, int stride);

    auto deleteHandle() -> void;
};