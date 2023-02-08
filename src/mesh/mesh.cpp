#include "mesh.h"

Mesh::Mesh(
	std::vector<Vertex> &&vertices,
	std::vector<unsigned int> &&indices,
	std::vector<Texture> &&textures)
	: vertices(std::move(vertices)),
	  indices(std::move(indices)),
	  textures(std::move(textures)) {
  setupMesh();
}

void Mesh::setupMesh() {
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  using Vertex = decltype(vertices)::value_type;
  auto const sizeOfVertex = sizeof(Vertex);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER,
			   static_cast<GLsizeiptr>(vertices.size() * sizeOfVertex),
			   vertices.data(),
			   GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			   static_cast<GLsizeiptr>(vertices.size() * sizeOfVertex),
			   indices.data(),
			   GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0,       // location
						3,       // how many components
						GL_FLOAT,// what is a component
						GL_FALSE,
						sizeOfVertex,                                         // size of the struct which contain components
						reinterpret_cast<void *>(offsetof(Vertex, position)));// position of the components in the struct

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1,
						3,
						GL_FLOAT,
						GL_FALSE,
						sizeOfVertex,
						reinterpret_cast<void *>(offsetof(Vertex, normal)));

  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2,
						2,
						GL_FLOAT,
						GL_FALSE,
						sizeOfVertex,
						reinterpret_cast<void *>(offsetof(Vertex, texCoords)));

  glBindVertexArray(0);
}

void Mesh::draw(ShaderProgram &shader) {
  int diffuseIndex = 1;
  int specularIndex = 1;

  shader.bind();
  for (int i = 0; i < textures.size(); ++i) {
	auto &texture = textures[i];

	int currentIndex = 0;
	const char *type = "";

	if (texture.type == Texture::Diffuse) {
	  currentIndex = diffuseIndex++;
	  type = "diffuse";
	} else if (texture.type == Texture::Specular) {
	  currentIndex = specularIndex++;
	  type = "specular";
	}

	auto const uniformName = std::string("material.texture_")
								 .append(type)
								 .append(std::to_string(currentIndex));
	shader.setUniform(uniformName, i);

	texture.activate_as(i);
  }

  glActiveTexture(GL_TEXTURE0);

  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
  shader.unbind();
}

Mesh::Mesh(Mesh &&from) noexcept : vertices(std::move(from.vertices)),
								   indices(std::move(from.indices)),
								   textures(std::move(from.textures)),
								   VAO(from.VAO),
								   VBO(from.VBO),
								   EBO(from.EBO) {
  from.VAO = 0;
  from.VBO = 0;
  from.EBO = 0;
}

auto Mesh::operator=(Mesh &&from) noexcept -> Mesh & {
  vertices = std::move(from.vertices);
  indices = std::move(from.indices);
  textures = std::move(from.textures);

  VAO = from.VAO;
  VBO = from.VBO;
  EBO = from.EBO;

  from.VAO = 0;
  from.VBO = 0;
  from.EBO = 0;

  return *this;
}
