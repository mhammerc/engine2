#include "scene.h"

#include <spdlog/fmt/fmt.h>

auto Scene::draw(glm::mat4 projection) -> void {
  camera->computeFront();

  lights.at(9) = Light{
	  .type = Light::Spot,
	  .position = camera->pos,
	  .direction = camera->front,
	  .innerCutOff = glm::cos(glm::radians(12.5F)),
	  .outerCutOff = glm::cos(glm::radians(17.5F)),
	  .linear = 0.09F,
	  .quadratic = 0.032F,
	  .ambient = glm::vec3(0.2F, 0.2F, 0.2F),
	  .diffuse = glm::vec3(0.5F, 0.5F, 0.5F),
	  .specular = glm::vec3(1.F, 1.F, 1.F)};

  draw_nodes(projection);
  draw_lights(projection);
}

auto Scene::draw_nodes(glm::mat4 projection) -> void {
  for (auto &node : nodes) {
	node.shader()->setUniform("view", camera->getMatrix());
	node.shader()->setUniform("projection", projection);
	node.shader()->setUniform("cameraPosition", camera->pos);
	node.shader()->setUniform("material.shininess", 32.0F);

	for (size_t i = 0; i < 10; ++i) {
	  Light const &light = lights.at(i);

	  node.shader()->setUniform(fmt::format("lights[{}].type", i), static_cast<int>(light.type));
	  node.shader()->setUniform(fmt::format("lights[{}].position", i), light.position);
	  node.shader()->setUniform(fmt::format("lights[{}].direction", i), glm::normalize(light.direction));
	  node.shader()->setUniform(fmt::format("lights[{}].innerCutOff", i), light.innerCutOff);
	  node.shader()->setUniform(fmt::format("lights[{}].outerCutOff", i), light.outerCutOff);
	  node.shader()->setUniform(fmt::format("lights[{}].constant", i), light.constant);
	  node.shader()->setUniform(fmt::format("lights[{}].linear", i), light.linear);
	  node.shader()->setUniform(fmt::format("lights[{}].quadratic", i), light.quadratic);
	  node.shader()->setUniform(fmt::format("lights[{}].ambient", i), light.ambient);
	  node.shader()->setUniform(fmt::format("lights[{}].diffuse", i), light.diffuse);
	  node.shader()->setUniform(fmt::format("lights[{}].specular", i), light.specular);
	}

	node.draw();
  }
}

auto Scene::draw_lights(glm::mat4 projection) -> void {
  return;

  //  for (const auto &light : lights) {
  //	if (light.type != Light::Type::Point) {
  //	  continue;
  //	}
  //
  //	glm::mat4 model = glm::mat4(1.0F);
  //	model = glm::translate(model, light.position);
  //	model = glm::scale(model, glm::vec3(0.25F));
  //
  //	light_source_shader->setUniform("model", model);
  //	light_source_shader->setUniform("view", camera->getMatrix());
  //	light_source_shader->setUniform("projection", projection);
  //
  //	light_source_shader->bind();
  //	//	cubeVao->draw();
  //	light_source_shader->unbind();
  //  }
}