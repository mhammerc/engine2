#pragma once

#include <entt/entt.hpp>

#include "graphics/renderer_context.h"

namespace engine {
auto create_scene(entt::registry& registry, RendererContext& renderer_context) -> void;
}