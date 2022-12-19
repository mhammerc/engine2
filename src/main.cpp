#include "opengl/opengl.h"
#include <GLFW/glfw3.h>
#include "shader/shader_program.h"
#include "spdlog/spdlog.h"
#include "glfw/glfw.h"
#include "stb_image/stb_image.h"
#include "texture/texture.h"
#include "camera/camera.h"
#include "gui/gui.h"
#include "vertex_buffer_object/vertex_buffer_object.h"
#include "engine.h"

int main() {
  auto window = init_glfw_and_opengl();
  if (!window) {
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

  auto program = ShaderProgram::from_vertex_and_fragment("../shaders/vertex.vert", "../shaders/frag.frag");
  if (!program.has_value()) {
	spdlog::critical("could not create shader program.");
	return 1;
  }

  Camera camera(window);

  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  auto VBO = VertexBufferObject::from_cube();

  VBO.bind();
  glBindVertexArray(0);
  VBO.unbind();

  game_loop(window, [&](float delta_time, bool &should_quit) {
	gui_prepare_frame();

	processInputs(delta_time, window, camera);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

	int width = 0;
	int height = 0;
	glfwGetWindowSize(window, &width, &height);
	glm::mat4 projection;
	projection =
		glm::perspective(glm::radians(45.0f), static_cast<float>(width) / static_cast<float>(height), 0.1f, 100.0f);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	texture1->activate_as(0);
	texture2->activate_as(1);
	program->setUniform("texture1", 0);
	program->setUniform("texture2", 1);
	program->setUniform("model", model);
	program->setUniform("view", camera.getMatrix());
	program->setUniform("projection", projection);
	glUseProgram(program->handle);
	glBindVertexArray(VAO);

	VBO.draw();

	glUseProgram(0);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	gui_end_frame();

	glfwPollEvents();
	glfwSwapBuffers(window);
  });

  terminate_glfw(window);

  return 0;
}