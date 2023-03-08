#pragma once

#include <entt/entt.hpp>

namespace engine::systems {

auto camera_system(float delta_time, entt::registry& registry) -> void;

auto should_close_system() -> void;

auto draw_light_gizmo(entt::registry& registry) -> void;

}  // namespace engine::systems