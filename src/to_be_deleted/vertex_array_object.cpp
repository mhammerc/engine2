#include "vertex_array_object.h"

#include <utility>

#include "../platform/opengl.h"

auto VertexArrayObject::from_cube() -> std::shared_ptr<VertexArrayObject> {
    unsigned int handle = 0;
    glGenVertexArrays(1, &handle);
    glBindVertexArray(handle);

    auto vbo = std::make_shared<VertexBufferObject>(VertexBufferObject::from_cube());

    glBindVertexArray(0);

    auto vao = VertexArrayObject(handle, vbo);

    return std::make_shared<VertexArrayObject>(std::move(vao));
}

auto VertexArrayObject::from_quad() -> std::unique_ptr<VertexArrayObject> {
    GLuint handle = 0;
    glGenVertexArrays(1, &handle);
    glBindVertexArray(handle);

    auto vbo = std::make_shared<VertexBufferObject>(VertexBufferObject::from_quad());

    glBindVertexArray(0);

    auto vao = VertexArrayObject(handle, vbo);

    return std::make_unique<VertexArrayObject>(std::move(vao));
}

auto VertexArrayObject::bind() -> void {
    glBindVertexArray(handle);
}

auto VertexArrayObject::unbind() -> void {
    glBindVertexArray(0);
}

auto VertexArrayObject::deleteHandle() -> void {
    if (handle != 0) {
        glDeleteVertexArrays(1, &handle);
        handle = 0;
    }
}

auto VertexArrayObject::draw() -> void {
    bind();
    vbo->draw();
    unbind();
}

VertexArrayObject::VertexArrayObject(unsigned int handle, const std::shared_ptr<VertexBufferObject>& vbo) :
    handle(handle),
    vbo(vbo) {}

VertexArrayObject::~VertexArrayObject() noexcept {
    deleteHandle();
}

VertexArrayObject::VertexArrayObject(VertexArrayObject&& from) noexcept :
    handle(from.handle),
    vbo(std::move(from.vbo)) {
    from.handle = 0;
}

auto VertexArrayObject::operator=(VertexArrayObject&& from) noexcept -> VertexArrayObject& {
    deleteHandle();

    handle = from.handle;
    vbo = std::move(from.vbo);

    from.handle = 0;

    return *this;
}