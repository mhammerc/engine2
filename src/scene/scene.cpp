#include "scene.h"

#include "../gui/gui.h"

#include <spdlog/fmt/fmt.h>

auto Scene::draw(GLFWwindow *window, float delta_time, glm::mat4 projection) -> void {
  gui_show_system_window(this, delta_time, window);

  camera->computeFront();

  lights.at(9) = Light{
	  .type = flashlight ? Light::Spot : Light::Unset,
	  .position = camera->pos,
	  .direction = camera->front,
	  .innerCutOff = glm::cos(glm::radians(12.5F)),
	  .outerCutOff = glm::cos(glm::radians(17.5F)),
	  .linear = 0.09F,
	  .quadratic = 0.032F,
	  .ambient = glm::vec3(0.2F, 0.2F, 0.2F),
	  .diffuse = glm::vec3(0.5F, 0.5F, 0.5F),
	  .specular = glm::vec3(1.F, 1.F, 1.F)};

  if (outline) {
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilMask(0xFF);
  }

  draw_nodes(projection);

  if (outline) {
	glStencilMask(0x00);
	glDisable(GL_DEPTH_TEST);
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);

	draw_nodes_outline(projection);

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_STENCIL_TEST);
	glStencilMask(0xFF);
  }
}

auto Scene::draw_nodes(glm::mat4 projection) -> void {
  for (auto &node : nodes) {
	node.shader()->setUniform("view", camera->getMatrix());
	node.shader()->setUniform("projection", projection);
	node.shader()->setUniform("cameraPosition", camera->pos);
	node.shader()->setUniform("material.shininess", 32.0F);
	node.shader()->setUniform("material.texture_environment", 10);
	node.shader()->setUniform("material.reflect", mirror ? 1.F : 0.F);
	node.shader()->setUniform("material.refract", glass ? 1.F : 0.F);

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

auto Scene::draw_nodes_outline(glm::mat4 projection) -> void {
  for (auto &node : nodes) {
	auto previousShader = node.shader();
	node.shader() = outline_shader;

	node.shader()->setUniform("view", camera->getMatrix());
	node.shader()->setUniform("projection", projection);

	node.draw();

	node.shader() = previousShader;
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