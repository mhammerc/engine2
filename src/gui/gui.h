#pragma once

#include <GLFW/glfw3.h>

auto gui_init(GLFWwindow *window) -> bool;
auto gui_prepare_frame() -> void;
auto gui_end_frame() -> void;
auto gui_show_system_window(float delta_time, GLFWwindow *window) -> void;
