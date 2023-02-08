#include "camera/camera.h"
#include "engine.h"
#include "glfw/glfw.h"
#include "gui/gui.h"
#include "model/model.h"
#include "opengl/opengl.h"
#include "scene/scene.h"
#include "shader/shader_program.h"
#include "spdlog/spdlog.h"
#include "stb_image/stb_image.h"

auto main() -> int {
  auto *window = init_glfw_and_opengl();
  if (window == nullptr) {
	spdlog::critical("Could not initialize GLFW3 or OpenGL.");
	return 1;
  }

  if (!gui_init(window)) {
	spdlog::critical("Could not initialize GUI.");
	return 1;
  }

  auto diffuse_shader = ShaderProgram::from_vertex_and_fragment("../shaders/diffuse.vert", "../shaders/diffuse.frag");
  if (!diffuse_shader.has_value()) {
	spdlog::critical("could not create shader program.");
	return 1;
  }

  auto lightShader = ShaderProgram::from_vertex_and_fragment("../shaders/light.vert", "../shaders/light.frag");
  if (!lightShader.has_value()) {
	spdlog::critical("could not create shader program.");
	return 1;
  }

  Camera camera(window);

  std::array<Light, 10> lights{};
  auto const pointLight = Light{
	  .type = Light::Point,
	  .position = glm::vec3(1.F, 0.F, 2.F),
	  .linear = 0.09F,
	  .quadratic = 0.032F,
	  .ambient = glm::vec3(0.35F, 0.35F, 0.35F),
	  .diffuse = glm::vec3(0.8F, 0.8F, 0.8F),
	  .specular = glm::vec3(1.F, 1.F, 1.F),
  };
  lights.at(0) = pointLight;

  Model model("../assets/backpack/backpack.obj");

  game_loop(window, [&](float delta_time, bool & /*should_quit*/) {
	gui_prepare_frame();
	gui_show_system_window(delta_time, window);

	processInputs(delta_time, window, camera);

	glClearColor(0.2F, 0.3F, 0.3F, 1.0F);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	camera.computeFront();
	lights.at(9) = Light{
		.type = Light::Spot,
		.position = camera.pos,
		.direction = camera.front,
		.innerCutOff = glm::cos(glm::radians(20.F)),
		.outerCutOff = glm::cos(glm::radians(25.F)),
		.linear = 0.09F,
		.quadratic = 0.032F,
		.ambient = glm::vec3(0.2F, 0.2F, 0.2F),
		.diffuse = glm::vec3(0.5F, 0.5F, 0.5F),
		.specular = glm::vec3(1.F, 1.F, 1.F)};

	int width = 0;
	int height = 0;
	glfwGetWindowSize(window, &width, &height);
	const glm::mat4 projection =
		glm::perspective(glm::radians(45.0F), static_cast<float>(width) / static_cast<float>(height), 0.1F, 100.0F);

	glm::mat4 modelmat = glm::mat4(1.0F);
	modelmat = glm::translate(modelmat, glm::vec3(5.F, 0.F, 0.F));
	modelmat = glm::rotate(modelmat, glm::radians(0.F), glm::vec3(1.F, 0.F, 0.F));
	modelmat = glm::rotate(modelmat, glm::radians(0.F), glm::vec3(0.F, 1.F, 0.F));
	modelmat = glm::rotate(modelmat, glm::radians(0.F), glm::vec3(0.F, 0.F, 1.F));
	modelmat = glm::scale(modelmat, glm::vec3(1.F, 1.F, 1.F));
	diffuse_shader->setUniform("model", modelmat);
	diffuse_shader->setUniform("view", camera.getMatrix());
	diffuse_shader->setUniform("projection", projection);
	diffuse_shader->setUniform("modelNormal", glm::mat3(glm::transpose(glm::inverse(modelmat))));
	diffuse_shader->setUniform("cameraPosition", camera.pos);
	diffuse_shader->setUniform("material.shininess", 32.0F);

	for (size_t i = 0; i < 10; ++i) {
	  Light const &light = lights.at(i);

	  diffuse_shader->setUniform(fmt::format("lights[{}].type", i), static_cast<int>(light.type));
	  diffuse_shader->setUniform(fmt::format("lights[{}].position", i), light.position);
	  diffuse_shader->setUniform(fmt::format("lights[{}].direction", i), glm::normalize(light.direction));
	  diffuse_shader->setUniform(fmt::format("lights[{}].innerCutOff", i), light.innerCutOff);
	  diffuse_shader->setUniform(fmt::format("lights[{}].outerCutOff", i), light.outerCutOff);
	  diffuse_shader->setUniform(fmt::format("lights[{}].constant", i), light.constant);
	  diffuse_shader->setUniform(fmt::format("lights[{}].linear", i), light.linear);
	  diffuse_shader->setUniform(fmt::format("lights[{}].quadratic", i), light.quadratic);
	  diffuse_shader->setUniform(fmt::format("lights[{}].ambient", i), light.ambient);
	  diffuse_shader->setUniform(fmt::format("lights[{}].diffuse", i), light.diffuse);
	  diffuse_shader->setUniform(fmt::format("lights[{}].specular", i), light.specular);
	}

	model.draw(*diffuse_shader);

	static float elapsed_time = 0;
	elapsed_time += delta_time;

	gui_end_frame();
	glfwPollEvents();
	glfwSwapBuffers(window);
  });

  terminate_glfw(window);

  return 0;
}
