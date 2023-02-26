#include <chrono>

#include "game_loop.h"

using namespace std::chrono;

void game_loop(GLFWwindow* window, const std::function<void(float, bool&)>& loop) {
    auto last_iteration = steady_clock::now();

    bool should_quit = false;

    while (glfwWindowShouldClose(window) == 0 && !should_quit) {
        auto current_iteration = steady_clock::now();

        std::chrono::duration<float> const delta_time = current_iteration - last_iteration;

        last_iteration = current_iteration;

        loop(delta_time.count(), should_quit);
    }
}