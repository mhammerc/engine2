#pragma once

#include <functional>

#include <GLFW/glfw3.h>

void game_loop(GLFWwindow *window, const std::function<void(float, bool &)> &loop);