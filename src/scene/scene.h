#pragma once

#include <array>
#include <memory>
#include <vector>

#include "../camera/camera.h"
#include "../shader/shader_program.h"
#include "../vertex_array_object/vertex_array_object.h"
#include "cube.h"
#include "light.h"

/**
 * A `Scene` represent the "world". Everything that currently lives into
 * the world such as objects, lights, behaviors and more.
 *
 * Members and methods are public because you are expected to edit them
 * directly.
 */
class Scene {
 public:
  auto draw_cubes(glm::mat4 projection) -> void;
  auto draw_lights(glm::mat4 projection) -> void;

  std::array<Light, 10> lights{};
  std::vector<Cube> cubes;

  std::shared_ptr<Camera> camera;
  std::shared_ptr<VertexArrayObject> cubeVao;
  std::shared_ptr<ShaderProgram> diffuseShader;
  std::shared_ptr<ShaderProgram> lightSourceShader;
};