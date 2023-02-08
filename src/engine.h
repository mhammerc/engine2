#pragma once

#include <GLFW/glfw3.h>
#include <functional>

void game_loop(GLFWwindow *window, const std::function<void(float, bool &)> &loop);