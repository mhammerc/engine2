#pragma once

#include <string>
#include <vector>

#include "../../platform/opengl.h"
#include "shader_program.h"
#include "texture.h"

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

class Mesh {
  public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<std::unique_ptr<Texture>> textures;

    Mesh(
        std::vector<Vertex>&& vertices,
        std::vector<unsigned int>&& indices,
        std::vector<std::unique_ptr<Texture>>&& textures
    );

    void draw(ShaderProgram& shader);

    ~Mesh();

    Mesh(const Mesh&) = delete;
    auto operator=(const Mesh&) -> Mesh& = delete;

    Mesh(Mesh&&) noexcept;
    auto operator=(Mesh&&) noexcept -> Mesh&;

  private:
    unsigned int VAO = 0;
    unsigned int VBO = 0;
    unsigned int EBO = 0;

    void setupMesh();
};