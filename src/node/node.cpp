#include "node.h"

#include "../opengl/opengl.h"

auto Node::getModel() -> glm::mat4 {
  glm::mat4 model = glm::mat4(1.0F);
  model = glm::translate(model, _position);

  model = glm::rotate(model, glm::radians(_rotation), glm::vec3(1.0F, 0.3F, 0.5F));

  model = glm::scale(model, _scale);

//                             Angle of rotation    axis of rotation
//  model = glm::rotate(model, glm::radians(50.0F), glm::vec3(0.5F, 1.0F, 0.0F));

  return model;
}

Node::Node(std::shared_ptr<VertexArrayObject> vao, std::shared_ptr<ShaderProgram> shader)
	: vao(std::move(vao)), shader(std::move(shader)) {}

auto Node::draw(const glm::mat4 &view,
				const glm::mat4 &projection,
				const glm::vec3 &light,
				const glm::vec3 &camera_position) -> void {
  auto model = getModel();

  shader->setUniform("model", model);
  shader->setUniform("view", view);
  shader->setUniform("projection", projection);
  shader->setUniform("viewPos", camera_position);
  shader->setUniform("material.diffuse", 0);
  shader->setUniform("material.specular", 1);
  shader->setUniform("material.emission", 2);
  shader->setUniform("material.shininess", 32.0F);
  shader->setUniform("light.position", light);
  shader->setUniform("light.ambient",  glm::vec3(0.2F, 0.2F, 0.2F));
  shader->setUniform("light.diffuse",  glm::vec3(0.5F, 0.5F, 0.5F)); // darken diffuse light a bit
  shader->setUniform("light.specular", glm::vec3(1.0F, 1.0F, 1.0F));

  shader->bind();
  vao->draw();
  shader->unbind();
}

auto Node::position() -> glm::vec3 & {
  return _position;
}

auto Node::rotation() -> float & {
  return _rotation;
}

auto Node::scale() -> glm::vec3 & {
  return _scale;
}