#pragma once

#include <functional>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

void game_loop(GLFWwindow* window, const std::function<void(float, bool&)>& loop);