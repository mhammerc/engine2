#pragma once

#include <entt/entt.hpp>

namespace engine::systems {

auto camera_system(float delta_time, entt::registry& registry) -> void;

auto should_close_system() -> void;

// TODO: goes to deferred renderer?
auto draw_shadow_maps(entt::registry& registry) -> void;

auto draw_render_deferred(entt::registry& registry) -> void;

}  // namespace engine::systems