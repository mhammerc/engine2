#pragma once

#include <entt/entt.hpp>

#include "entt/entity/fwd.hpp"

namespace engine {
auto duplicate_entity(entt::registry& registry, entt::entity source) -> entt::entity;
}