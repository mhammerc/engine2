#include "camera/camera.h"
#include "engine.h"
#include "framebuffer/framebuffer.h"
#include "glfw/glfw.h"
#include "gui/gui.h"
#include "model/model.h"
#include "opengl/opengl.h"
#include "scene/scene.h"
#include "shader/shader_program.h"
#include "skybox/skybox.h"
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

  //  std::shared_ptr<ShaderProgram> diffuse_shader = std::move(ShaderProgram::from_vertex_and_fragment("../shaders/diffuse.vert", "../shaders/diffuse.frag"));
  std::shared_ptr<ShaderProgram> diffuse_shader = std::move(ShaderProgram::from_vertex_and_fragment_and_geometry("../shaders/diffuse.vert", "../shaders/diffuse.frag", "../shaders/diffuse.glsl"));
  if (diffuse_shader == nullptr) {
	spdlog::critical("could not create shader program.");
	return 1;
  }

  auto light_source_shader = ShaderProgram::from_vertex_and_fragment("../shaders/light.vert", "../shaders/light.frag");
  if (light_source_shader == nullptr) {
	spdlog::critical("could not create shader program.");
	return 1;
  }

  auto outline_shader = ShaderProgram::from_vertex_and_fragment("../shaders/outline.vert", "../shaders/outline.frag");
  if (outline_shader == nullptr) {
	spdlog::critical("could not create shader program.");
	return 1;
  }

  auto postprocess_shader = ShaderProgram::from_vertex_and_fragment("../shaders/postprocess.vert", "../shaders/postprocess.frag");
  if (postprocess_shader == nullptr) {
	spdlog::critical("could not create shader program.");
	return 1;
  }

  auto normal_shader = ShaderProgram::from_vertex_and_fragment_and_geometry("../shaders/normal.vert", "../shaders/normal.frag", "../shaders/normal.geo");
  if (normal_shader == nullptr) {
	spdlog::critical("could not create shader program.");
	return 1;
  }

  Scene scene;
  scene.camera = std::make_shared<Camera>(window);
  scene.outline_shader = std::move(outline_shader);
  scene.normal_shader = std::move(normal_shader);

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
	//	auto model = std::make_shared<Model>("../assets/sponza2/sponza.obj");

	node.shader() = diffuse_shader;
	node.model() = model;
	node.position() = glm::vec3(0.F, 0.F, -5.F);

	scene.nodes.push_back(node);
  }

  Node node2;
  {
	auto model = std::make_shared<Model>("../assets/backpack/backpack.obj");
	//	auto model = std::make_shared<Model>("../assets/sponza2/sponza.obj");

	node2.shader() = diffuse_shader;
	node2.model() = model;
	node2.position() = glm::vec3(-4.F, 0.F, -5.F);

	scene.nodes.push_back(node2);
  }

  auto skybox = Skybox::from_files(
	  {
		  "../assets/skybox/right.jpg",
		  "../assets/skybox/left.jpg",
		  "../assets/skybox/top.jpg",
		  "../assets/skybox/bottom.jpg",
		  "../assets/skybox/front.jpg",
		  "../assets/skybox/back.jpg",
	  });

  int width = 0;
  int height = 0;
  glfwGetFramebufferSize(window, &width, &height);
  auto fb = FrameBuffer::create(width, height);

  auto quad_vao = VertexArrayObject::from_quad();

  game_loop(window, [&](float delta_time, bool & /*should_quit*/) {
	gui_prepare_frame();

	processInputs(delta_time, window, *scene.camera);

	glClearColor(0.2F, 0.3F, 0.3F, 1.0F);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glfwGetFramebufferSize(window, &width, &height);
	const glm::mat4 projection =
		glm::perspective(glm::radians(45.0F), static_cast<float>(width) / static_cast<float>(height), 0.1F, 100.0F);

	// Resize the framebuffer as needed
	fb->resize(width, height);

	// Render scene
	{
	  fb->bind();

	  glClearColor(0.1F, 0.1F, 0.1F, 1.0F);
	  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	  skybox->activate_cubemap_as(10);
	  scene.draw(window, delta_time, projection, skybox.get());

	  fb->unbind();
	}

	// Render postprocess quad
	{
	  glDisable(GL_DEPTH_TEST);

	  fb->color_texture()->activate_as(0);
	  postprocess_shader->setUniform("screenTexture", 0);

	  int post_process = 0;
	  if (scene.inverse) {
		post_process |= POST_PROCESS_INVERSE;
	  }
	  if (scene.black_and_white) {
		post_process |= POST_PROCESS_GRAYSCALE;
	  }
	  if (scene.sepia) {
		post_process |= POST_PROCESS_SEPIA;
	  }
	  if (scene.blur) {
		post_process |= POST_PROCESS_BLUR;
	  }
	  if (scene.sharpen) {
		post_process |= POST_PROCESS_SHARPEN;
	  }
	  if (scene.edge_dectection) {
		post_process |= POST_PROCESS_EDGE_DETECTION;
	  }
	  postprocess_shader->setUniform("post_process", post_process);

	  postprocess_shader->bind();
	  quad_vao->draw();
	  postprocess_shader->unbind();

	  glEnable(GL_DEPTH_TEST);
	}

	gui_end_frame();
	glfwPollEvents();
	glfwSwapBuffers(window);
  });

  terminate_glfw(window);

  return 0;
}
