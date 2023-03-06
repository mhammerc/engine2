#include "entity.h"

#include "../scene/components/base_component.h"

using namespace engine;

/**
 * Duplicate a entity and its childrens.
 */
auto engine::duplicate_entity(entt::registry& registry, entt::entity source, bool first) -> entt::entity {
    auto destination = registry.create();

    for (auto [id, storage] : registry.storage()) {
        if (storage.contains(source)) {
            storage.emplace(destination, storage.get(source));
        }
    }

    auto* destination_base = registry.try_get<BaseComponent>(destination);

    if (first && destination_base->parent != entt::null) {
        // If we are duplicating an entity which have a parent,
        // We insert the duplicated entity alongside its siblings.
        // We do this only for the first entity to be duplicated, not recursive calls.
        auto& source_base = registry.get<BaseComponent>(source);

        destination_base->next_sibling = source_base.next_sibling;

        source_base.next_sibling = destination;
        destination_base->previous_sibling = source;
    }

    auto child = destination_base->first_child;
    entt::entity previous_new_child = entt::null;
    while (child != entt::null) {
        // If the duplicated entity have any child, duplicate the childs recursively.
        auto* child_base = registry.try_get<BaseComponent>(child);

        if (!child_base) {
            continue;
        }

        auto new_child = duplicate_entity(registry, child, false);
        registry.get<BaseComponent>(new_child).parent = destination;

        if (previous_new_child == entt::null) {
            destination_base->first_child = new_child;
        } else {
            registry.get<BaseComponent>(previous_new_child).next_sibling = new_child;
            registry.get<BaseComponent>(new_child).previous_sibling = previous_new_child;
        }

        child = child_base->next_sibling;
        previous_new_child = new_child;
    }

    return destination;
}