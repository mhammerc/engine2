#pragma once

#include "../scene/components/camera_component.h"
#include "../utils/glfw3.h"

auto init_glfw_and_opengl() -> GLFWwindow*;

auto process_inputs(float delta_time, GLFWwindow* window, engine::CameraComponent& camera) -> void;

auto terminate_glfw(GLFWwindow* window) -> void;
