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
#include "texture/texture.h"
#include "vertex_array_object/vertex_array_object.h"

auto emplace_cubes(Scene &scene) -> void {
  const std::array<glm::vec3, 10> cubePositions = {
	  glm::vec3(0.0F, 0.0F, 0.0F),
	  glm::vec3(2.0F, 5.0F, -15.0F),
	  glm::vec3(-1.5F, -2.2F, -2.5F),
	  glm::vec3(-3.8F, -2.0F, -12.3F),
	  glm::vec3(2.4F, -0.4F, -3.5F),
	  glm::vec3(-1.7F, 3.0F, -7.5F),
	  glm::vec3(1.3F, -2.0F, -2.5F),
	  glm::vec3(1.5F, 2.0F, -2.5F),
	  glm::vec3(1.5F, 0.2F, -1.5F),
	  glm::vec3(-1.3F, 1.0F, -1.5F)};

  for (size_t i = 0; i < cubePositions.size(); ++i) {
	auto const cube = Cube{
		.position = cubePositions.at(i),
		.rotation = glm::vec3(20.F * i, 10.F * i, 5.F * i)};
	scene.cubes.emplace_back(cube);
  }
}

auto main() -> int {
  auto *window = init_glfw_and_opengl();
  if (window == nullptr) {
	spdlog::critical("Could not initialize GLFW3 or OpenGL.");
	return 1;
  }

  //  auto texture1 = Texture::from_file("../assets/container2.png");
  //  if (!texture1.has_value()) {
  //	spdlog::critical("Could not create texture1.");
  //	return 1;
  //  }
  //
  //  auto texture1_specular = Texture::from_file("../assets/container2_specular.png");
  //  if (!texture1_specular.has_value()) {
  //	spdlog::critical("Could not create texture1_specular.");
  //	return 1;
  //  }

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

  std::shared_ptr<Camera> camera = std::make_shared<Camera>(window);

  //  auto cubeVao = VertexArrayObject::from_cube();
  //
  //  Scene scene;
  //  scene.camera = camera;
  //  scene.cubeVao = cubeVao;
  //  scene.diffuseShader = diffuse_shader;
  //  scene.lightSourceShader = lightShader;
  //  emplace_cubes(scene);

  auto const pointLight = Light{
	  .type = Light::Point,
	  .position = glm::vec3(1.F, 0.F, 2.F),
	  .linear = 0.09F,
	  .quadratic = 0.032F,
	  .ambient = glm::vec3(0.35F, 0.35F, 0.35F),
	  .diffuse = glm::vec3(0.8F, 0.8F, 0.8F),
	  .specular = glm::vec3(1.F, 1.F, 1.F),
  };
  //  scene.lights.at(0) = pointLight;

  //  auto const directionalLight = Light{
  //	  .type = Light::Directional,
  //	  .direction = glm::vec3(1.F, 0.F, 0.F),
  //	  .ambient = glm::vec3(0.2F, 0.2F, 0.2F),
  //	  .diffuse = glm::vec3(0.5F, 0.5F, 0.5F),
  //	  .specular = glm::vec3(1.F, 1.F, 1.F),
  //  };
  //  scene.lights.at(1) = directionalLight;

  Model model("../assets/backpack/backpack.obj");

  std::array<Light, 10> lights{};
  lights.at(0) = pointLight;

  game_loop(window, [&](float delta_time, bool & /*should_quit*/) {
	gui_prepare_frame();
	gui_show_system_window(delta_time, window);

	processInputs(delta_time, window, *camera);

	glClearColor(0.2F, 0.3F, 0.3F, 1.0F);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	camera->computeFront();
	lights.at(9) = Light{
		.type = Light::Spot,
		.position = camera->pos,
		.direction = camera->front,
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
	diffuse_shader->setUniform("view", camera->getMatrix());
	diffuse_shader->setUniform("projection", projection);
	diffuse_shader->setUniform("modelNormal", glm::mat3(glm::transpose(glm::inverse(modelmat))));
	diffuse_shader->setUniform("cameraPosition", camera->pos);
	//	diffuse_shader->setUniform("material.diffuse", 0);
	//	diffuse_shader->setUniform("material.specular", 1);
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