#include "scene.h"

#include <spdlog/fmt/fmt.h>

auto Scene::draw_cubes(glm::mat4 projection) -> void {
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

  for (const auto &cube : cubes) {
	glm::mat4 model = glm::mat4(1.0F);
	model = glm::translate(model, cube.position);
	model = glm::rotate(model, glm::radians(cube.rotation.x), glm::vec3(1.F, 0.F, 0.F));
	model = glm::rotate(model, glm::radians(cube.rotation.y), glm::vec3(0.F, 1.F, 0.F));
	model = glm::rotate(model, glm::radians(cube.rotation.z), glm::vec3(0.F, 0.F, 1.F));
	model = glm::scale(model, cube.scale);

	diffuseShader->setUniform("model", model);
	diffuseShader->setUniform("view", camera->getMatrix());
	diffuseShader->setUniform("projection", projection);
	diffuseShader->setUniform("viewPos", camera->pos);
	diffuseShader->setUniform("material.diffuse", 0);
	diffuseShader->setUniform("material.specular", 1);
	diffuseShader->setUniform("material.shininess", 32.0F);

	for (size_t i = 0; i < 10; ++i) {
	  Light const &light = lights.at(i);

	  diffuseShader->setUniform(fmt::format("lights[{}].type", i), static_cast<int>(light.type));
	  diffuseShader->setUniform(fmt::format("lights[{}].position", i), light.position);
	  diffuseShader->setUniform(fmt::format("lights[{}].direction", i), light.direction);
	  diffuseShader->setUniform(fmt::format("lights[{}].innerCutOff", i), light.innerCutOff);
	  diffuseShader->setUniform(fmt::format("lights[{}].outerCutOff", i), light.outerCutOff);
	  diffuseShader->setUniform(fmt::format("lights[{}].constant", i), light.constant);
	  diffuseShader->setUniform(fmt::format("lights[{}].linear", i), light.linear);
	  diffuseShader->setUniform(fmt::format("lights[{}].quadratic", i), light.quadratic);
	  diffuseShader->setUniform(fmt::format("lights[{}].ambient", i), light.ambient);
	  diffuseShader->setUniform(fmt::format("lights[{}].diffuse", i), light.diffuse);
	  diffuseShader->setUniform(fmt::format("lights[{}].specular", i), light.specular);
	}

	diffuseShader->bind();
	cubeVao->draw();
	diffuseShader->unbind();
  }
}

auto Scene::draw_lights(glm::mat4 projection) -> void {
  for (const auto &light : lights) {
	if (light.type != Light::Type::Point) {
	  continue;
	}

	glm::mat4 model = glm::mat4(1.0F);
	model = glm::translate(model, light.position);
	model = glm::scale(model, glm::vec3(0.25F));

	lightSourceShader->setUniform("model", model);
	lightSourceShader->setUniform("view", camera->getMatrix());
	lightSourceShader->setUniform("projection", projection);

	lightSourceShader->bind();
	cubeVao->draw();
	lightSourceShader->unbind();
  }
}