#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "../scene/camera.h"
#include "opengl.h"

auto init_glfw_and_opengl() -> GLFWwindow*;

auto process_inputs(float delta_time, GLFWwindow* window, Camera& camera) -> void;

auto terminate_glfw(GLFWwindow* window) -> void;
