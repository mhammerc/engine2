#include "mesh.h"

using namespace engine;

auto Mesh::upload_buffers() -> void {
    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);
    glGenBuffers(1, &_ebo);

    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);

    // upload vertices
    auto constexpr size_of_vertex = static_cast<GLsizeiptr>(sizeof(decltype(_vertices)::value_type));
    auto const number_of_vertices = static_cast<GLsizeiptr>(_vertices.size());
    glBufferData(GL_ARRAY_BUFFER, number_of_vertices * size_of_vertex, _vertices.data(), GL_STATIC_DRAW);

    // upload indices
    auto constexpr size_of_indice = static_cast<GLsizeiptr>(sizeof(decltype(_indices)::value_type));
    auto const number_of_indices = static_cast<GLsizeiptr>(_indices.size());
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, number_of_indices * size_of_indice, _indices.data(), GL_STATIC_DRAW);

    // NOLINTBEGIN(performance-no-int-to-ptr, cppcoreguidelines-pro-type-reinterpret-cast)

    // bind attribute 0 (position)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        size_of_vertex,
        reinterpret_cast<void*>(offsetof(Vertex, position))
    );

    // bind attribute 1 (normal)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, size_of_vertex, reinterpret_cast<void*>(offsetof(Vertex, normal)));

    // bind attribute 2 (tex_coords)
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(
        2,
        2,
        GL_FLOAT,
        GL_FALSE,
        size_of_vertex,
        reinterpret_cast<void*>(offsetof(Vertex, tex_coords))
    );

    // NOLINTEND(performance-no-int-to-ptr, cppcoreguidelines-pro-type-reinterpret-cast)

    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

auto Mesh::draw() const -> void {
    auto const number_of_indices = static_cast<GLsizei>(_indices.size());

    glBindVertexArray(_vao);
    glDrawElements(GL_TRIANGLES, number_of_indices, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

auto Mesh::vao() const -> u32 {
    return _vao;
}

Mesh::Mesh(std::vector<Vertex>&& vertices, std::vector<u32>&& indices) : _vertices(vertices), _indices(indices) {
    upload_buffers();
}

Mesh::~Mesh() noexcept {
    release();
}

Mesh::Mesh(Mesh&& from) noexcept :
    _vertices(std::move(from._vertices)),
    _indices(std::move(from._indices)),
    _vao(from._vao),
    _vbo(from._vbo),
    _ebo(from._ebo) {
    from._vao = 0;
    from._vbo = 0;
    from._ebo = 0;
}

auto Mesh::operator=(Mesh&& from) noexcept -> Mesh& {
    release();

    _vertices = std::move(from._vertices);
    _indices = std::move(from._indices);
    _vao = from._vao;
    _vbo = from._vbo;
    _ebo = from._ebo;

    from._vao = 0;
    from._vbo = 0;
    from._ebo = 0;

    return *this;
}

auto Mesh::release() -> void {
    if (_ebo != 0) {
        glDeleteBuffers(1, &_ebo);
        _ebo = 0;
    }

    if (_vbo != 0) {
        glDeleteBuffers(1, &_vbo);
        _vbo = 0;
    }

    if (_vao != 0) {
        glDeleteVertexArrays(1, &_vao);
        _vao = 0;
    }
}