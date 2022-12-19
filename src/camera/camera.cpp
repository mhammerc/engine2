#include "camera.h"
#include "spdlog/spdlog.h"

glm::mat4 Camera::getMatrix() const {
//  How to manually compute lookAt:
//
//  glm::vec3 cameraRight = glm::normalize(glm::cross(up, front));
//  glm::vec3 cameraUp = glm::cross(front, cameraRight);
//
//  glm::mat4x4 axis_matrix(1.f);
//  axis_matrix = glm::row(axis_matrix, 0, glm::vec4(-cameraRight, 0.f));
//  axis_matrix = glm::row(axis_matrix, 1, glm::vec4(cameraUp, 0.f));
//  axis_matrix = glm::row(axis_matrix, 2, glm::vec4(-front, 0.f));

//  glm::mat4x4 position_matrix(1.f);
//  position_matrix = glm::column(position_matrix, 3, glm::vec4(-pos, 1.0f));
//
//  return axis_matrix * position_matrix;

  return glm::lookAt(pos, pos + front, up);
}

void Camera::computeFront() {
  glm::vec3 direction;
  direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  direction.y = sin(glm::radians(pitch));
  direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

  front = glm::normalize(direction);
}

Camera::Camera(GLFWwindow *window) {
  int width = 0;
  int height = 0;

  glfwGetWindowSize(window, &width, &height);

  previous_cursor_position = glm::vec2(width / 2, height / 2);
}