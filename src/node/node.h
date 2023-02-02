#pragma once

#include <memory>
#include <glm/glm.hpp>

#include "../vertex_array_object/vertex_array_object.h"
#include "../shader/shader_program.h"

class Node {
 public:
  explicit Node(std::shared_ptr<VertexArrayObject> vao, std::shared_ptr<ShaderProgram> shader);

  auto draw(const glm::mat4 &view,
			const glm::mat4 &projection,
			const glm::vec3 &light,
			const glm::vec3 &camera_position) -> void;

  auto position() -> glm::vec3 &;
  auto rotation() -> glm::vec3 &;
  auto scale() -> glm::vec3 &;

 private:
  glm::vec3 _position{};
  glm::vec3 _rotation{};
  glm::vec3 _scale{1.F};

  auto getModel() -> glm::mat4;

  std::shared_ptr<VertexArrayObject> vao;
  std::shared_ptr<ShaderProgram> shader;
};