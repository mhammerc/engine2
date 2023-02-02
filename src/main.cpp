#include "camera/camera.h"
#include "engine.h"
#include "glfw/glfw.h"
#include "gui/gui.h"
#include "opengl/opengl.h"
#include "shader/shader_program.h"
#include "spdlog/spdlog.h"
#include "stb_image/stb_image.h"
#include "texture/texture.h"
#include "vertex_buffer_object/vertex_buffer_object.h"
#include "vertex_array_object/vertex_array_object.h"
#include "node/node.h"

auto main() -> int {
  auto *window = init_glfw_and_opengl();
  if (window == nullptr) {
	spdlog::critical("Could not initialize GLFW3 or OpenGL.");
	return 1;
  }

  auto texture1 = Texture::from_file("../assets/container.jpg");
  if (!texture1.has_value()) {
	spdlog::critical("Could not create texture1.");
	return 1;
  }

  auto texture2 = Texture::from_file("../assets/awesomeface.png");
  if (!texture2.has_value()) {
	spdlog::critical("Could not create texture2.");
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

  auto lightShader = ShaderProgram::from_vertex_and_fragment("../shaders/light.vert", "../shaders/light.frag");
  if (lightShader == nullptr) {
	spdlog::critical("could not create shader program.");
	return 1;
  }

  Camera camera(window);

  auto vao = VertexArrayObject::from_cube();

  // V-Sync
  // 0 = unlimited FPS on my Mac
  glfwSwapInterval(1);

  auto node = Node(vao, diffuse_shader);
  node.position() = glm::vec3(0.F, 0.F, 0.F);

  auto light = Node(vao, lightShader);
  light.scale() = glm::vec3(0.25F);

  game_loop(window, [&](float delta_time, bool & /*should_quit*/) {
	gui_prepare_frame();
	gui_show_system_window(delta_time, window);

	processInputs(delta_time, window, camera);

	int width = 0;
	int height = 0;
	glfwGetWindowSize(window, &width, &height);
	const glm::mat4 projection =
		glm::perspective(glm::radians(45.0F), static_cast<float>(width) / static_cast<float>(height), 0.1F, 100.0F);

	glClearColor(0.2F, 0.3F, 0.3F, 1.0F);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	auto view = camera.getMatrix();

	texture1->activate_as(0);
	texture2->activate_as(1);

	static float elapsed_time = 0;
	elapsed_time += delta_time;
	light.position() = glm::vec3(sinf(elapsed_time) * 3.F, 0.F, cosf(elapsed_time) * 3.F);

	node.draw(view, projection, light.position(), camera.pos);
	light.draw(view, projection, light.position(), camera.pos);

	gui_end_frame();

	glfwPollEvents();
	glfwSwapBuffers(window);
  });

  terminate_glfw(window);

  return 0;
}