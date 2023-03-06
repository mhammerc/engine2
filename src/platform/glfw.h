#pragma once

#include "../utils/glfw3.h"

auto init_glfw_and_opengl() -> GLFWwindow*;

auto terminate_glfw(GLFWwindow* window) -> void;
