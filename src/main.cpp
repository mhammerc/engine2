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
  if (diffuse_shader == nullptr) {
	spdlog::critical("could not create shader program.");
	return 1;
  }

  auto light_source_shader = ShaderProgram::from_vertex_and_fragment("../shaders/light.vert", "../shaders/light.frag");
  if (light_source_shader == nullptr) {
	spdlog::critical("could not create shader program.");
	return 1;
  }

  Scene scene;
  scene.camera = std::make_shared<Camera>(window);

  {
	auto const pointLight = Light{
		.type = Light::Point,
		.position = glm::vec3(0.F, 0.F, -2.F),
		.linear = 0.09F,
		.quadratic = 0.032F,
		.ambient = glm::vec3(0.35F, 0.35F, 0.35F),
		.diffuse = glm::vec3(0.8F, 0.8F, 0.8F),
		.specular = glm::vec3(1.F, 1.F, 1.F),
	};
	scene.lights.at(0) = pointLight;
  }

  Node node;
  {
	auto model = std::make_shared<Model>("../assets/backpack/backpack.obj");

	node.shader() = diffuse_shader;
	node.model() = model;
	node.position() = glm::vec3(0.F, 0.F, -5.F);

	scene.nodes.push_back(node);
  }

  game_loop(window, [&](float delta_time, bool & /*should_quit*/) {
	gui_prepare_frame();

	processInputs(delta_time, window, *scene.camera);

	glClearColor(0.2F, 0.3F, 0.3F, 1.0F);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	int width = 0;
	int height = 0;
	glfwGetWindowSize(window, &width, &height);
	const glm::mat4 projection =
		glm::perspective(glm::radians(45.0F), static_cast<float>(width) / static_cast<float>(height), 0.1F, 100.0F);

	scene.draw(window, delta_time, projection);

	gui_end_frame();
	glfwPollEvents();
	glfwSwapBuffers(window);
  });

  terminate_glfw(window);

  return 0;
}
