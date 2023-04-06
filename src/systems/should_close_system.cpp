#include "../core/input.hpp"
#include "../utils/glfw3.h"
#include "systems.h"

using namespace engine;

auto systems::should_close_system() -> void {
    PROFILER_BLOCK("systems::should_close");

    auto const& input = entt::locator<Input>::value();
    auto* window = entt::locator<GLFWwindow*>::value();

    if (input.keys[GLFW_KEY_ESCAPE]) {
        glfwSetWindowShouldClose(window, 1);
    }
}