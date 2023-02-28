#pragma once

#include <entt/entt.hpp>

#include "../scene/scene.h"

namespace engine {

auto gui_init(GLFWwindow* window) -> bool;
auto gui_prepare_frame() -> void;
auto gui_end_frame() -> void;
auto gui_show_system_window(Scene* scene, float delta_time, GLFWwindow* window, entt::registry& registry) -> void;

}  // namespace engine