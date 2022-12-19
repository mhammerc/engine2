#include "glfw.h"
#include "spdlog/spdlog.h"
#include "../utils/platform_info.h"

static void error_callback(int error, const char *description) {
  spdlog::error("GLFW3 Error ({}): {}", error, description);
}

static void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void processInputs(float delta_time, GLFWwindow *window, Camera &camera) {
  // KEYBOARD
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	glfwSetWindowShouldClose(window, true);

  const float cameraSpeed = 3.f * delta_time;
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	camera.pos += cameraSpeed * camera.front;

  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	camera.pos -= cameraSpeed * camera.front;

  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	camera.pos -= glm::normalize(glm::cross(camera.front, camera.up)) * cameraSpeed;

  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	camera.pos += glm::normalize(glm::cross(camera.front, camera.up)) * cameraSpeed;

  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	camera.pos += camera.up * cameraSpeed;

  if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	camera.pos -= camera.up * cameraSpeed;

  static bool first_mouse = true;

  if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
	first_mouse = true;

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  }

  if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
	first_mouse = true;

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  }

  // MOUSE
  double cursor_x = 0;
  double cursor_y = 0;
  glfwGetCursorPos(window, &cursor_x, &cursor_y);
  glm::vec2 cursor_position(cursor_x, cursor_y);

  if (first_mouse) {
	camera.previous_cursor_position = cursor_position;
	first_mouse = false;
  }

  glm::vec2 cursor_delta = cursor_position - camera.previous_cursor_position;

  float sensitivity = 0.1f;
  cursor_delta *= sensitivity;

  camera.yaw += cursor_delta.x;
  camera.pitch += -cursor_delta.y;

  if (camera.pitch > 89.0f)
	camera.pitch = 89.0f;
  if (camera.pitch < -89.0f)
	camera.pitch = -89.0f;

  camera.computeFront();
  camera.previous_cursor_position = cursor_position;
}

GLFWwindow *init_glfw_and_opengl() {

  glfwSetErrorCallback(error_callback);

  if (!glfwInit()) {
	return nullptr;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  GLFWwindow *window = glfwCreateWindow(1280, 900, "OpenGL Engine", nullptr, nullptr);
  if (!window) {
	return nullptr;
  }

  glfwMakeContextCurrent(window);

  if (!load_opengl()) {
	spdlog::critical("Could not load OpenGL.");
	return nullptr;
  }

  spdlog::info(get_platform_info());

  int framebuffer_width = 0;
  int framebuffer_height = 0;
  glfwGetFramebufferSize(window, &framebuffer_width, &framebuffer_height);

  glViewport(0, 0, framebuffer_width, framebuffer_height);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  glEnable(GL_DEPTH_TEST);

  return window;
}

void terminate_glfw(GLFWwindow *window) {
  glfwDestroyWindow(window);
  window = nullptr;

  glfwTerminate();
}