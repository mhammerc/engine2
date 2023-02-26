#include "vertex_buffer_object.h"

#include <utility>

#include "../platform/opengl.h"

auto VertexBufferObject::define_attribute(size_t index, size_t size, size_t offset) -> void {
    bind();

    glVertexAttribPointer(
        index,
        static_cast<GLint>(size),
        GL_FLOAT,
        GL_FALSE,
        static_cast<GLsizei>(stride * sizeof(float)),
        reinterpret_cast<void*>(offset * sizeof(float))
    );

    glEnableVertexAttribArray(index);

    unbind();
}

auto VertexBufferObject::draw() -> void {
    size_t const vertex_size_in_bytes = sizeof(decltype(vertices)::value_type);

    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertices.size() * vertex_size_in_bytes / stride));
}

auto VertexBufferObject::bind() -> void {
    glBindBuffer(GL_ARRAY_BUFFER, handle);
}

auto VertexBufferObject::unbind() -> void {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

VertexBufferObject::VertexBufferObject(std::vector<float>&& _vertices, int stride) :
    vertices(_vertices),
    stride(stride),
    handle(0) {
    glGenBuffers(1, &handle);

    bind();

    size_t const vertex_size_in_bytes = sizeof(decltype(vertices)::value_type);

    glBufferData(
        GL_ARRAY_BUFFER,
        static_cast<GLsizeiptr>(vertices.size() * vertex_size_in_bytes),
        vertices.data(),
        GL_STATIC_DRAW
    );

    unbind();
}

auto VertexBufferObject::deleteHandle() -> void {
    if (handle != 0) {
        glDeleteBuffers(1, &handle);
        handle = 0;
    }
}

VertexBufferObject::~VertexBufferObject() noexcept {
    deleteHandle();
}

VertexBufferObject::VertexBufferObject(VertexBufferObject&& from) noexcept :
    vertices(std::move(from.vertices)),
    stride(from.stride),
    handle(from.handle) {
    from.handle = 0;
}

auto VertexBufferObject::operator=(VertexBufferObject&& from) noexcept -> VertexBufferObject& {
    deleteHandle();

    handle = from.handle;
    vertices = std::move(from.vertices);
    stride = from.stride;

    from.handle = 0;

    return *this;
}