#pragma once

#include "../opengl/opengl.h"
#include "../camera/camera.h"
#include <GLFW/glfw3.h>

auto init_glfw_and_opengl() -> GLFWwindow *;

auto processInputs(float delta_time, GLFWwindow *window, Camera &camera) -> void;

auto terminate_glfw(GLFWwindow *window) -> void;
