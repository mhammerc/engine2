#pragma once

#include "../camera/camera.h"
#include "../opengl/opengl.h"

auto init_glfw_and_opengl() -> GLFWwindow*;

auto processInputs(float delta_time, GLFWwindow* window, Camera& camera) -> void;

auto terminate_glfw(GLFWwindow* window) -> void;
