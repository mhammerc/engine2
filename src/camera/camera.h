#pragma once
#include "../opengl/opengl.h"
#include <GLFW/glfw3.h>

struct Camera {
  [[nodiscard]] glm::mat4 getMatrix() const;

  glm::vec3 pos = glm::vec3(0.0f, 0.0f, 3.0f);
  glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
  glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

  float yaw = -90.f;
  float pitch = 0.f;
  glm::vec2 previous_cursor_position;

  void computeFront();

  explicit Camera(GLFWwindow *window);
};