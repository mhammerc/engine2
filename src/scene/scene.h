#pragma once

#include "../camera/camera.h"
#include "../node/node.h"
#include "../shader/shader_program.h"
#include "../vertex_array_object/vertex_array_object.h"
#include "light.h"

#include <array>
#include <memory>
#include <vector>

/**
 * A `Scene` represent the "world". Everything that currently lives into
 * the world such as objects, lights, behaviors and more.
 */
class Scene {
 public:
  auto draw(GLFWwindow *window, float delta_time, glm::mat4 projection) -> void;
  auto draw_nodes(glm::mat4 projection) -> void;
  auto draw_lights(glm::mat4 projection) -> void;

  std::array<Light, 10> lights{};
  std::vector<Node> nodes;

  std::shared_ptr<Camera> camera;

  bool flashlight = true;

  //  std::shared_ptr<ShaderProgram> light_source_shader;
};