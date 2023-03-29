#pragma once

#include <entt/entt.hpp>

#include "../graphics/renderer_context.h"

namespace engine::systems {

auto camera_system(float delta_time, entt::registry& registry) -> void;

auto should_close_system() -> void;

auto draw_light_gizmo(entt::registry& registry, RendererContext renderer_context) -> void;

auto draw_outline(entt::registry& registry, RendererContext renderer_context) -> void;

auto draw_shadow_maps(entt::registry& registry) -> void;

auto draw_skybox(entt::registry& registry, RendererContext renderer_context) -> void;

}  // namespace engine::systems