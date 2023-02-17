#pragma once

#include <memory>

#include "../vertex_buffer_object/vertex_buffer_object.h"

class VertexArrayObject {
  public:
    static auto from_cube() -> std::shared_ptr<VertexArrayObject>;
    static auto from_quad() -> std::unique_ptr<VertexArrayObject>;

    auto bind() -> void;
    auto unbind() -> void;

    auto draw() -> void;

    ~VertexArrayObject() noexcept;
    VertexArrayObject(const VertexArrayObject&) = delete;
    VertexArrayObject(VertexArrayObject&&) noexcept;
    auto operator=(const VertexArrayObject&) -> VertexArrayObject& = delete;
    auto operator=(VertexArrayObject&&) noexcept -> VertexArrayObject&;

  private:
    unsigned int handle;
    std::shared_ptr<VertexBufferObject> vbo;

    explicit VertexArrayObject(unsigned int handle, const std::shared_ptr<VertexBufferObject>& vbo);

    auto deleteHandle() -> void;
};