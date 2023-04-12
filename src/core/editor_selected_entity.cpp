#include "editor_selected_entity.h"

#include "../components/editor_selected_component.h"

using namespace engine;

auto engine::get_editor_selected_entity(const entt::registry& registry) -> entt::entity {
    auto entity = registry.view<EditorSelectedComponent>().front();

    return entity;
}

auto engine::set_editor_selected_entity(entt::registry& registry, entt::entity new_entity) -> entt::entity {
    auto previous_entity = get_editor_selected_entity(registry);

    if (previous_entity != entt::null) {
        registry.remove<EditorSelectedComponent>(previous_entity);
    }

    registry.emplace<EditorSelectedComponent>(new_entity);

    return new_entity;
}