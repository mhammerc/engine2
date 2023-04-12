#pragma once

#include <entt/entity/registry.hpp>

namespace engine {

/**
 * The selected entity is the entity which have the component EditorSelectedEntity
 *
 * May return entt::null.
 */
auto get_editor_selected_entity(entt::registry const& registry) -> entt::entity;

auto set_editor_selected_entity(entt::registry& registry, entt::entity new_entity) -> entt::entity;

}  // namespace engine